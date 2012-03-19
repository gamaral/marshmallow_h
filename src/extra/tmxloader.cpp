/*
 * Copyright 2011-2012 Marshmallow Engine. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this list of
 *      conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this list
 *      of conditions and the following disclaimer in the documentation and/or other materials
 *      provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY MARSHMALLOW ENGINE ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MARSHMALLOW ENGINE OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those of the
 * authors and should not be interpreted as representing official policies, either expressed
 * or implied, of Marshmallow Engine.
 */

#include "extra/tmxloader.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <tinyxml.h>
using namespace TinyXML;

#include "core/base64.h"
#include "core/logger.h"
#include "core/zlib.h"

#include "graphics/factory.h"
#include "graphics/quadmesh.h"
#include "graphics/tileset.h"
#include "graphics/viewport.h"

#include "game/entity.h"
#include "game/entityscenelayer.h"
#include "game/factory.h"
#include "game/iscene.h"
#include "game/positioncomponent.h"
#include "game/rendercomponent.h"
#include "game/sizecomponent.h"
#include "game/tilesetcomponent.h"
#include "game/tilemapscenelayer.h"

#define TMXLAYER_DATA_NODE    "data"
#define TMXLAYER_NODE         "layer"
#define TMXTILESET_IMAGE_NODE "image"
#define TMXTILESET_NODE       "tileset"
#define TMXOBJECTGROUP_NODE   "objectgroup"
#define TMXOBJECTGROUP_OBJECT_NODE "object"

MARSHMALLOW_NAMESPACE_USE
using namespace Extra;

TMXLoader::TMXLoader(Game::IScene &s)
    : m_scene(s)
    , m_is_loaded(false)
    , m_conv_ratio(0, 0)
    , m_hrmap_size(0, 0)
    , m_map_size(0, 0)
    , m_tile_size(0, 0)
{
}

TMXLoader::~TMXLoader(void)
{
}

bool
TMXLoader::load(const char *f)
{
	TiXmlDocument l_tmx;
	TiXmlElement *l_root;

	m_is_loaded = false;

	if (!l_tmx.LoadFile(f))
	    return(false);

	/* parse general map data */
	if (!(l_root = l_tmx.RootElement())
	 || !processMap(*l_root))
	    return(false);

	/* parse tilesets */
	TiXmlElement *l_tileset = l_root->FirstChildElement(TMXTILESET_NODE);
	while (l_tileset) {
		if (!processTileset(*l_tileset))
			return(false);
		l_tileset = l_tileset->NextSiblingElement(TMXTILESET_NODE);
	}

	/* parse layers and object groups */
	TiXmlElement *l_element = l_root->FirstChildElement();
	while (l_element) {
		if (0 == strcmp(l_element->Value(), TMXLAYER_NODE)
		    && !processLayer(*l_element))
			return(false);
		else if (0 == strcmp(l_element->Value(), TMXOBJECTGROUP_NODE)
		    && !processObjectGroup(*l_element))
			return(false);
		l_element = l_element->NextSiblingElement();
	}

	/* attach layers to scene */
	Game::SharedSceneLayerList::iterator l_layer_i;
	for (l_layer_i = m_layers.begin(); l_layer_i != m_layers.end(); ++l_layer_i)
		m_scene.pushLayer(*l_layer_i);

	return(m_is_loaded = true);
}

