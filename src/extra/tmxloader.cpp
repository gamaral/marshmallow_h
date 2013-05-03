/*
 * Copyright (c) 2011-2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
 * All rights reserved.
 *
 * This file is part of Marshmallow Game Engine.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the project as a whole.
 */

#include "extra/tmxloader.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/base64.h"
#include "core/gzip.h"
#include "core/identifier.h"
#include "core/logger.h"
#include "core/platform.h"
#include "core/type.h"
#include "core/zlib.h"

#include "graphics/backend.h"
#include "graphics/factory.h"
#include "graphics/quadmesh.h"
#include "graphics/tileset.h"

#include "game/entity.h"
#include "game/entityscenelayer.h"
#include "game/factory.h"
#include "game/positioncomponent.h"
#include "game/propertycomponent.h"
#include "game/rendercomponent.h"
#include "game/scene.h"
#include "game/sizecomponent.h"
#include "game/tilemapscenelayer.h"
#include "game/tilesetcomponent.h"

#include <map>

#include <cassert>

#include <tinyxml2.h>
namespace TinyXML = tinyxml2;

#define TMXLAYER_DATA_NODE          "data"
#define TMXLAYER_NODE               "layer"
#define TMXTILESET_IMAGE_NODE       "image"
#define TMXTILESET_NODE             "tileset"
#define TMXOBJECTGROUP_NODE         "objectgroup"
#define TMXOBJECTGROUP_OBJECT_NODE  "object"

#define TMXPROPERTIES_NODE          "properties"
#define TMXPROPERTIES_PROPERTY_NODE "property"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Extra { /****************************************** Extra Namespace */
namespace { /*********************************** Extra::<anonymous> Namespace */

	typedef std::map<uint16_t, Graphics::ITileset *> TilesetCollection;

	Graphics::Color
	PixelToColor(uint16_t *p)
	{
		return(Graphics::Color(static_cast<float>(p[0]) / 255.f,
		                       static_cast<float>(p[1]) / 255.f,
		                       static_cast<float>(p[2]) / 255.f));
	}

} /********************************************* Extra::<anonymous> Namespace */

struct TMXLoader::Private
{
	Private(Game::IScene *s)
	    : scene(s)
	    , base_directory(".")
	    , scale(1.f, 1.f)
	    , hrmap_size(0, 0)
	    , map_size(0, 0)
	    , tile_size(0, 0)
	    , is_loaded(false)
	{}

	bool load(const char *file);
	bool processLayer(TinyXML::XMLElement &element);
	bool processMap(TinyXML::XMLElement &element);
	bool processObjectGroup(TinyXML::XMLElement &element);
	bool processTileset(TinyXML::XMLElement &element);

	Game::IScene *scene;

	TilesetCollection tilesets;

	Game::SceneLayerList layers;

	std::string base_directory;

	Math::Size2f scale;
	Math::Size2f hrmap_size;
	Math::Size2i map_size;
	Math::Size2i tile_size;

	bool is_loaded;
};

bool
TMXLoader::Private::load(const char *f)
{
	TinyXML::XMLDocument l_tmx;
	TinyXML::XMLElement *l_root;

	is_loaded = false;

	if (l_tmx.LoadFile(f) != TinyXML::XML_NO_ERROR)
	    return(false);

	/* get parent directory */
	base_directory = Core::Platform::PathDirectory(f);

	/* parse general map data */
	if (!(l_root = l_tmx.RootElement())
	 || !processMap(*l_root))
	    return(false);

	/* parse tilesets */
	TinyXML::XMLElement *l_tileset = l_root->FirstChildElement(TMXTILESET_NODE);
	while (l_tileset) {
		if (!processTileset(*l_tileset))
			return(false);
		l_tileset = l_tileset->NextSiblingElement(TMXTILESET_NODE);
	}

	/* parse layers and object groups */
	TinyXML::XMLElement *l_element = l_root->FirstChildElement();
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
	Game::SceneLayerList::iterator l_layer_i;
	for (l_layer_i = layers.begin(); l_layer_i != layers.end(); ++l_layer_i)
		scene->pushLayer(*l_layer_i);

	return(is_loaded = true);
}

