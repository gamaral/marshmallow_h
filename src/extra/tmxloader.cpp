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

#include <tinyxml2.h>

#include <map>

#define TMXLAYER_DATA_NODE    "data"
#define TMXLAYER_NODE         "layer"
#define TMXTILESET_IMAGE_NODE "image"
#define TMXTILESET_NODE       "tileset"
#define TMXOBJECTGROUP_NODE   "objectgroup"
#define TMXOBJECTGROUP_OBJECT_NODE "object"

#define TMXPROPERTIES_NODE    "properties"
#define TMXPROPERTIES_PROPERTY_NODE "property"

MARSHMALLOW_NAMESPACE_USE
using namespace Extra;

/******************************************************************************/

namespace {
	typedef std::map<uint16_t, Graphics::SharedTileset> TilesetCollection;
} // namespace

/******************************************************************************/

struct TMXLoader::Private
{
	Private(Game::IScene &s)
	    : scene(s)
	    , is_loaded(false)
	    , scale(1.f, 1.f)
	    , hrmap_size(0, 0)
	    , map_size(0, 0)
	    , tile_size(0, 0) {}

	Game::IScene &scene;

	TilesetCollection tilesets;

	Game::SharedSceneLayerList layers;

	bool is_loaded;

	Math::Size2f scale;
	Math::Size2f hrmap_size;
	Math::Size2i map_size;
	Math::Size2i tile_size;
};

TMXLoader::TMXLoader(Game::IScene &s)
    : m_p(new Private(s))
{
}

TMXLoader::~TMXLoader(void)
{
	delete m_p;
	m_p = 0;
}

bool
TMXLoader::isLoaded(void) const
{
	return(m_p->is_loaded);
}

const Game::SharedSceneLayerList &
TMXLoader::layers(void) const
{
	return(m_p->layers);
}

bool
TMXLoader::load(const char *f)
{
	TinyXML::XMLDocument l_tmx;
	XMLElement *l_root;

	m_p->is_loaded = false;

	if (l_tmx.LoadFile(f) != XML_NO_ERROR)
	    return(false);

	/* parse general map data */
	if (!(l_root = l_tmx.RootElement())
	 || !processMap(*l_root))
	    return(false);

	/* parse tilesets */
	XMLElement *l_tileset = l_root->FirstChildElement(TMXTILESET_NODE);
	while (l_tileset) {
		if (!processTileset(*l_tileset))
			return(false);
		l_tileset = l_tileset->NextSiblingElement(TMXTILESET_NODE);
	}

	/* parse layers and object groups */
	XMLElement *l_element = l_root->FirstChildElement();
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
	for (l_layer_i = m_p->layers.begin(); l_layer_i != m_p->layers.end(); ++l_layer_i)
		m_p->scene.pushLayer(*l_layer_i);

	return(m_p->is_loaded = true);
}

