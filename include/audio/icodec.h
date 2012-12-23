/*
 * Copyright 2012 Marshmallow Engine. All rights reserved.
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

#ifndef MARSHMALLOW_AUDIO_ICODEC_H
#define MARSHMALLOW_AUDIO_ICODEC_H 1

#include <core/environment.h>
#include <core/namespace.h>
#include <core/idataio.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Audio { /****************************************** Audio Namespace */

	/*!
	 * @brief Audio Codec Interface
	 */
	struct MARSHMALLOW_AUDIO_EXPORT
	ICodec
	{
		virtual ~ICodec(void) {};

		virtual bool open(const Core::SharedDataIO &dio) = 0;
		virtual void close(void) = 0;

		virtual bool isOpen(void) const = 0;

		virtual uint32_t rate(void) const = 0;
		virtual uint16_t channels(void) const = 0;
		virtual uint16_t depth(void) const = 0;

		virtual size_t read(void *buffer, size_t bsize) = 0;

		virtual void reset(void) = 0;
	};
	typedef Core::Shared<ICodec> SharedCodec;
	typedef Core::Weak<ICodec> WeakCodec;

} /********************************************************** Audio Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
