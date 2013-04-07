/*
 * Copyright 2011-2013 Marshmallow Engine. All rights reserved.
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

#include "core/gzip.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include <zlib.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Core { /******************************************** Core Namespace */

size_t
Gzip::Inflate(const char *in, size_t in_size, size_t out_size, char **out)
{
	z_stream l_stream;
	*out = new char[out_size];

	l_stream.next_in  = reinterpret_cast<Bytef *>(const_cast<char *>(in));
	l_stream.avail_in = static_cast<uInt>(in_size);

	l_stream.next_out  = reinterpret_cast<Bytef *>(*out);
	l_stream.avail_out = static_cast<uInt>(out_size);

	l_stream.zalloc = static_cast<alloc_func>(0);
	l_stream.zfree  = static_cast<free_func>(0);

	if (inflateInit2(&l_stream, 16) != Z_OK) {
		MMWARNING("Failed to initialize inflate.");
		return(0);
	}

	if (inflate(&l_stream, Z_FINISH) != Z_STREAM_END) {
		inflateEnd(&l_stream);
		MMWARNING("Failed during inflation.");
		return(0);
	}

	out_size = l_stream.total_out;
	inflateEnd(&l_stream);

	return(out_size);
}

size_t
Gzip::Deflate(const char *in, size_t in_size, char **out, int l)
{
	z_stream l_stream;
	uLongf l_out_size = compressBound(in_size);

	*out = new char[l_out_size];

	l_stream.next_in  = reinterpret_cast<Bytef *>(const_cast<char *>(in));
	l_stream.avail_in = static_cast<uInt>(in_size);

	l_stream.next_out  = reinterpret_cast<Bytef *>(*out);
	l_stream.avail_out = static_cast<uInt>(l_out_size);

	l_stream.zalloc = static_cast<alloc_func>(0);
	l_stream.zfree  = static_cast<free_func>(0);
	l_stream.opaque = static_cast<voidpf>(0);

	if (deflateInit2(&l_stream, l, Z_DEFLATED, 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
		MMWARNING("Failed to initialize deflate.");
		return(0);
	}

	if (deflate(&l_stream, Z_FINISH) != Z_STREAM_END) {
		deflateEnd(&l_stream);
		MMWARNING("Failed during deflation.");
		return(0);
	}

	l_out_size = l_stream.total_out;
	deflateEnd(&l_stream);

	return(l_out_size);
}

} /*********************************************************** Core Namespace */
MARSHMALLOW_NAMESPACE_END