bool
TMXLoader::processMap(XMLElement &m)
{
	if ((XML_SUCCESS != m.QueryIntAttribute("width", &m_p->map_size[0]))
	 || (XML_SUCCESS != m.QueryIntAttribute("height", &m_p->map_size[1]))
	 || (XML_SUCCESS != m.QueryIntAttribute("tilewidth", &m_p->tile_size[0]))
	 || (XML_SUCCESS != m.QueryIntAttribute("tileheight", &m_p->tile_size[1]))) {
		MMWARNING("Map element is missing one or more required attributes.");
		return(false);
	}

	/* calculate default scale */
	const Math::Size2f &l_vsize = Graphics::Viewport::Size();
	if (m_p->map_size.width() < m_p->map_size.height()) {
		m_p->scale[0] = l_vsize.width()  / static_cast<float>(m_p->map_size.width()  * m_p->tile_size.width());
		m_p->scale[1] = m_p->scale.width() * static_cast<float>(m_p->tile_size.height() / m_p->tile_size.width());
		MMINFO("Calculated scale based off map size width.");
	}
	if (m_p->map_size.width() > m_p->map_size.height()) {
		m_p->scale[1] = l_vsize.height() / static_cast<float>(m_p->map_size.height() * m_p->tile_size.height());
		m_p->scale[0] = m_p->scale.height() * static_cast<float>(m_p->tile_size.width() / m_p->tile_size.height());
		MMINFO("Calculated scale based off map size height.");
	}
	else m_p->scale[0] = m_p->scale[1] = l_vsize.height() / static_cast<float>(m_p->map_size.height() * m_p->tile_size.height());
	MMINFO("Calculated map scale size (" << m_p->scale.width() << "x" << m_p->scale.height() << ")");

	/* calculate half-relative map size (used to offset coordinates) */
	m_p->hrmap_size[0] = m_p->scale.width()
	    * static_cast<float>(m_p->map_size.width() * m_p->tile_size.width());
	m_p->hrmap_size[1] = m_p->scale.height()
	    * static_cast<float>(m_p->map_size.height() * m_p->tile_size.height());

	/* process properties */
	XMLElement *l_properties = m.FirstChildElement(TMXPROPERTIES_NODE);
	XMLElement *l_property = l_properties ? l_properties->FirstChildElement(TMXPROPERTIES_PROPERTY_NODE) : 0;
	if (l_property)
	do {
		const char *l_pname = l_property->Attribute("name");
		if (!l_pname)
			continue;

		/* scale property */
		if (STRCASECMP(l_pname, "scale") == 0) {
			const char *l_value = l_property->Attribute("value");
			if (!l_value) {
				MMWARNING("Skipping incomplete scale property.");
				continue;
			}

			if (STRCASECMP(l_value, "screen") == 0) {
				const Math::Size2i &l_wsize = Graphics::Viewport::WindowSize();

				/*
				 * calculate pixels per viewport coordinate ratio
				 * scale ratio = vSize (vcoord)) / wSize (pixels)
				 */
				m_p->scale[0] = l_vsize.width()  / static_cast<float>(l_wsize.width());
				m_p->scale[1] = l_vsize.height() / static_cast<float>(l_wsize.height());

				continue;
			}
			else if (sscanf(l_value, "%fx%f", &m_p->scale[0], &m_p->scale[1]) == 2)
				continue;
			else if (sscanf(l_value, "%f", &m_p->scale[0]) == 1) {
				m_p->scale[1] = m_p->scale[0];
				continue;
			}

			MMERROR("Invalid scale value encountered.");
			continue;
		}

	} while ((l_property = l_property->NextSiblingElement(TMXPROPERTIES_PROPERTY_NODE)));

	return(true);
}

