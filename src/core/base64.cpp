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

#include "core/base64.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Core;

/******************************************************************************/

namespace
{

	static const char s_encoder64[] =
	    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	static const char s_decoder64[] = {
	    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	    0 , 0 , 0 , 62, 0 , 0 , 0 , 63, 52, 53,
	    54, 55, 56, 57, 58, 59, 60, 61, 0 , 0 ,
	    0 , 0 , 0 , 0 , 0 , 0 , 1 , 2 , 3 , 4 ,
	    5 , 6 , 7 , 8 , 9 , 10, 11, 12, 13, 14,
	    15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
	    25, 0 , 0 , 0 , 0 , 0 , 0 , 26, 27, 28,
	    29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
	    39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
	    49, 50, 51
	};

} // namespace

/******************************************************************************/

size_t
Base64::Decode(const char *in, size_t in_size, char **out)
{
	/* check in buffer */
	if (in_size % 4) {
		MMWARNING1("Base64 decode in buffer incomplete.");
		return(0);
	}

	/* calculate out data size */
	size_t l_out_size = ((in_size * 3) / 4) + 1 /* null-term */;
	if (in[in_size - 2] == '=')
	    l_out_size -= 2;
	else if (in[in_size - 1] == '=')
	    l_out_size -= 1;

	/* TODO(gamaral) replace with double buffered allocator */
	*out = new char[l_out_size];

	char *l_out = *out;
	char *l_out_max = *out + l_out_size - 1 /* null-term */;

	for (size_t i = 0; i < in_size; i = i + 4) {
		l_out[0] = static_cast<char>
		    ( s_decoder64[static_cast<int>(in[i])]   << 2
		    | s_decoder64[static_cast<int>(in[i+1])] >> 4);

		if (++l_out >= l_out_max) break;
		*l_out = static_cast<char>
		    ( s_decoder64[static_cast<int>(in[i+1])] << 4
		    | s_decoder64[static_cast<int>(in[i+2])] >> 2);

		if (++l_out >= l_out_max) break;
		*l_out = static_cast<char>
		    ( s_decoder64[static_cast<int>(in[i+2])] << 6
		    | s_decoder64[static_cast<int>(in[i+3])]);

		++l_out;
	}

	/* null-term */
	*l_out_max = '\0';

	return(l_out_size);
}

size_t
Base64::Encode(const char *in, size_t in_size, char **out)
{
	const size_t l_out_size = ((in_size * 4) / 3) + (in_size % 3 ? 3 : 0);

	/* TODO(gamaral) replace with double buffered allocator */
	*out = new char[l_out_size + 1 /* null-term */];

	char *l_out = *out;

	for (size_t i = 0; i < in_size; i = i + 3) {
		const size_t l_pad = (i + 3 < in_size ? 0 : (i + 3) - in_size);

		l_out[0] = static_cast<char>
		    (s_encoder64[in[i] >> 2]);
		l_out[1] = static_cast<char>
		    (s_encoder64[((in[i] & 0x03) << 4) | ((in[i+1] & 0xF0) >> 4)]);
		l_out[2] = (2 == l_pad ? '=' : static_cast<char>
		    (s_encoder64[((in[i+1] & 0x0F) << 2) | ((in[i+2] & 0xC0) >> 6)]));
		l_out[3] = (1 <= l_pad ? '=' : static_cast<char>
		    (s_encoder64[in[i+2] & 0x3F]));
		l_out += 4;
	}

	/* null-term */
	*l_out = '\0';

	return(l_out_size);
}

