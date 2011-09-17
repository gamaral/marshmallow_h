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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef GRAPHICS_TGA_H
#define GRAPHICS_TGA_H 1

MARSHMALLOW_NAMESPACE_BEGIN

namespace Graphics
{

	typedef TilesetBase Tileset;

}

MARSHMALLOW_NAMESPACE_END

#include <iostream>

namespace
{
	struct pixel_t { UINT8 r,g,b,a; };

	void
	ParseChunk(pixel_t *pixel, UINT8 *p, int size)
	{
		if (size == 4) {
			pixel->r = p[2];
			pixel->g = p[1];
			pixel->b = p[0];
			pixel->a = p[3];
		} else if (size == 3) {
			pixel->r = p[2];
			pixel->g = p[1];
			pixel->b = p[0];
			pixel->a = 0;
		} else if (size == 2) {
			pixel->r = ( p[1] & 0x7c) << 1;
			pixel->g = ((p[1] & 0x03) << 6) | ((p[0] & 0xe0) >> 2);
			pixel->b = ( p[0] & 0x1f) << 3;
			pixel->a = ( p[1] & 0x80);
		}
	}
}

bool
tga_data_load(const char *fn, UINT8 *d, Math::Sizei2 *s, UINT8 **b)
{
	assert(d && s && b && "Invalid output variables supplied");

	UINT16 l_size[2];
	std::ifstream l_inf(fn, ios_base::in | ios_base::binary);
	if (!l_inf.is_open()) {
		MMERROR("Failed to open TGA file (%s).", fn);
		return(false);
	}

	int md, t;

	/* read width, height and depth */
	l_inf.seekg(12, ios_base::beg)
	l_inf.read(&l_size, 4);
	l_inf.read(&d, 1);
	l_inf.ignore();
}