bool
TMXLoader::processLayer(XMLElement &e)
{
	const char *l_name;
	float l_opacity = 1.f;
	int l_visible = 1;

	if ((!(l_name = e.Attribute("name")))) {
		MMWARNING("Layer element is missing one or more required attributes.");
		return(false);
	}

	MMIGNORE e.QueryFloatAttribute("opacity", &l_opacity);
	MMIGNORE e.QueryIntAttribute("visible", &l_visible);

	XMLElement *l_data = e.FirstChildElement(TMXLAYER_DATA_NODE);

	if (!l_data) {
		MMWARNING("Layer element is missing data element.");
		return(false);
	}

	const char *l_data_encoding;
	const char *l_data_compression;

	if (!(l_data_encoding = l_data->Attribute("encoding")) ||
	    !(l_data_compression = l_data->Attribute("compression"))) {
		MMWARNING("Layer data element is missing one or more required attributes.");
		return(false);
	}
	const char *l_data_raw = XMLUtil::SkipWhiteSpace(l_data->GetText());
	size_t l_data_raw_len = strlen(l_data_raw);

	/* right-side data trim */
	while (l_data_raw_len > 0 && isspace(l_data_raw[l_data_raw_len - 1]))
		--l_data_raw_len;

	if (!l_data_raw_len) {
		MMWARNING("Zero size layer data encountered.");
		return(false);
	}

	char *l_data_array = 0;

#define TMXDATA_ENCODING_BASE64 "base64"
	if (0 == strcmp(l_data_encoding, TMXDATA_ENCODING_BASE64)) {
		char *l_decoded_data;
		size_t l_decoded_data_size =
		    Core::Base64::Decode(l_data_raw, l_data_raw_len, &l_decoded_data);

#define TMXDATA_COMPRESSION_ZLIB "zlib"
		if (0 == strcmp(l_data_compression, TMXDATA_COMPRESSION_ZLIB)) {
			char *l_inflated_data;
			if (0 < Core::Zlib::Inflate(l_decoded_data, l_decoded_data_size,
			    static_cast<size_t>(m_p->map_size.width() * m_p->map_size.height() * 4), &l_inflated_data))
				l_data_array = l_inflated_data;
		}
#define TMXDATA_COMPRESSION_GZIP "gzip"
		else if (0 == strcmp(l_data_compression, TMXDATA_COMPRESSION_GZIP)) {
			// TODO(gamaral)
			assert(0 && "GZIP data decompression is currently unimplemented");
			return(false);
		}

		delete[] l_decoded_data;
	}

#define TMXDATA_ENCODING_CSV "csv"
	else if (0 == strcmp(l_data_encoding, TMXDATA_ENCODING_CSV)) {
		// TODO(gamaral)
		assert(0 && "CSV data encoding is currently unimplemented");
		return(false);
	}

	if (!l_data_array)
		return(false);

	Game::TilemapSceneLayer *l_layer = new Game::TilemapSceneLayer(l_name, m_p->scene);
	l_layer->setData(reinterpret_cast<uint32_t *>(l_data_array));
	l_layer->setOpacity(l_opacity);
	l_layer->setSize(m_p->map_size);
	l_layer->setTileSize(m_p->tile_size);
	l_layer->setScale(m_p->scale);
	l_layer->setVisibility(1 == l_visible);

	/* process properties */
	XMLElement *l_properties = e.FirstChildElement(TMXPROPERTIES_NODE);
	XMLElement *l_property = l_properties ? l_properties->FirstChildElement(TMXPROPERTIES_PROPERTY_NODE) : 0;
	if (l_property)
	do {
		const char *l_pname = l_property->Attribute("name");
		if (!l_pname)
			continue;

		/* scale property */
		if (strcmp(l_pname, "scale") == 0) {
			const char *l_value = l_property->Attribute("value");
			if (!l_value) {
				MMWARNING("Skipping incomplete scale property.");
				continue;
			}

			Math::Size2f l_scale = l_layer->scale();
			if (STRCASECMP(l_value, "screen") == 0) {
				const Math::Size2f &l_vsize = Graphics::Viewport::Size();
				const Math::Size2i &l_wsize = Graphics::Viewport::WindowSize();

				/*
				 * calculate pixels per viewport coordinate ratio
				 * scale ratio = vSize (vcoord)) / wSize (pixels)
				 */
				l_scale[0] = l_vsize.width()  / static_cast<float>(l_wsize.width());
				l_scale[1] = l_vsize.height() / static_cast<float>(l_wsize.height());
				l_layer->setScale(l_scale);

				continue;
			}
			else if (sscanf(l_value, "%fx%f", &l_scale[0], &l_scale[1]) == 2) {
				l_layer->setScale(l_scale);
				continue;
			} else if (sscanf(l_value, "%f", &l_scale[0]) == 1) {
				l_scale[1] = l_scale[0];
				l_layer->setScale(l_scale);
				continue;
			}

			MMERROR("Invalid scale value encountered.");
			continue;
		}

	} while ((l_property = l_property->NextSiblingElement(TMXPROPERTIES_PROPERTY_NODE)));

	/* attach tilesets */
	TilesetCollection::iterator l_tileset_i;
	for (l_tileset_i = m_p->tilesets.begin(); l_tileset_i != m_p->tilesets.end(); ++l_tileset_i)
		l_layer->attachTileset(l_tileset_i->first, l_tileset_i->second);

	m_p->layers.push_back(l_layer);

	return(true);
}

