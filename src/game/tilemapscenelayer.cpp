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

#include "game/tilemapscenelayer.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <cassert>
#include <cmath>

#include "math/point2.h"
#include "math/vector2.h"

#include "graphics/factory.h"
#include "graphics/painter.h"
#include "graphics/quadmesh.h"
#include "graphics/transform.h"
#include "graphics/viewport.h"

MARSHMALLOW_NAMESPACE_USE
using namespace Game;

const Core::Type TilemapSceneLayer::sType("Game::TilemapSceneLayer");

/******************************************************************************/

namespace {
	typedef std::map<UINT32, Graphics::SharedTileset> TilesetCollection;
	typedef std::map<UINT32, Graphics::SharedVertexData> VertexDataCache;
} // namespace

/******************************************************************************/

struct TilemapSceneLayer::Private
{
	TilesetCollection tilesets;

	VertexDataCache vertexes;

	UINT32 *data;

	Math::Size2f hrsize;
	Math::Size2f hrtile_size;
	Math::Size2f rsize;
	Math::Size2f rtile_size;

	Math::Vector2 translate;

	Math::Size2i size;
	Math::Size2i tile_size;
	Math::Size2f scale;

	float opacity;

	bool  visible;
};

TilemapSceneLayer::TilemapSceneLayer(const Core::Identifier &i, IScene &s)
    : SceneLayerBase(i, s)
    , m_p(new Private)
{
	m_p->data = 0;
	m_p->opacity = 1.0f;
	m_p->visible = true;
	m_p->scale = Math::Size2f::Identity();

	recalculateRelativeTileSize();
}

TilemapSceneLayer::~TilemapSceneLayer(void)
{
	delete [] m_p->data;
	m_p->data = 0;

	delete m_p;
	m_p = 0;
}

Graphics::SharedTileset
TilemapSceneLayer::tileset(UINT32 i, UINT32 *o)
{
	TilesetCollection::iterator l_i;
	TilesetCollection::const_iterator l_end = m_p->tilesets.end();

	UINT32 l_offset = 0;
	Graphics::SharedTileset l_ts;

	for (l_i = m_p->tilesets.begin(); l_i != l_end; ++l_i)
		if (l_i->first <= i && l_offset < l_i->first) {
			l_offset = l_i->first;
			l_ts = l_i->second;
		}

	if (l_offset > 0) {
		if (o) *o = l_offset;
		return(l_ts);
	} else return(Graphics::SharedTileset());
}

void
TilemapSceneLayer::attachTileset(UINT32 o, Graphics::SharedTileset ts)
{
	m_p->tilesets[o] = ts;
	m_p->vertexes[o] = Graphics::Factory::CreateVertexData(MARSHMALLOW_QUAD_VERTEXES);
	recalculateVertexData(o);
}

void
TilemapSceneLayer::dettachTileset(UINT32 o)
{
	m_p->tilesets.erase(o);
	m_p->vertexes.erase(o);
}

const UINT32 *
TilemapSceneLayer::data(void) const
{
	return(m_p->data);
}

void
TilemapSceneLayer::setData(UINT32 *d)
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
	recalculateRelativeTileSize();
	recalculateAllVertexData();
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
	recalculateRelativeTileSize();
	recalculateAllVertexData();
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
	recalculateRelativeTileSize();
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