bool
TMXLoader::processMap(TiXmlElement &m)
{
	if ((TIXML_SUCCESS != m.QueryIntAttribute("width", &m_map_size[0]))
	 || (TIXML_SUCCESS != m.QueryIntAttribute("height", &m_map_size[1]))
	 || (TIXML_SUCCESS != m.QueryIntAttribute("tilewidth", &m_tile_size[0]))
	 || (TIXML_SUCCESS != m.QueryIntAttribute("tileheight", &m_tile_size[1]))) {
		MMWARNING1("Map element is missing one or more required attributes.");
		return(false);
	}

	const Math::Size2f &l_vsize = Graphics::Viewport::Size();
	const Math::Size2i &l_wsize = Graphics::Viewport::WindowSize();

	/* calculate pixels per viewport coordinate ratio */
	m_conv_ratio[0] =
	    static_cast<float>(l_vsize.width())
	        / static_cast<float>(l_wsize.width());
	m_conv_ratio[1] =
	    static_cast<float>(l_vsize.height())
	        / static_cast<float>(l_wsize.height());

	/* calculate half-relative map size (used to offset coordinates) */
	m_hrmap_size[0] = m_conv_ratio.width()
	    * static_cast<float>(m_map_size.width() * m_tile_size.width());
	m_hrmap_size[1] = m_conv_ratio.height()
	    * static_cast<float>(m_map_size.height() * m_tile_size.height());

	return(true);
}

bool
TMXLoader::processLayer(TinyXML::TiXmlElement &e)
{
	const char *l_name;
	float l_opacity = 1.f;
	int l_visible = 1;

	if ((!(l_name = e.Attribute("name")))) {
		MMWARNING1("Layer element is missing one or more required attributes.");
		return(false);
	}

	MMIGNORE e.QueryFloatAttribute("opacity", &l_opacity);
	MMIGNORE e.QueryIntAttribute("visible", &l_visible);

	TiXmlElement *l_data = e.FirstChildElement(TMXLAYER_DATA_NODE);

	if (!l_data) {
		MMWARNING1("Layer element is missing data element.");
		return(false);
	}

	const char *l_data_encoding;
	const char *l_data_compression;

	if (!(l_data_encoding = l_data->Attribute("encoding")) ||
	    !(l_data_compression = l_data->Attribute("compression"))) {
		MMWARNING1("Layer data element is missing one or more required attributes.");
		return(false);
	}
	const char *l_data_raw = l_data->GetText();
	char *l_data_array = 0;

#define TMXDATA_ENCODING_BASE64 "base64"
	if (0 == strcmp(l_data_encoding, TMXDATA_ENCODING_BASE64)) {
		char *l_decoded_data;
		size_t l_decoded_data_size =
		    Core::Base64::Decode(l_data_raw, strlen(l_data_raw), &l_decoded_data);

#define TMXDATA_COMPRESSION_ZLIB "zlib"
		if (0 == strcmp(l_data_compression, TMXDATA_COMPRESSION_ZLIB)) {
			char *l_inflated_data;
			if (0 < Core::Zlib::Inflate(l_decoded_data, l_decoded_data_size,
			    static_cast<size_t>(m_map_size.width() * m_map_size.height() * 4), &l_inflated_data))
				l_data_array = l_inflated_data;
		}
#define TMXDATA_COMPRESSION_GZIP "gzip"
		else if (0 == strcmp(l_data_compression, TMXDATA_COMPRESSION_GZIP)) {
			// TODO(gamaral)
			assert(1 && "GZIP data decompression is currently unimplemented");
		}

		delete[] l_decoded_data;
	}

#define TMXDATA_ENCODING_CSV "csv"
	else if (0 == strcmp(l_data_encoding, TMXDATA_ENCODING_CSV)) {
		// TODO(gamaral)
		assert(1 && "CSV data encoding is currently unimplemented");
	}

	if (!l_data_array)
		return(false);

	Game::TilemapSceneLayer *l_layer = new Game::TilemapSceneLayer(l_name, m_scene);
	l_layer->setData(reinterpret_cast<UINT32 *>(l_data_array));
	l_layer->setOpacity(l_opacity);
	l_layer->setSize(m_map_size);
	l_layer->setTileSize(m_tile_size);
	l_layer->setVisibility(1 == l_visible);

	/* attach tilesets */
	TilesetCollection::iterator l_tileset_i;
	for (l_tileset_i = m_tilesets.begin(); l_tileset_i != m_tilesets.end(); ++l_tileset_i)
		l_layer->attachTileset(l_tileset_i->first, l_tileset_i->second);

	m_layers.push_back(l_layer);

	return(true);
}