bool
TMXLoader::processObjectGroup(XMLElement &e)
{
	const char *l_name;

	if ((!(l_name = e.Attribute("name")))) {
		MMWARNING("Object group element is missing one or more required attributes.");
		return(false);
	}

	Game::EntitySceneLayer *l_layer = new Game::EntitySceneLayer(l_name, m_p->scene);

	XMLElement *l_object = e.FirstChildElement(TMXOBJECTGROUP_OBJECT_NODE);
	while (l_object) {
		const char *l_object_name;
		const char *l_object_type;
		int l_object_gid;
		int l_object_x;
		int l_object_y;
		int l_object_width = m_p->tile_size.width();
		int l_object_height = m_p->tile_size.height();

		l_object_name = l_object->Attribute("name");
		l_object_type = l_object->Attribute("type");

		Game::SharedEntity l_entity = Game::Factory::Instance()->
		    createEntity(l_object_type, l_object_name ? l_object_name : "", *l_layer);
		if (!l_entity) {
			MMWARNING("Object '" << l_object_name << "' of type '" << l_object_type << "' was left unhandled.");
			return(false);
		}

		if ((XML_SUCCESS != l_object->QueryIntAttribute("x", &l_object_x))
		 || (XML_SUCCESS != l_object->QueryIntAttribute("y", &l_object_y))) {
			MMWARNING("Object element is missing one or more required attributes.");
			return(false);
		}

		/* map offset position (0 in the middle) */
		l_object_x -= (m_p->map_size.width()  * m_p->tile_size.width())  / 2;
		l_object_y -= (m_p->map_size.height() * m_p->tile_size.height()) / 2;
		l_object_y *= -1; /* invert top/bottom */

		/* object size (later initialized) */
		Math::Size2f l_object_rsize;
		Math::Size2f l_object_hrsize;

		/* standard object */
		if (XML_SUCCESS != l_object->QueryIntAttribute("gid", &l_object_gid)) {
			l_object->QueryIntAttribute("width",  &l_object_width);
			l_object->QueryIntAttribute("height", &l_object_height);

			/* calculate object size */
			l_object_rsize[0] = m_p->scale.width()  * static_cast<float>(l_object_width);
			l_object_rsize[1] = m_p->scale.height() * static_cast<float>(l_object_height);
			l_object_hrsize = l_object_rsize / 2.f;

		/* tile object */
		} else {
			bool l_found = false;
			uint16_t l_ts_firstgid = 0;

			/* offset position to top-left (later centered) */
			l_object_y += l_object_height;

			/* look for appropriate tileset */
			TilesetCollection::iterator l_tileset_i;
			for (l_tileset_i = m_p->tilesets.begin(); l_tileset_i != m_p->tilesets.end(); ++l_tileset_i)
				if (l_tileset_i->first > l_ts_firstgid && l_tileset_i->first <= l_object_gid) {
					l_ts_firstgid = l_tileset_i->first;
					l_found = true;
				}

			if (!l_found) {
				MMWARNING("Object tile GID tileset was not found.");
				return(false);
			}

			Graphics::SharedTileset l_tileset = m_p->tilesets[l_ts_firstgid];

			/* calculate object size from tileset */
			l_object_width  = l_tileset->tileSize().width();
			l_object_height = l_tileset->tileSize().height();
			l_object_rsize[0] = m_p->scale.width()  * static_cast<float>(l_object_width);
			l_object_rsize[1] = m_p->scale.height() * static_cast<float>(l_object_height);
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
			    l_tileset->getTextureCoordinateData(static_cast<uint16_t>(l_object_gid - l_ts_firstgid));

			l_render->mesh() =
			    new Graphics::QuadMesh(l_tdata, l_tileset->textureData(), l_vdata);

			l_entity->pushComponent(l_render);
		}

		/* create position component */
		Game::PositionComponent *l_pos_component = new Game::PositionComponent("position", *l_entity);
		l_pos_component->position()[0] = m_p->scale.width()  * static_cast<float>(l_object_x);
		l_pos_component->position()[1] = m_p->scale.height() * static_cast<float>(l_object_y);

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

	m_p->layers.push_back(l_layer);

	return(true);
}

bool
TMXLoader::processTileset(XMLElement &e)
{
	int l_first_gid;
	const char *l_name;
	int l_tile_width;
	int l_tile_height;
	int l_tile_margin = 0;
	int l_tile_spacing = 0;

	if ((XML_SUCCESS != e.QueryIntAttribute("firstgid", &l_first_gid))
	 || (!(l_name = e.Attribute("name")))
	 || (XML_SUCCESS != e.QueryIntAttribute("tilewidth", &l_tile_width))
	 || (XML_SUCCESS != e.QueryIntAttribute("tileheight", &l_tile_height))) {
		MMWARNING("Tileset element is missing one or more required attributes.");
		return(false);
	}

	/* check for margin and spacing */
	MMIGNORE e.QueryIntAttribute("margin", &l_tile_margin);
	MMIGNORE e.QueryIntAttribute("spacing", &l_tile_spacing);

	XMLElement *l_image = e.FirstChildElement(TMXTILESET_IMAGE_NODE);
	if (!l_image) {
		MMWARNING("Tileset element is missing an image element.");
		return(false);
	}
	const char *l_source = l_image->Attribute("source");
	const char *l_trans = l_image->Attribute("trans");

	Graphics::SharedTextureData l_texture = Graphics::Factory::CreateTextureData();
	if (!l_texture->load(l_source)) {
		MMERROR("Failed to load tileset texture.");
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
	m_p->tilesets[static_cast<uint16_t>(l_first_gid)] = l_tileset;

	return(true);
}

