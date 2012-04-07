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

#include "vertexdata.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include "graphics/viewport.h"

#include <SDL_video.h>

#include <cstring>

MARSHMALLOW_NAMESPACE_USE
using namespace Graphics;
using namespace SDL;

VertexData::VertexData(uint16_t c)
#define AXES 2
    : m_id()
    , m_data(new float[c * AXES]) // TODO: replace with custom allocator
    , m_count(c)
{
	memset(m_data, 0, m_count * AXES);
}

VertexData::~VertexData(void)
{
	delete[] m_data;
}

bool
VertexData::asRect(const Math::Point2 &t, SDL_Rect &r) const
{
	if (m_count != 4 /* has four sides */) {
		MMWARNING("Can't convert to rect, object has more than 4 sides.");
		return(false);
	}

	const Math::Size2f &l_vsize = Viewport::Size();
	const Math::Size2i &l_wsize = Viewport::WindowSize();
	const Math::Size2f l_hvsize = l_vsize / 2.f;

	const float l_ratio_x =  static_cast<float>(l_wsize.width())  / l_vsize.width();
	const float l_ratio_y = -static_cast<float>(l_wsize.height()) / l_vsize.height();

	r.x = Sint16(( t.x() + m_data[0] + l_hvsize.width())  * l_ratio_x);
	r.y = Sint16(( t.y() + m_data[1] - l_hvsize.height()) * l_ratio_y);
	r.w = Uint16((m_data[6] - m_data[0]) * l_ratio_x);
	r.h = Uint16((m_data[1] - m_data[7]) * l_ratio_y);

	return(true);
}

bool
VertexData::get(uint16_t i, float &x, float &y) const
{
	const uint16_t l_offset = static_cast<uint16_t>((i % m_count) * AXES);
	x = m_data[l_offset];
	y = m_data[l_offset + 1];
	return(true);
}

bool
VertexData::set(uint16_t i, float x, float y)
{
	const uint16_t l_offset = static_cast<uint16_t>((i % m_count) * AXES);
	m_data[l_offset] = x;
	m_data[l_offset + 1] = y;
	return(true);
}

const Core::Type &
VertexData::Type(void)
{
	static const Core::Type sType("Graphics::SDL::VertexData");
	return(sType);
}