bool
TMXLoader::processObjectGroup(TinyXML::TiXmlElement &e)
{
	const char *l_name;

	if ((!(l_name = e.Attribute("name")))) {
		MMWARNING1("Object group element is missing one or more required attributes.");
		return(false);
	}

	Game::EntitySceneLayer *l_layer = new Game::EntitySceneLayer(l_name, m_scene);

	TiXmlElement *l_object = e.FirstChildElement(TMXOBJECTGROUP_OBJECT_NODE);
	while (l_object) {
		const char *l_object_name;
		const char *l_object_type;
		int l_object_gid;
		int l_object_x;
		int l_object_y;
		int l_object_width = m_tile_size.width();
		int l_object_height = m_tile_size.height();

		l_object_name = l_object->Attribute("name");
		l_object_type = l_object->Attribute("type");

		Game::SharedEntity l_entity = Game::Factory::Instance()->
		    createEntity(l_object_type, l_object_name ? l_object_name : "", *l_layer);
		if (!l_entity) {
			MMWARNING("Object '%s' of type '%s' was left unhandled.", l_object_name, l_object_type);
			return(false);
		}

		if ((TIXML_SUCCESS != l_object->QueryIntAttribute("x", &l_object_x))
		 || (TIXML_SUCCESS != l_object->QueryIntAttribute("y", &l_object_y))) {
			MMWARNING1("Object element is missing one or more required attributes.");
			return(false);
		}

		/* map offset position (0 in the middle) */
		l_object_x -= (m_map_size.width()  * m_tile_size.width())  / 2;
		l_object_y -= (m_map_size.height() * m_tile_size.height()) / 2;
		l_object_y *= -1; /* invert top/bottom */

		/* object size (later initialized) */
		Math::Size2f l_object_rsize;
		Math::Size2f l_object_hrsize;

		/* standard object */
		if (TIXML_SUCCESS != l_object->QueryIntAttribute("gid", &l_object_gid)) {
			l_object->QueryIntAttribute("width",  &l_object_width);
			l_object->QueryIntAttribute("height", &l_object_height);

			/* calculate object size */
			l_object_rsize[0] = m_conv_ratio.width()  * static_cast<float>(l_object_width);
			l_object_rsize[1] = m_conv_ratio.height() * static_cast<float>(l_object_height);
			l_object_hrsize = l_object_rsize / 2.f;

		/* tile object */
		} else {
			bool l_found = false;
			UINT16 l_ts_firstgid = 0;

			/* offset position to top-left (later centered) */
			l_object_y += l_object_height;

			/* look for appropriate tileset */
			TilesetCollection::iterator l_tileset_i;
			for (l_tileset_i = m_tilesets.begin(); l_tileset_i != m_tilesets.end(); ++l_tileset_i)
				if (l_tileset_i->first > l_ts_firstgid && l_tileset_i->first <= l_object_gid) {
					l_ts_firstgid = l_tileset_i->first;
					l_found = true;
				}

			if (!l_found) {
				MMWARNING1("Object tile GID tileset was not found.");
				return(false);
			}

			Graphics::SharedTileset l_tileset = m_tilesets[l_ts_firstgid];

			/* calculate object size from tileset */
			l_object_width  = l_tileset->tileSize().width();
			l_object_height = l_tileset->tileSize().height();
			l_object_rsize[0] = m_conv_ratio.width()  * static_cast<float>(l_object_width);
			l_object_rsize[1] = m_conv_ratio.height() * static_cast<float>(l_object_height);
			l_object_hrsize = l_object_rsize / 2.f;

			/* attach tileset used */

			Game::TilesetComponent *l_tscomponent = new Game::TilesetComponent("tileset", *l_entity);
			l_tscomponent->tileset() = l_tileset;
			l_entity->pushComponent(l_tscomponent);

			/* generate tile mesh */

			Game::RenderComponent *l_render = new Game::RenderComponent("render", *l_entity);

			Graphics::SharedVertexData l_vdata = Graphics::Factory::CreateVertexData(MARSHMALLOW_QUAD_VERTEXES);
			l_vdata->set(0, -l_object_hrsize.width(),  l_object_hrsize.height());
			l_vdata->set(1, -l_object_hrsize.width(), -l_object_hrsize.height());
			l_vdata->set(2,  l_object_hrsize.width(),  l_object_hrsize.height());
			l_vdata->set(3,  l_object_hrsize.width(), -l_object_hrsize.height());

			Graphics::SharedTextureCoordinateData l_tdata =
			    l_tileset->getTextureCoordinateData(static_cast<UINT16>(l_object_gid) - l_ts_firstgid);

			l_render->mesh() =
			    new Graphics::QuadMesh(l_tdata, l_tileset->textureData(), l_vdata);

			l_entity->pushComponent(l_render);
		}

		/* create position component */
		Game::PositionComponent *l_pos_component = new Game::PositionComponent("position", *l_entity);
		l_pos_component->position()[0] = m_conv_ratio.width()  * static_cast<float>(l_object_x);
		l_pos_component->position()[1] = m_conv_ratio.height() * static_cast<float>(l_object_y);

		/* change position to center of object (offset) */
		l_pos_component->position()[0] += l_object_hrsize.width();
		l_pos_component->position()[1] -= l_object_hrsize.height();

		l_entity->pushComponent(l_pos_component);

		/* create size component */
		Game::SizeComponent *l_size = new Game::SizeComponent("size", *l_entity);
		l_size->size() = l_object_rsize;
		l_entity->pushComponent(l_size);


		l_layer->addEntity(l_entity);

		l_object = l_object->NextSiblingElement(TMXOBJECTGROUP_OBJECT_NODE);
	}

	m_layers.push_back(l_layer);

	return(true);
}

