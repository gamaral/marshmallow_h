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

#include "game/tilemapscenelayer.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <map>

#include <cassert>

#include "core/type.h"

#include "graphics/camera.h"
#include "graphics/factory.h"
#include "graphics/painter.h"
#include "graphics/quadmesh.h"
#include "graphics/transform.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */
namespace { /************************************ Game::<anonymous> Namespace */
	typedef std::map<uint32_t, Graphics::ITileset *> TilesetCollection;
	typedef std::map<uint32_t, Graphics::IVertexData *> VertexDataCache;
	typedef std::map<std::string, std::string> PropertyMap;
} /********************************************** Game::<anonymous> Namespace */

/******************************************************************************/

struct TilemapSceneLayer::Private
{
	Private(void)
	    : scale(Math::Size2f::Identity())
	    , opacity(1.0f)
	    , data(0)
	    , visible(true) {}

	~Private(void)
	{
		delete [] data;
		data = 0;
	}

	Graphics::ITileset * tileset(uint32_t i, uint32_t *o);
	void render(void);

	void recalculateAllVertexData();
	void recalculateRelativeTileSize();
	void recalculateVertexData(uint32_t offset);

	TilesetCollection tilesets;
	VertexDataCache vertexes;
	PropertyMap properties;

	Math::Size2f hrsize;
	Math::Size2f hrtile_size;
	Math::Size2f rsize;
	Math::Size2f rtile_size;

	Math::Vector2 translate;

	Math::Size2i size;
	Math::Size2i tile_size;
	Math::Size2f scale;

	float opacity;

	uint32_t *data;

	bool  visible;
};

Graphics::ITileset *
TilemapSceneLayer::Private::tileset(uint32_t i, uint32_t *o)
{
	TilesetCollection::iterator l_i;
	TilesetCollection::const_iterator l_end = tilesets.end();

	uint32_t l_offset = 0;
	Graphics::ITileset *l_ts;

	for (l_i = tilesets.begin(); l_i != l_end; ++l_i)
		if (l_i->first <= i && l_offset < l_i->first) {
			l_offset = l_i->first;
			l_ts = l_i->second;
		}

	if (l_offset > 0) {
		if (o) *o = l_offset;
		return(l_ts);
	} else return(0);
}

