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
      m_tile_size(24, 24),
      m_texture_data(),
      m_rmargin(0, 0),
      m_rspacing(0, 0),
      m_tile_rsize(0, 0),
      m_margin(0),
      m_spacing(0),
      m_tile_cols(0)
{
}

TilesetBase::~TilesetBase(void)
{
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
TilesetBase::setTileSize(const Math::Size2i &s)
{
	m_tile_size = s;
	reset();
}

void
TilesetBase::setMargin(int m)
{
	m_margin = m;
	reset();
}

void
TilesetBase::setSpacing(int s)
{
	m_spacing = s;
	reset();
}

SharedTextureCoordinateData
TilesetBase::getTextureCoordinateData(int i)
{
	if (!m_texture_data || !m_texture_data->isLoaded() || !m_tile_cols)
		return(SharedTextureCoordinateData());

	TextureCoordinateMap::iterator l_cached_i = m_cache.find(i);
	if (l_cached_i == m_cache.end()) {
		/* create new entry */
#define TILECOORDINATES 4
		SharedTextureCoordinateData l_data =
		    Factory::CreateTextureCoordinateData(TILECOORDINATES);

		const int l_col = i % m_tile_cols;
		const int l_row = i / m_tile_cols;

		/* calculate tile position
		 *
		 * margin + (pos * ( tile_size + spacing ))
		 *
		 */

		float l_left = m_rmargin.rheight() +
		    (static_cast<float>(l_col) *
		        (m_tile_rsize.rwidth() + m_rspacing.rwidth()));
		float l_top = m_rmargin.rwidth() +
		    (static_cast<float>(l_row) *
		        (m_tile_rsize.rheight() + m_rspacing.rheight()));

		l_data->set(0, l_left, l_top);
		l_data->set(1, l_left, l_top + m_tile_rsize.rheight());
		l_data->set(2, l_left + m_tile_rsize.rwidth(), l_top);
		l_data->set(3, l_left + m_tile_rsize.rwidth(), l_top + m_tile_rsize.rheight());

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
	/* clear current tile data and cache */
	m_tile_cols = 0;
	m_cache.clear();

	if (!m_texture_data)
		return;

	const Math::Size2i l_size = m_texture_data->size();

	/* calculate sizes relative to the texture */

	m_rmargin.rwidth() = static_cast<float>(m_margin) /
	                     static_cast<float>(l_size.rwidth());
	m_rmargin.rheight() = static_cast<float>(m_margin) /
	                      static_cast<float>(l_size.rheight());

	m_rspacing.rwidth() = static_cast<float>(m_spacing) /
	                      static_cast<float>(l_size.rwidth());
	m_rspacing.rheight() = static_cast<float>(m_spacing) /
	                       static_cast<float>(l_size.rheight());

	m_tile_rsize.rwidth() = static_cast<float>(m_tile_size.rwidth()) /
	                        static_cast<float>(l_size.rwidth());
	m_tile_rsize.rheight() = static_cast<float>(m_tile_size.rheight()) /
	                         static_cast<float>(l_size.rheight());

	/* calculate max cols */

	m_tile_cols = (static_cast<int>(l_size.rwidth()) - m_margin) /
	              (static_cast<int>(m_tile_size.rwidth()) + m_spacing);
}

