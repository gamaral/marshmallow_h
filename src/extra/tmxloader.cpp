/*
 * Copyright 2011 Marshmallow Engine. All rights reserved.
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
#include "graphics/tileset.h"

#include "game/tilemapscenelayer.h"
#include "game/iscene.h"

#define TMXLAYER_DATA_NODE    "data"
#define TMXLAYER_NODE         "layer"
#define TMXTILESET_IMAGE_NODE "image"
#define TMXTILESET_NODE       "tileset"

MARSHMALLOW_NAMESPACE_USE;
using namespace Extra;

TMXLoader::TMXLoader(Game::IScene &s)
    : m_scene(s)
    , m_is_loaded(false)
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

	/* parse layers */
	TiXmlElement *l_layer = l_root->FirstChildElement(TMXLAYER_NODE);
	while (l_layer) {
		if (!processLayer(*l_layer))
			return(false);
		l_layer = l_layer->NextSiblingElement(TMXLAYER_NODE);
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
	if ((TIXML_SUCCESS != m.QueryIntAttribute("width", &m_map_size.rwidth()))
	 || (TIXML_SUCCESS != m.QueryIntAttribute("height", &m_map_size.rheight()))
	 || (TIXML_SUCCESS != m.QueryIntAttribute("tilewidth", &m_tile_size.rwidth()))
	 || (TIXML_SUCCESS != m.QueryIntAttribute("tileheight", &m_tile_size.rheight()))) {
		MMWARNING1("Map element is missing one or more required attributes.");
		return(false);
	}

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

	e.QueryFloatAttribute("opacity", &l_opacity);
	e.QueryIntAttribute("visible", &l_visible);

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
			    m_map_size.rwidth() * m_map_size.rheight() * 4, &l_inflated_data))
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
TMXLoader::processTileset(TiXmlElement &e)
{
	int l_first_gid;
	const char *l_name;
	int l_margin = 0;
	int l_spacing = 0;
	int l_tile_height;
	int l_tile_width;

	if ((TIXML_SUCCESS != e.QueryIntAttribute("firstgid", &l_first_gid))
	 || (!(l_name = e.Attribute("name")))
	 || (TIXML_SUCCESS != e.QueryIntAttribute("tilewidth", &l_tile_width))
	 || (TIXML_SUCCESS != e.QueryIntAttribute("tileheight", &l_tile_height))) {
		MMWARNING1("Tileset element is missing one or more required attributes.");
		return(false);
	}
	e.QueryIntAttribute("margin", &l_margin);
	e.QueryIntAttribute("spacing", &l_spacing);

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
	l_tileset->setTextureData(l_texture);
	l_tileset->setTileSize(Math::Size2i(l_tile_width, l_tile_height));
	m_tilesets[l_first_gid] = l_tileset;

	return(true);
}

