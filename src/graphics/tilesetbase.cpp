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

#include "graphics/tilesetbase.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"
#include "math/size2.h"
#include "graphics/factory.h"
#include "graphics/itexturecoordinatedata.h"
#include "graphics/itexturedata.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Graphics;

TilesetBase::TilesetBase()
    : m_cache(),
      m_tile_size(32, 32),
      m_texture_data(),
      m_tile_count(0)
{
}

TilesetBase::~TilesetBase(void)
{
}

void
TilesetBase::reset(void)
{
	/* clear current tile count and cache */
	m_tile_count = 0;
	m_cache.clear();

	if (!m_texture_data)
		return;

	const Math::Size2 l_size = m_texture_data->size();

	m_tile_count = static_cast<int>
	    ((l_size.rwidth()  / m_tile_size.rwidth()) *
	     (l_size.rheight() / m_tile_size.rheight()));
}

void
TilesetBase::setTextureData(const SharedTextureData &ts)
{
	if (ts == m_texture_data)
		return;

	/* replace texture */
	m_texture_data = ts;

	reset();
}

void
TilesetBase::setTileSize(const Math::Size2 &s)
{
	m_tile_size = s;
	reset();
}

SharedTextureCoordinateData
TilesetBase::getCoordinate(int t)
{
	if (!m_texture_data)
		return(SharedTextureCoordinateData());

	int l_tile = t % m_tile_count;

	TextureCoordinateMap::iterator l_cached_i = m_cache.find(l_tile);
	if (l_cached_i == m_cache.end()) {
		/* create new entry */
#define TILECOORDINATES 4
		SharedTextureCoordinateData l_data =
		    Factory::CreateTextureCoordinateData(TILECOORDINATES);

		const Math::Size2 & l_txsize = m_texture_data->size();

		const int l_cols = static_cast<int>(l_txsize.rwidth() / m_tile_size.rwidth());

		const int l_col = l_tile % l_cols;
		const int l_row = l_tile / l_cols;

		const float l_tw = m_tile_size.rwidth() / l_txsize.rwidth();
		const float l_th = m_tile_size.rheight() / l_txsize.rheight();

		float l_left = static_cast<float>(l_col) * l_tw;
		float l_top = static_cast<float>(l_row) * l_th;

		l_data->set(0, l_top,        l_left);
		l_data->set(1, l_top + l_th, l_left);
		l_data->set(2, l_top,        l_left + l_tw);
		l_data->set(3, l_top + l_th, l_left + l_tw);

		m_cache[l_tile] = l_data;
		return(l_data);
	}

	return(m_cache[l_tile]);
}