bool
TMXLoader::Private::processMap(TinyXML::XMLElement &m)
{
	if ((TinyXML::XML_SUCCESS != m.QueryIntAttribute("width", &map_size.width))
	 || (TinyXML::XML_SUCCESS != m.QueryIntAttribute("height", &map_size.height))
	 || (TinyXML::XML_SUCCESS != m.QueryIntAttribute("tilewidth", &tile_size.width))
	 || (TinyXML::XML_SUCCESS != m.QueryIntAttribute("tileheight", &tile_size.height))) {
		MMWARNING("Map element is missing one or more required attributes.");
		return(false);
	}

	/* default scale */
	scale.set(1.f, 1.f);

	/* process properties */
	TinyXML::XMLElement *l_properties = m.FirstChildElement(TMXPROPERTIES_NODE);
	TinyXML::XMLElement *l_property = l_properties ? l_properties->FirstChildElement(TMXPROPERTIES_PROPERTY_NODE) : 0;
	if (l_property)
	do {
		const char *l_pname = l_property->Attribute("name");
		if (!l_pname)
			continue;

		/* scale property */
		if (0 == MMSTRCASECMP(l_pname, "scale")) {
			const char *l_value = l_property->Attribute("value");
			if (!l_value) {
				MMWARNING("Skipping incomplete scale property.");
				continue;
			}

			if (0 == MMSTRCASECMP(l_value, "screen")) {
				const Math::Size2f &l_vsize = Graphics::Backend::Size();
				const Math::Size2i &l_wsize = Graphics::Backend::WindowSize();

				/*
				 * calculate pixels per viewport coordinate ratio
				 * scale ratio = vSize (vcoord)) / wSize (pixels)
				 */
				scale = l_vsize / l_wsize.cast<float>();

				continue;
			}
			else if (2 == sscanf(l_value, "%fx%f", &scale.width, &scale.height))
				continue;
			else if (1 == sscanf(l_value, "%f", &scale.width)) {
				scale.height = scale.width;
				continue;
			}

			MMERROR("Invalid scale value encountered.");
			continue;
		}

		/*
		 * Parse scene background color
		 */
		else if (0 == MMSTRCASECMP(l_pname, "bgcolor")) {
			const char *l_value = l_property->Attribute("value");
			if (!l_value) {
				MMWARNING("Skipping incomplete background color property.");
				continue;
			}

			/*
			 * Valid formats: #RRGGBB, RRGGBB.
			 */
			uint16_t l_pxl[3];
			if ((sscanf(l_value, "#%2hx%2hx%2hx",
			        &l_pxl[0], &l_pxl[1], &l_pxl[2]) != 3) &&
			    (sscanf(l_value,  "%2hx%2hx%2hx",
			        &l_pxl[0], &l_pxl[1], &l_pxl[2]) != 3)) {
				MMWARNING("Skipping invalid background color value.");
				continue;
			}

			/* set background color */
			Game::Scene *l_scene =
			    static_cast<Game::Scene *>(scene);
			l_scene->setBackground(PixelToColor(l_pxl));
			continue;
		}

	} while ((l_property = l_property->NextSiblingElement(TMXPROPERTIES_PROPERTY_NODE)));

	MMINFO("Map scale size (" << scale.width << "x" << scale.height << ")");

	/* calculate half-relative map size (used to offset coordinates) */
	hrmap_size.width = scale.width
	    * float(map_size.width  * tile_size.width);
	hrmap_size.height = scale.height
	    * float(map_size.height * tile_size.height);

	return(true);
}

bool
TMXLoader::Private::processLayer(TinyXML::XMLElement &e)
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

	TinyXML::XMLElement *l_data = e.FirstChildElement(TMXLAYER_DATA_NODE);

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
	const char *l_data_raw = TinyXML::XMLUtil::SkipWhiteSpace(l_data->GetText());
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
			    size_t(map_size.width * map_size.height * 4), &l_inflated_data))
				l_data_array = l_inflated_data;
		}