void
TilemapSceneLayer::Private::render(void)
{
	if (!data || !visible) return;

	typedef std::map<uint32_t, uint32_t> TileIndexCount;
	typedef std::pair<float, float> Origin;
	typedef std::multimap<uint32_t, Origin> TileIndexOrigins;
	typedef std::pair<uint32_t, Origin> TileIndexOrigin;
	typedef std::pair<TileIndexOrigins::iterator, TileIndexOrigins::iterator> TileIndexOriginsRange;
	TileIndexCount   l_tile_count;
	TileIndexOrigins l_tile_origins;
	uint32_t         l_tile_count_max = 0;

	/* calculate visible row and column range */

	const Graphics::Transform &l_camera = Graphics::Camera::Transform();
	const bool  l_camera_rotated = l_camera.rotation() != 0;
	const float l_camera_x = l_camera.translation().x + translate.x;
	const float l_camera_y = l_camera.translation().y + translate.y;

	float col_start_cam;
	float col_stop_cam;
	float row_start_cam;
	float row_stop_cam;

	if (l_camera_rotated) {
		const float l_visible_radius = sqrtf(Graphics::Camera::VisibleMagnitude2());
		col_start_cam = l_camera_x - l_visible_radius;
		col_stop_cam  = l_camera_x + l_visible_radius;
		row_start_cam = l_camera_y + l_visible_radius;
		row_stop_cam  = l_camera_y - l_visible_radius;
	}
	else {
		Math::Size2f l_hviewport_size = Graphics::Camera::Visiblility() / 2.f;

		col_start_cam = l_camera_x - l_hviewport_size.width  - hrtile_size.width;
		col_stop_cam  = l_camera_x + l_hviewport_size.width  + hrtile_size.width;
		row_start_cam = l_camera_y + l_hviewport_size.height + hrtile_size.height;
		row_stop_cam  = l_camera_y - l_hviewport_size.height - hrtile_size.height;
	}

	int col_start = static_cast<int>(floorf(((col_start_cam + hrsize.width) / rsize.width)
	    * static_cast<float>(size.width)));
	int col_stop  = static_cast<int>(ceilf(((col_stop_cam + hrsize.width) / rsize.width)
	    * static_cast<float>(size.width)));
	int row_start = static_cast<int>(floorf(((row_start_cam - hrsize.height) / rsize.height)
	    * static_cast<float>(-size.height)));
	int row_stop  = static_cast<int>(ceilf(((row_stop_cam - hrsize.height) / rsize.height)
	    * static_cast<float>(-size.height)));

	/* calculate tile origins */

	for (int l_r = row_start; l_r < row_stop; ++l_r) {
		const int l_rindex = l_r % size.height;
		const int l_roffset = ((l_rindex >= 0 ? 0 : size.height) + l_rindex) * size.width;

		for (int l_c = col_start; l_c < col_stop; ++l_c) {
			const int l_cindex = l_c % size.width;
			uint32_t l_tindex = data[l_roffset + (l_cindex >= 0 ? 0 : size.width) + l_cindex];
			if (!l_tindex)
				continue;

			/* calculate tile position */
			float l_x = static_cast<float>(l_c) * rtile_size.width;
			float l_y = static_cast<float>(size.height - l_r) * rtile_size.height;

			/* offset to center of viewport */
			l_x -= hrsize.width;
			l_y -= hrsize.height;

			/* translate */
			l_x -= translate.x;
			l_y -= translate.y;

			/* offset to bottom of tile (we draw up) */
			l_y -= rtile_size.height;

			/* add to cache */

			uint32_t l_current_count = (l_tile_count[l_tindex] += 1);
			if (l_tile_count_max < l_current_count)
				l_tile_count_max = l_current_count;

			l_tile_origins.insert(TileIndexOrigin(l_tindex, Origin(l_x, l_y)));
		}
	}

	/* draw tiles */

	Math::Point2   *l_origins = new Math::Point2[l_tile_count_max];
	Graphics::Color l_color(1.f, 1.f, 1.f, opacity);

	TileIndexCount::iterator ti;
	TileIndexCount::iterator te = l_tile_count.end();
	for (ti = l_tile_count.begin(); ti != te; ++ti) {
		TileIndexOriginsRange l_range = l_tile_origins.equal_range(ti->first);
		TileIndexOrigins::iterator oi;
		uint32_t oic = 0;
		for (oi = l_range.first; oi != l_range.second; ++oi)
			l_origins[oic++].set(oi->second.first, oi->second.second);

		assert(oic == ti->second && "Tile origin count mismatch!");

		uint32_t l_tioffset;
		Graphics::ITileset *l_ts = tileset(ti->first, &l_tioffset);
		if (l_ts) {
			Graphics::IVertexData *l_svdata = vertexes[l_tioffset];

			Graphics::QuadMesh l_mesh(l_ts->getTextureCoordinateData(static_cast<uint16_t>(ti->first - l_tioffset)),
			    l_ts->textureData(), l_svdata);
			l_mesh.setColor(l_color);

			Graphics::Painter::Draw(l_mesh, l_origins, oic);
		}
	}
	delete [] l_origins;
}

void
TilemapSceneLayer::Private::recalculateAllVertexData()
{
	VertexDataCache::iterator l_i;
	VertexDataCache::const_iterator l_end = vertexes.end();

	for (l_i = vertexes.begin(); l_i != l_end; ++l_i)
		recalculateVertexData(l_i->first);
}

void
TilemapSceneLayer::Private::recalculateRelativeTileSize()
{
	/* convert tile size to relative tile size */
	rtile_size.set(static_cast<float>(tile_size.width) * scale.width,
	               static_cast<float>(tile_size.height) * scale.height);
	hrtile_size = rtile_size / 2.0f;

	/* calculate relative map size */
	rsize.set(rtile_size.width * static_cast<float>(size.width),
	          rtile_size.height * static_cast<float>(size.height));
	hrsize = rsize / 2.f;
}

