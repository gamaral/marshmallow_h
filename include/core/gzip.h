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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef MARSHMALLOW_CORE_GZIP_H
#define MARSHMALLOW_CORE_GZIP_H 1

#include <core/environment.h>
#include <core/namespace.h>

MARSHMALLOW_NAMESPACE_BEGIN

namespace Core
{
	/*! @brief Core Gzip Interface */
	namespace Gzip
	{
		enum CompressionLevel
		{
			     NoCompression = 0,
			   FastCompression = 1,
			   BestCompression = 9,
			DefaultCompression = (-1)
			
		};

		/*! @brief Gzip inflate
		 *  @param in In buffer
		 *  @param in_size In buffer size
		 *  @param out_size Out buffer max size estimate
		 *  @param out Out buffer pointer
		 *
		 *  Inflate a gzip compressed buffer, out buffer allocation is
		 *  handled by the function.
		 *
		 *  Deallocation will be automatic by the double buffered
		 *  allocator. (not yet implemented)
		 *
		 *  @return Actual out buffer size
		 */
		MARSHMALLOW_CORE_EXPORT
		size_t Inflate(const char *in, size_t in_size, size_t out_size, char **out);

		/*! @brief Gzip deflate
		 *  @param in In buffer
		 *  @param in_size In buffer size
		 *  @param out Out buffer pointer
		 *  @param level Compression level
		 *
		 *  Deflate a buffer using gzip compression, out buffer
		 *  allocation is handled by the function.
		 *
		 *  Deallocation will be automatic by the double buffered
		 *  allocator. (not yet implemented)
		 *
		 *  @return Actual Out buffer size
		 */
		MARSHMALLOW_CORE_EXPORT
		size_t Deflate(const char *in, size_t in_size, char **out, int level = DefaultCompression);
	}
}

MARSHMALLOW_NAMESPACE_END

#endif