#define TMXDATA_COMPRESSION_GZIP "gzip"
		else if (0 == strcmp(l_data_compression, TMXDATA_COMPRESSION_GZIP)) {
			char *l_inflated_data;
			if (0 < Core::Gzip::Inflate(l_decoded_data, l_decoded_data_size,
			    size_t(map_size.width * map_size.height * 4), &l_inflated_data))
				l_data_array = l_inflated_data;
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

	Game::TilemapSceneLayer *l_layer = new Game::TilemapSceneLayer(l_name, scene);
	l_layer->setData(reinterpret_cast<uint32_t *>(l_data_array));
	l_layer->setOpacity(l_opacity);
	l_layer->setSize(map_size);
	l_layer->setTileSize(tile_size);
	l_layer->setScale(scale);
	l_layer->setVisibility(1 == l_visible);

	/* process properties */
	TinyXML::XMLElement *l_properties = e.FirstChildElement(TMXPROPERTIES_NODE);
	TinyXML::XMLElement *l_property = l_properties ? l_properties->FirstChildElement(TMXPROPERTIES_PROPERTY_NODE) : 0;
	if (l_property)
	do {
		const char *l_pname = l_property->Attribute("name");
		const char *l_value = l_property->Attribute("value");
		if (!l_pname)
			continue;

		/* scale property */
		if (0 == strcmp(l_pname, "scale")) {
			if (!l_value) {
				MMWARNING("Skipping incomplete scale property.");
				continue;
			}

			Math::Size2f l_scale = l_layer->scale();
			if (0 == MMSTRCASECMP(l_value, "screen")) {
				const Math::Size2f &l_vsize = Graphics::Backend::Size();
				const Math::Size2i &l_wsize = Graphics::Backend::WindowSize();

				/*
				 * calculate pixels per viewport coordinate ratio
				 * scale ratio = vSize (vcoord)) / wSize (pixels)
				 */
				l_scale = l_vsize / l_wsize.cast<float>();
				l_layer->setScale(l_scale);

				continue;
			}
			else if (2 == sscanf(l_value, "%fx%f", &l_scale.width, &l_scale.height)) {
				l_layer->setScale(l_scale);
				continue;
			} else if (1 == sscanf(l_value, "%f", &l_scale.width)) {
				l_scale.height = l_scale.width;
				l_layer->setScale(l_scale);
				continue;
			}

			MMERROR("Invalid scale value encountered.");
			continue;
		}
		else l_layer->setProperty(l_name, l_value ? l_value : std::string());

	} while ((l_property = l_property->NextSiblingElement(TMXPROPERTIES_PROPERTY_NODE)));

	/* attach tilesets */
	TilesetCollection::iterator l_tileset_i;
	for (l_tileset_i = tilesets.begin(); l_tileset_i != tilesets.end(); ++l_tileset_i)
		l_layer->attachTileset(l_tileset_i->first, l_tileset_i->second);

	layers.push_back(l_layer);

	return(true);
}