void
TilemapSceneLayer::render(void)
{
	if (!m_p->data || !m_p->visible) return;

	/* calculate visible row and column range */

	const bool  l_camera_rotated = Graphics::Viewport::Camera().rotation() != 0;
	const float l_camera_x = Graphics::Viewport::Camera().translation().x() + m_p->translate.x();
	const float l_camera_y = Graphics::Viewport::Camera().translation().y() + m_p->translate.y();

	float col_start_cam;
	float col_stop_cam;
	float row_start_cam;
	float row_stop_cam;

	if (l_camera_rotated) {
		const float l_visible_radius = sqrtf(Graphics::Viewport::Radius2());
		col_start_cam = l_camera_x - l_visible_radius;
		col_stop_cam  = l_camera_x + l_visible_radius;
		row_start_cam = l_camera_y + l_visible_radius;
		row_stop_cam  = l_camera_y - l_visible_radius;
	}
	else {
		const Math::Size2f l_hviewport_size = Graphics::Viewport::ScaledSize() / 2.f;
		col_start_cam = l_camera_x - l_hviewport_size.width()  - m_p->hrtile_size.width();
		col_stop_cam  = l_camera_x + l_hviewport_size.width()  + m_p->hrtile_size.width();
		row_start_cam = l_camera_y + l_hviewport_size.height() + m_p->hrtile_size.height();
		row_stop_cam  = l_camera_y - l_hviewport_size.height() - m_p->hrtile_size.height();
	}

	int col_start = static_cast<int>(floorf(((col_start_cam + m_p->hrsize.width()) / m_p->rsize.width())
	    * static_cast<float>(m_p->size.width())));
	int col_stop  = static_cast<int>(ceilf(((col_stop_cam + m_p->hrsize.width()) / m_p->rsize.width())
	    * static_cast<float>(m_p->size.width())));
	int row_start = static_cast<int>(floorf(((row_start_cam - m_p->hrsize.height()) / m_p->rsize.height())
	    * static_cast<float>(-m_p->size.height())));
	int row_stop  = static_cast<int>(ceilf(((row_stop_cam - m_p->hrsize.height()) / m_p->rsize.height())
	    * static_cast<float>(-m_p->size.height())));

	/* draw tiles */

	Graphics::Color l_color(1.f, 1.f, 1.f, m_p->opacity);

	for (int l_r = row_start; l_r < row_stop; ++l_r) {
		const int l_rindex = l_r % m_p->size.height();
		const int l_roffset = ((l_rindex >= 0 ? 0 : m_p->size.height()) + l_rindex) * m_p->size.width();

		for (int l_c = col_start; l_c < col_stop; ++l_c) {
			const int l_cindex = l_c % m_p->size.width();
			UINT32 l_tindex = m_p->data[l_roffset + (l_cindex >= 0 ? 0 : m_p->size.width()) + l_cindex];
			if (!l_tindex)
				continue;

			/* calculate tile position */
			float l_x = static_cast<float>(l_c) * m_p->rtile_size.width();
			float l_y = static_cast<float>(m_p->size.height() - l_r) * m_p->rtile_size.height();

			/* offset to center of viewport */
			l_x -= m_p->hrsize.width();
			l_y -= m_p->hrsize.height();

			/* translate */
			l_x -= m_p->translate.x();
			l_y -= m_p->translate.y();

			/* offset to bottom of tile (we draw up) */
			l_y -= m_p->rtile_size.height();

			UINT32 l_tioffset;
			Graphics::SharedTileset l_ts = tileset(l_tindex, &l_tioffset);
			Graphics::SharedTextureCoordinateData l_tcd;

			if (l_ts) {
				Graphics::SharedVertexData &l_svdata = m_p->vertexes[l_tioffset];

				Graphics::QuadMesh l_mesh(l_ts->getTextureCoordinateData(static_cast<UINT16>(l_tindex - l_tioffset)),
				    l_ts->textureData(), l_svdata);
				l_mesh.setColor(l_color);

				Graphics::Painter::Draw(l_mesh, Math::Point2(l_x, l_y));
			}
		}
	}
}

void
TilemapSceneLayer::recalculateAllVertexData()
{
	VertexDataCache::iterator l_i;
	VertexDataCache::const_iterator l_end = m_p->vertexes.end();

	for (l_i = m_p->vertexes.begin(); l_i != l_end; ++l_i)
		recalculateVertexData(l_i->first);
}

void
TilemapSceneLayer::recalculateRelativeTileSize()
{
	/* convert tile size to relative tile size */
	m_p->rtile_size[0] =
	    (static_cast<float>(m_p->tile_size.width())  * m_p->scale.width());
	m_p->rtile_size[1] =
	    (static_cast<float>(m_p->tile_size.height()) * m_p->scale.height());
	m_p->hrtile_size = m_p->rtile_size / 2.0f;

	/* calculate relative map size */
	m_p->rsize[0] = m_p->rtile_size.width()  * static_cast<float>(m_p->size.width());
	m_p->rsize[1] = m_p->rtile_size.height() * static_cast<float>(m_p->size.height());
	m_p->hrsize = m_p->rsize / 2.f;
}

void
TilemapSceneLayer::recalculateVertexData(UINT32 o)
{
	assert(m_p->vertexes[o] && "Invalid vertex data!");
	assert(m_p->tilesets[o] && "Invalid tileset!");

	/*
	 * tileset tile size should be scale of the map tile size
	 * tts (32x32) / mts (16x16) = 2x scale
	 */
	const int l_tswscale = m_p->tilesets[o]->tileSize().width() / m_p->tile_size.width();
	const int l_tshscale = m_p->tilesets[o]->tileSize().height() / m_p->tile_size.height();

	float l_vwidth  = (static_cast<float>(l_tswscale) * m_p->rtile_size.width());
	float l_vheight = (static_cast<float>(l_tshscale) * m_p->rtile_size.height());

	/* anchor to (0,0) */
	m_p->vertexes[o]->set(0, 0,        l_vheight);
	m_p->vertexes[o]->set(1, 0,        0);
	m_p->vertexes[o]->set(2, l_vwidth, l_vheight);
	m_p->vertexes[o]->set(3, l_vwidth, 0);
}

const Core::Type &
TilemapSceneLayer::Type(void)
{
	return(sType);
}

