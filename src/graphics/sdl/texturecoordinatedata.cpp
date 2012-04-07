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

#include "texturecoordinatedata.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include <SDL_video.h>

#include <cstring>

MARSHMALLOW_NAMESPACE_USE
using namespace Graphics;
using namespace SDL;

TextureCoordinateData::TextureCoordinateData(uint16_t c)
#define AXES 2
    : m_id()
    , m_data(new float[c * AXES]) // TODO: replace with custom allocator
    , m_count(c)
{
	memset(m_data, 0, m_count * AXES);
}

TextureCoordinateData::~TextureCoordinateData(void)
{
	delete[] m_data;
}

bool
TextureCoordinateData::asRect(int w, int h, SDL_Rect &r) const
{
	if (m_count != 4 /* has four sides */) {
		MMWARNING("Can't convert to rect, object has more than 4 sides.");
		return(false);
	}

	r.x = Sint16(static_cast<float>(w)  * m_data[0]);
	r.y = Sint16(static_cast<float>(h)  * m_data[1]);
	r.w = Uint16((static_cast<float>(w) * m_data[6]) - r.x);
	r.h = Uint16((static_cast<float>(h) * m_data[7]) - r.y);

	if (r.x < 0 || r.y < 0 || r.w > w || r.h > h) {
		MMERROR("Invalid texture coordinate data encountered.");
		return(false);
	}

	return(true);
}

bool
TextureCoordinateData::get(uint16_t i, float &u, float &v) const
{
	const uint16_t l_offset = static_cast<uint16_t>((i % m_count) * AXES);
	u = m_data[l_offset];
	v = m_data[l_offset + 1];
	return(true);
}

bool
TextureCoordinateData::set(uint16_t i, float u, float v)
{
	const uint16_t l_offset = static_cast<uint16_t>((i % m_count) * AXES);
	m_data[l_offset] = u;
	m_data[l_offset + 1] = v;
	return(true);
}

const Core::Type &
TextureCoordinateData::Type(void)
{
	static const Core::Type sType("Graphics::SDL::TextureCoordinateData");
	return(sType);
}