bool
TMXLoader::Private::processObjectGroup(TinyXML::XMLElement &e)
{
	const char *l_name;

	if ((!(l_name = e.Attribute("name")))) {
		MMWARNING("Object group element is missing one or more required attributes.");
		return(false);
	}

	Game::EntitySceneLayer *l_layer = new Game::EntitySceneLayer(l_name, scene);

	TinyXML::XMLElement *l_object = e.FirstChildElement(TMXOBJECTGROUP_OBJECT_NODE);
	while (l_object) {
		const char *l_object_name;
		const char *l_object_type;
		int l_object_gid;
		int l_object_x;
		int l_object_y;
		int l_object_width = tile_size.width;
		int l_object_height = tile_size.height;

		l_object_name = l_object->Attribute("name");
		l_object_type = l_object->Attribute("type");

		Game::IEntity *l_entity = Game::Factory::Instance()->
		    createEntity(l_object_type, l_object_name ? l_object_name : "", l_layer);
		if (!l_entity) {
			MMWARNING("Object '" << l_object_name << "' of type '" << l_object_type << "' was left unhandled.");
			return(false);
		}

		if ((TinyXML::XML_SUCCESS != l_object->QueryIntAttribute("x", &l_object_x))
		 || (TinyXML::XML_SUCCESS != l_object->QueryIntAttribute("y", &l_object_y))) {
			MMWARNING("Object element is missing one or more required attributes.");
			return(false);
		}

		/* map offset position (0 in the middle) */
		l_object_x -= (map_size.width * tile_size.width) / 2;
		l_object_y -= (map_size.height * tile_size.height) / 2;
		l_object_y *= -1; /* invert top/bottom */

		/* object size (later initialized) */
		Math::Size2f l_object_rsize;
		Math::Size2f l_object_hrsize;

		/* standard object */
		if (TinyXML::XML_SUCCESS != l_object->QueryIntAttribute("gid", &l_object_gid)) {
			l_object->QueryIntAttribute("width",  &l_object_width);
			l_object->QueryIntAttribute("height", &l_object_height);

			/* calculate object size */
			l_object_rsize.width = scale.width * float(l_object_width);
			l_object_rsize.height = scale.height * float(l_object_height);
			l_object_hrsize = l_object_rsize / 2.f;

		}

		/* tile object */
		else {
			bool l_found = false;
			uint16_t l_ts_firstgid = 0;

			/* offset position to top-left (later centered) */
			l_object_y += l_object_height;

			/* look for appropriate tileset */
			TilesetCollection::iterator l_tileset_i;
			for (l_tileset_i = tilesets.begin(); l_tileset_i != tilesets.end(); ++l_tileset_i)
				if (l_tileset_i->first > l_ts_firstgid && l_tileset_i->first <= l_object_gid) {
					l_ts_firstgid = l_tileset_i->first;
					l_found = true;
				}

			if (!l_found) {
				MMWARNING("Object tile GID tileset was not found.");
				return(false);
			}

			Graphics::ITileset *l_tileset = tilesets[l_ts_firstgid];

			/* calculate object size from tileset */
			l_object_width  = l_tileset->tileSize().width;
			l_object_height = l_tileset->tileSize().height;
			l_object_rsize.width  = scale.width  * float(l_object_width);
			l_object_rsize.height = scale.height * float(l_object_height);
			l_object_hrsize = l_object_rsize / 2.f;

			/* attach tileset used */

			Game::TilesetComponent *l_tscomponent = new Game::TilesetComponent("tileset", l_entity);
			l_tscomponent->setTileset(l_tileset);
			l_entity->pushComponent(l_tscomponent);

			/* generate tile mesh */

			Game::RenderComponent *l_render = new Game::RenderComponent("render", l_entity);

			Graphics::IVertexData *l_vdata =
			    Graphics::Backend::Factory::CreateVertexData(MARSHMALLOW_QUAD_VERTEXES);
			l_vdata->set(0, -l_object_hrsize.width,  l_object_hrsize.height);
			l_vdata->set(1, -l_object_hrsize.width, -l_object_hrsize.height);
			l_vdata->set(2,  l_object_hrsize.width,  l_object_hrsize.height);
			l_vdata->set(3,  l_object_hrsize.width, -l_object_hrsize.height);

			Graphics::ITextureCoordinateData *l_tdata =
			    l_tileset->getTextureCoordinateData(uint16_t(l_object_gid - l_ts_firstgid));

			l_render->setMesh(new Graphics::QuadMesh
			    (l_tdata, l_tileset->textureData(), l_vdata, Graphics::QuadMesh::None));

			l_entity->pushComponent(l_render);
		}

		/* create position component */
		Game::PositionComponent *l_pos_component = new Game::PositionComponent("position", l_entity);
		l_pos_component->setPosition(scale.width  * float(l_object_x),
		                             scale.height * float(l_object_y));

		/* change position to center of object (offset) */
		l_pos_component->translate(l_object_hrsize.width,
		                          -l_object_hrsize.height);

		l_entity->pushComponent(l_pos_component);

		/* create size component */
		Game::SizeComponent *l_size = new Game::SizeComponent("size", l_entity);
		l_size->size() = l_object_rsize;
		l_entity->pushComponent(l_size);

		/* object properties */
		TinyXML::XMLElement *l_properties = l_object->FirstChildElement(TMXPROPERTIES_NODE);
		TinyXML::XMLElement *l_property = l_properties ? l_properties->FirstChildElement(TMXPROPERTIES_PROPERTY_NODE) : 0;
		if (l_property) {
			Game::PropertyComponent *l_pcomponent =
			    new Game::PropertyComponent("property", l_entity);

			do {
				const char *l_pname = l_property->Attribute("name");
				const char *l_value = l_property->Attribute("value");

				if (!l_pname || !l_value)
					continue;

				l_pcomponent->set(l_pname, l_value);
			} while ((l_property = l_property->NextSiblingElement(TMXPROPERTIES_PROPERTY_NODE)));

			l_entity->pushComponent(l_pcomponent);
		}

		/* add entity to layer */
		l_layer->addEntity(l_entity);

		l_object = l_object->NextSiblingElement(TMXOBJECTGROUP_OBJECT_NODE);
	}

	layers.push_back(l_layer);

	return(true);
}