void
TilemapSceneLayer::Private::recalculateVertexData(uint32_t o)
{
	assert(vertexes[o] && "Invalid vertex data!");
	assert(tilesets[o] && "Invalid tileset!");

	/*
	 * tileset tile size should be scale of the map tile size
	 * tts (32x32) / mts (16x16) = 2x scale
	 */
	const int l_tswscale = tilesets[o]->tileSize().width / tile_size.width;
	const int l_tshscale = tilesets[o]->tileSize().height / tile_size.height;

	const float l_vwidth  = (static_cast<float>(l_tswscale) * rtile_size.width);
	const float l_vheight = (static_cast<float>(l_tshscale) * rtile_size.height);

	/* anchor to (0,0) */
	vertexes[o]->set(0, 0,        l_vheight);
	vertexes[o]->set(1, 0,        0);
	vertexes[o]->set(2, l_vwidth, l_vheight);
	vertexes[o]->set(3, l_vwidth, 0);
}

/******************************************************************************/

TilemapSceneLayer::TilemapSceneLayer(const Core::Identifier &i, IScene &s)
    : SceneLayerBase(i, s)
    , m_p(new Private)
{
	m_p->recalculateRelativeTileSize();
}

TilemapSceneLayer::~TilemapSceneLayer(void)
{
	delete m_p, m_p = 0;
}

Graphics::ITileset *
TilemapSceneLayer::tileset(uint32_t i, uint32_t *o)
{
	return(m_p->tileset(i, o));
}

void
TilemapSceneLayer::attachTileset(uint32_t o, Graphics::ITileset *ts)
{
	m_p->tilesets[o] = ts;
	m_p->vertexes[o] =
	    Graphics::Backend::Factory::CreateVertexData(MARSHMALLOW_QUAD_VERTEXES);
	m_p->recalculateVertexData(o);
}

void
TilemapSceneLayer::dettachTileset(uint32_t o)
{
	m_p->tilesets.erase(o);
	m_p->vertexes.erase(o);
}

const uint32_t *
TilemapSceneLayer::data(void) const
{
	return(m_p->data);
}

void
TilemapSceneLayer::setData(uint32_t *d)
{
	delete [] m_p->data;
	m_p->data = d;
}

const Math::Vector2 &
TilemapSceneLayer::translate(void) const
{
	return(m_p->translate);
}

void
TilemapSceneLayer::setTranslation(const Math::Vector2 &t)
{
	m_p->translate = t;
}

const Math::Size2i &
TilemapSceneLayer::size(void) const
{
	return(m_p->size);
}

void
TilemapSceneLayer::setSize(const Math::Size2i &s)
{
	m_p->size = s;
	m_p->recalculateRelativeTileSize();
	m_p->recalculateAllVertexData();
}

const Math::Size2i &
TilemapSceneLayer::tileSize(void) const
{
	return(m_p->tile_size);
}

void
TilemapSceneLayer::setTileSize(const Math::Size2i &s)
{
	m_p->tile_size = s;
	m_p->recalculateRelativeTileSize();
	m_p->recalculateAllVertexData();
}

const Math::Size2f &
TilemapSceneLayer::scale(void) const
{
	return(m_p->scale);
}

void
TilemapSceneLayer::setScale(const Math::Size2f &s)
{
	m_p->scale = s;
	m_p->recalculateRelativeTileSize();
}

float
TilemapSceneLayer::opacity(void) const
{
	return(m_p->opacity);
}

void
TilemapSceneLayer::setOpacity(float a)
{
	m_p->opacity = a;
}

bool
TilemapSceneLayer::visible(void) const
{
	return(m_p->visible);
}

void
TilemapSceneLayer::setVisibility(bool v)
{
	m_p->visible = v;
}

bool
TilemapSceneLayer::hasProperty(const std::string &n, std::string *v) const
{
	PropertyMap::const_iterator l_i = m_p->properties.find(n);
	const bool l_found = (l_i != m_p->properties.end());
	if (l_found && v) *v = l_i->second;
	return(l_found);
}

const std::string &
TilemapSceneLayer::property(const std::string &n) const
{
	return(m_p->properties[n]);
}

void
TilemapSceneLayer::setProperty(const std::string &n, const std::string &v)
{
	m_p->properties[n] = v;
}

const Math::Size2f &
TilemapSceneLayer::virtualSize(void) const
{
	return(m_p->rsize);
}

const Math::Size2f &
TilemapSceneLayer::virtualHalfSize(void) const
{
	return(m_p->hrsize);
}

void
TilemapSceneLayer::render(void)
{
	m_p->render();
}

const Core::Type &
TilemapSceneLayer::Type(void)
{
	static const Core::Type s_type("Game::TilemapSceneLayer");
	return(s_type);
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

