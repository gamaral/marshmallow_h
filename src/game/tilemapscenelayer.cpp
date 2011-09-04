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

#include "graphics/factory.h"
#include "graphics/painter.h"
#include "graphics/quadmesh.h"
#include "graphics/viewport.h"
#include "math/point2.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Game;

const Core::Type TilemapSceneLayer::sType("Game::TilemapSceneLayer");

TilemapSceneLayer::TilemapSceneLayer(const Core::Identifier &i, IScene &s)
    : SceneLayerBase(i, s),
      m_data(0),
      m_tile_size(24, 24),
      m_size(100, 100),
      m_opacity(1.0f),
      m_visible(true)
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

	Graphics::Color l_color(1.f, 1.f, 1.f, m_opacity);

	for (int l_r = 0; l_r < m_size.rheight(); ++l_r) {
		const int l_roffset = l_r * m_size.rwidth();

		for (int l_c = 0; l_c < m_size.rwidth(); ++l_c) {
			int l_tindex = m_data[l_roffset + l_c];
			int l_tioffset;

			if (!l_tindex)
				continue;

			Graphics::SharedTileset l_ts = tileset(l_tindex, &l_tioffset);
			Graphics::SharedTextureCoordinateData l_tcd;

			if (l_ts)
				l_tcd = l_ts->getTextureCoordinateData(l_tindex - l_tioffset);

			if (l_tcd) {
				Graphics::SharedVertexData l_svdata = m_vertexes[l_tioffset];
				float l_thw, l_thh;

				Graphics::QuadMesh l_mesh(l_tcd, l_ts->textureData(), l_svdata);
				l_mesh.setColor(l_color);
				l_svdata->get(2, l_thw, l_thh);

				const Math::Size2f &l_vsize = Graphics::Viewport::Size();
				const float l_x = (static_cast<float>(l_c) * m_rtile_size.rwidth()) + l_thw - (l_vsize.width() / 2.0f);
				const float l_y = (l_vsize.height() - (static_cast<float>(l_r + 1) * m_rtile_size.rheight()) - l_thh - (l_vsize.height() / 2.0f));
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
	m_rtile_size.rwidth() = l_vsize.rwidth() / static_cast<float>(m_size.rwidth());
	m_rtile_size.rheight() = l_vsize.rheight() / static_cast<float>(m_size.rheight());
	m_hrtile_size = m_rtile_size / 2.0f;
}

void
TilemapSceneLayer::recalculateVertexData(int o)
{
	assert(m_vertexes[o] && "Invalid vertex data!");
	assert(m_tilesets[o] && "Invalid tileset!");

	const Math::Size2f &l_vsize = Graphics::Viewport::Size();
	float l_hw = ((static_cast<float>(m_tilesets[o]->tileSize().rwidth()) * l_vsize.rwidth()) /
	              (static_cast<float>(m_tile_size.rwidth()) * static_cast<float>(m_size.rwidth()))) / 2.0f;
	float l_hh = ((static_cast<float>(m_tilesets[o]->tileSize().rheight()) * l_vsize.rheight()) /
	              (static_cast<float>(m_tile_size.rheight()) * static_cast<float>(m_size.rheight()))) / 2.0f;
	m_vertexes[o]->set(0, -l_hw,  l_hh);
	m_vertexes[o]->set(1, -l_hw, -l_hh);
	m_vertexes[o]->set(2,  l_hw,  l_hh);
	m_vertexes[o]->set(3,  l_hw, -l_hh);
}

const Core::Type &
TilemapSceneLayer::Type(void)
{
	return(sType);
}