bool
TMXLoader::Private::processTileset(TinyXML::XMLElement &e)
{
	int l_first_gid;
	const char *l_name;
	int l_tile_width;
	int l_tile_height;
	int l_tile_margin = 0;
	int l_tile_spacing = 0;

	if ((TinyXML::XML_SUCCESS != e.QueryIntAttribute("firstgid", &l_first_gid))
	 || (!(l_name = e.Attribute("name")))
	 || (TinyXML::XML_SUCCESS != e.QueryIntAttribute("tilewidth", &l_tile_width))
	 || (TinyXML::XML_SUCCESS != e.QueryIntAttribute("tileheight", &l_tile_height))) {
		MMWARNING("Tileset element is missing one or more required attributes.");
		return(false);
	}

	/* check for margin and spacing */
	MMIGNORE e.QueryIntAttribute("margin", &l_tile_margin);
	MMIGNORE e.QueryIntAttribute("spacing", &l_tile_spacing);

	TinyXML::XMLElement *l_image = e.FirstChildElement(TMXTILESET_IMAGE_NODE);
	if (!l_image) {
		MMWARNING("Tileset element is missing an image element.");
		return(false);
	}

	/*
	 * Texture sources are required to be relative to TMX file.
	 *  - So say we all.
	 */
	const std::string l_source =
	    base_directory + "/" + l_image->Attribute("source");

#if 0   // TODO(gamaral) use transparent color.
	const char *l_trans = l_image->Attribute("trans");
	(void) l_trans;
#endif

	Graphics::ITextureData *l_texture =
	    Graphics::Backend::Factory::CreateTextureData();
	if (!l_texture->load(l_source)) {
		MMERROR("Failed to load tileset texture.");
		return(false);
	}

	Graphics::Tileset *l_tileset = new Graphics::Tileset;
	l_tileset->setName(l_name);
	l_tileset->setMargin(l_tile_margin);
	l_tileset->setSpacing(l_tile_spacing);
	l_tileset->setTileSize(Math::Size2i(l_tile_width, l_tile_height));
	l_tileset->setTextureData(l_texture);
	tilesets[uint16_t(l_first_gid)] = l_tileset;

	return(true);
}

/****************************************************************** TMXLoader */

TMXLoader::TMXLoader(Game::IScene *s)
    : PIMPL_CREATE_X(s)
{
}

TMXLoader::~TMXLoader(void)
{
	PIMPL_DESTROY;
}

bool
TMXLoader::isLoaded(void) const
{
	return(PIMPL->is_loaded);
}

const Game::SceneLayerList &
TMXLoader::layers(void) const
{
	return(PIMPL->layers);
}

bool
TMXLoader::load(const char *f)
{
	return(PIMPL->load(f));
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

