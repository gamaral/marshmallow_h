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
    : m_cache()
    , m_name()
    , m_size(64, 64)
    , m_texture_data()
    , m_tile_size(16, 16)
    , m_adjust_col(0)
    , m_adjust_row(0)
    , m_offset_col(0)
    , m_offset_row(0)
{
}

TilesetBase::~TilesetBase(void)
{
	m_texture_data.clear();

	/* reset will clear out cache data and return */
	reset();
}

void
TilesetBase::setName(const Core::Identifier &n)
{
    m_name = n;
}

void
TilesetBase::setSize(const Math::Size2i &s)
{
	m_size = s;
	reset();
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

SharedTextureCoordinateData
TilesetBase::getTextureCoordinateData(int i)
{
	if (!m_texture_data || !m_texture_data->isLoaded())
		return(SharedTextureCoordinateData());

	TextureCoordinateMap::iterator l_cached_i = m_cache.find(i);
	if (l_cached_i == m_cache.end()) {
		/* create new entry */
#define TILECOORDINATES 4
		SharedTextureCoordinateData l_data =
		    Factory::CreateTextureCoordinateData(TILECOORDINATES);

		/* calculate row and column */

		const int l_row = i / m_size.rwidth();
		const int l_col = i % m_size.rwidth();

		
		const float &l_left  = m_offset_col[l_col] + m_adjust_col;
		const float &l_top   = m_offset_row[l_row] + m_adjust_row;
		const float l_right  = m_offset_col[l_col + 1] - m_adjust_col;
		const float l_bottom = m_offset_row[l_row + 1] - m_adjust_row;

		l_data->set(0, l_left,  l_top);
		l_data->set(1, l_left,  l_bottom);
		l_data->set(2, l_right, l_top);
		l_data->set(3, l_right, l_bottom);

		m_cache[i] = l_data;
		return(l_data);
	}

	return(m_cache[i]);
}

bool
TilesetBase::serialize(TinyXML::TiXmlElement &n) const
{
	UNUSED(n);

	/* TODO: Implement */

	return(false);
}

bool
TilesetBase::deserialize(TinyXML::TiXmlElement &n)
{
	UNUSED(n);

	/* TODO: Implement */

	return(false);
}

void
TilesetBase::reset(void)
{
	/* clear current tile cache */

	delete[] m_offset_col;
	delete[] m_offset_row;

	m_adjust_col = 0;
	m_adjust_row = 0;
	m_offset_col = 0;
	m_offset_row = 0;
	m_cache.clear();

	if (!m_texture_data)
		return;

	/* calculate tile size */

	m_tile_size.rwidth()  = m_texture_data->size().rwidth()  / m_size.rwidth();
	m_tile_size.rheight() = m_texture_data->size().rheight() / m_size.rheight();

	/*
	 * we generate the tile offsets, the extra offset (+1) acts as the limit
	 * for the last row/column.
	 *
	 * left/top = offset, right/bottom = (offset+1).
	 */

	m_adjust_col = .01f / static_cast<float>(m_size.rwidth());
	m_offset_col = new float[m_size.rwidth() + 1];
	for (int i = m_size.rwidth(); i >= 0; --i)
		m_offset_col[i] =
		    static_cast<float>(i) / static_cast<float>(m_size.rwidth());

	m_adjust_row = .01f / static_cast<float>(m_size.rheight());
	m_offset_row = new float[m_size.rheight() + 1];
	for (int i = m_size.rheight(); i >= 0; --i)
		m_offset_row[i] =
		    static_cast<float>(i) / static_cast<float>(m_size.rheight());
}