bool
TMXLoader::processTileset(TiXmlElement &e)
{
	int l_first_gid;
	const char *l_name;
	int l_tile_width;
	int l_tile_height;
	int l_tile_margin = 0;
	int l_tile_spacing = 0;

	if ((TIXML_SUCCESS != e.QueryIntAttribute("firstgid", &l_first_gid))
	 || (!(l_name = e.Attribute("name")))
	 || (TIXML_SUCCESS != e.QueryIntAttribute("tilewidth", &l_tile_width))
	 || (TIXML_SUCCESS != e.QueryIntAttribute("tileheight", &l_tile_height))) {
		MMWARNING1("Tileset element is missing one or more required attributes.");
		return(false);
	}

	/* check for margin and spacing */
	MMIGNORE e.QueryIntAttribute("margin", &l_tile_margin);
	MMIGNORE e.QueryIntAttribute("spacing", &l_tile_spacing);

	TiXmlElement *l_image = e.FirstChildElement(TMXTILESET_IMAGE_NODE);
	if (!l_image) {
		MMWARNING1("Tileset element is missing an image element.");
		return(false);
	}
	const char *l_source = l_image->Attribute("source");
	const char *l_trans = l_image->Attribute("trans");

	Graphics::SharedTextureData l_texture = Graphics::Factory::CreateTextureData();
	if (!l_texture->load(l_source)) {
		MMERROR1("Failed to load tileset texture.");
		return(false);
	}
	// TODO(gamaral) use transparent color.
	(void) l_trans;

	Graphics::Tileset *l_tileset = new Graphics::Tileset;
	l_tileset->setName(l_name);
	l_tileset->setMargin(l_tile_margin);
	l_tileset->setSpacing(l_tile_spacing);
	l_tileset->setTileSize(Math::Size2i(l_tile_width, l_tile_height));
	l_tileset->setTextureData(l_texture);
	m_tilesets[static_cast<UINT16>(l_first_gid)] = l_tileset;

	return(true);
}

