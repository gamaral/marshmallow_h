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

#include "game/tilemapscenelayer.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <cassert>

#include "math/point2.h"
#include "math/vector2.h"

#include "graphics/factory.h"
#include "graphics/painter.h"
#include "graphics/quadmesh.h"
#include "graphics/transform.h"
#include "graphics/viewport.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Game;

const Core::Type TilemapSceneLayer::sType("Game::TilemapSceneLayer");

TilemapSceneLayer::TilemapSceneLayer(const Core::Identifier &i, IScene &s)
    : SceneLayerBase(i, s)
    , m_data(0)
    , m_hrtile_size(0.f, 0.f)
    , m_hrsize(0.f, 0.f)
    , m_rsize(0.f, 0.f)
    , m_rtile_size(0.f, 0.f)
    , m_tile_size(24, 24)
    , m_size(100, 100)
    , m_opacity(1.0f)
    , m_visible(true)
{
	recalculateRelativeTileSize();
}

TilemapSceneLayer::~TilemapSceneLayer(void)
{
	delete [] m_data;
	m_data = 0;
}

Graphics::SharedTileset
TilemapSceneLayer::tileset(int i, int *o)
{
	TilesetCollection::iterator l_i;
	TilesetCollection::const_iterator l_end = m_tilesets.end();

	int l_offset = 0;
	Graphics::SharedTileset l_ts;

	for (l_i = m_tilesets.begin(); l_i != l_end; ++l_i)
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
TilemapSceneLayer::attachTileset(int o, Graphics::SharedTileset ts)
{
	m_tilesets[o] = ts;
	m_vertexes[o] = Graphics::Factory::CreateVertexData(QUAD_VERTEXES);
	recalculateVertexData(o);
}

void
TilemapSceneLayer::dettachTileset(int o)
{
	m_tilesets.erase(o);
	m_vertexes.erase(o);
}

void
TilemapSceneLayer::setTileSize(const Math::Size2i &s)
{
	m_tile_size = s;
	recalculateRelativeTileSize();
	recalculateAllVertexData();
}

void
TilemapSceneLayer::setSize(const Math::Size2i &s)
{
	m_size = s;
	recalculateRelativeTileSize();
	recalculateAllVertexData();
}

void
TilemapSceneLayer::setOpacity(float a)
{
	m_opacity = a;
}

void
TilemapSceneLayer::setVisibility(bool value)
{
	m_visible = value;
}

void
TilemapSceneLayer::setData(UINT32 *d)
{
	delete [] m_data;
	m_data = d;
}

void
TilemapSceneLayer::render(void)
{
	if (!m_data || !m_visible) return;

	/* calculate visible row and column range */

	const bool  l_camera_rotated = Graphics::Viewport::Camera().rotation() != 0;
	const float l_camera_x = Graphics::Viewport::Camera().translation().x();
	const float l_camera_y = Graphics::Viewport::Camera().translation().y();

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
		col_start_cam = l_camera_x - l_hviewport_size.width();
		col_stop_cam  = l_camera_x + l_hviewport_size.width();
		row_start_cam = l_camera_y + l_hviewport_size.height();
		row_stop_cam  = l_camera_y - l_hviewport_size.height();
	}

	int col_start = static_cast<int>(floorf(((col_start_cam + m_hrsize.width()) / m_rsize.width())
	    * static_cast<float>(m_size.width())));
	int col_stop  = static_cast<int>(ceilf(((col_stop_cam + m_hrsize.width()) / m_rsize.width())
	    * static_cast<float>(m_size.width())));
	int row_start = static_cast<int>(floorf(((row_start_cam - m_hrsize.height()) / m_rsize.height())
	    * static_cast<float>(-m_size.height())));
	int row_stop  = static_cast<int>(ceilf(((row_stop_cam - m_hrsize.height()) / m_rsize.height())
	    * static_cast<float>(-m_size.height())));

	/* check limits */

	if (col_start < 0) col_start = 0;
	else if (col_start >= m_size.width()) col_start = m_size.width();
	if (col_stop < 0) col_stop = 0;
	else if (col_stop >= m_size.width()) col_stop = m_size.width();

	if (row_start < 0) row_start = 0;
	else if (row_start >= m_size.height()) row_start = m_size.height();
	if (row_stop < 0) row_stop = 0;
	else if (row_stop >= m_size.height()) row_stop = m_size.height();

	/* abort if out of view */

	if (row_start >= row_stop || col_start >= col_stop)
		return;

	/* draw tiles */

	Graphics::Color l_color(1.f, 1.f, 1.f, m_opacity);

	for (int l_r = row_start; l_r < row_stop; ++l_r) {
		const int l_roffset = l_r * m_size.width();

		for (int l_c = col_start; l_c < col_stop; ++l_c) {
			int l_tindex = m_data[l_roffset + l_c];
			if (!l_tindex)
				continue;

			/* calculate tile position */
			float l_x = static_cast<float>(l_c) * m_rtile_size.width();
			float l_y = static_cast<float>(m_size.height() - l_r) * m_rtile_size.height();

			/* offset to center of viewport */
			l_x -= m_hrsize.width();
			l_y -= m_hrsize.height();

			/* offset to bottom of tile (we draw up) */
			l_y -= m_rtile_size.height();

			int l_tioffset;
			Graphics::SharedTileset l_ts = tileset(l_tindex, &l_tioffset);
			Graphics::SharedTextureCoordinateData l_tcd;

			if (l_ts) {
				Graphics::SharedVertexData &l_svdata = m_vertexes[l_tioffset];

				Graphics::QuadMesh l_mesh(l_ts->getTextureCoordinateData(l_tindex - l_tioffset),
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
	VertexDataCache::const_iterator l_end = m_vertexes.end();

	for (l_i = m_vertexes.begin(); l_i != l_end; ++l_i)
		recalculateVertexData(l_i->first);
}

void
TilemapSceneLayer::recalculateRelativeTileSize()
{
	const Math::Size2f &l_vsize = Graphics::Viewport::Size();
	const Math::Size2i &l_wsize = Graphics::Viewport::WindowSize();

	/*
	 * map tile size from pixels to virtual coordinates.
	 * vTS (vcoord) = (TS (pixels) * vSize (vcoord)) / wSize (pixels)
	 */
	m_rtile_size[0] =
	    (static_cast<float>(m_tile_size.width()) * l_vsize.width())
	        / static_cast<float>(l_wsize.width());
	m_rtile_size[1] =
	    (static_cast<float>(m_tile_size.height()) * l_vsize.height())
	        / static_cast<float>(l_wsize.height());
	m_hrtile_size = m_rtile_size / 2.0f;

	/* calculate relative map size */
	m_rsize[0] = m_rtile_size.width()  * static_cast<float>(m_size.width());
	m_rsize[1] = m_rtile_size.height() * static_cast<float>(m_size.height());
	m_hrsize = m_rsize / 2.f;
}

void
TilemapSceneLayer::recalculateVertexData(int o)
{
	assert(m_vertexes[o] && "Invalid vertex data!");
	assert(m_tilesets[o] && "Invalid tileset!");

	/*
	 * tileset tile size should be scale of the map tile size
	 * tts (32x32) / mts (16x16) = 2x scale
	 */
	const int l_tswscale = m_tilesets[o]->tileSize().width() / m_tile_size.width();
	const int l_tshscale = m_tilesets[o]->tileSize().height() / m_tile_size.height();

	float l_vwidth  = (static_cast<float>(l_tswscale) * m_rtile_size.width());
	float l_vheight = (static_cast<float>(l_tshscale) * m_rtile_size.height());

	/* anchor to (0,0) */
	m_vertexes[o]->set(0, 0,        l_vheight);
	m_vertexes[o]->set(1, 0,        0);
	m_vertexes[o]->set(2, l_vwidth, l_vheight);
	m_vertexes[o]->set(3, l_vwidth, 0);
}

const Core::Type &
TilemapSceneLayer::Type(void)
{
	return(sType);
}

