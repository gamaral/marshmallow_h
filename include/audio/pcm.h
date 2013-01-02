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

#ifndef MARSHMALLOW_AUDIO_PCM_H
#define MARSHMALLOW_AUDIO_PCM_H 1

#include <core/iasset.h>

#include <core/global.h>
#include <core/fd.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Core { /******************************************** Core Namespace */
	struct IDataIO;
	typedef Core::Shared<IDataIO> SharedDataIO;
} /*********************************************************** Core Namespace */

namespace Audio { /****************************************** Audio Namespace */

	struct ICodec;
	typedef Core::Shared<ICodec> SharedCodec;


	/*!
	 * @brief Audio PCM
	 */
	class MARSHMALLOW_AUDIO_EXPORT
	PCM
	{
		struct Private;
		Private *m_p;

	public:
		PCM(void);
		PCM(uint32_t sample_rate, uint8_t bit_depth, uint8_t channels);
		~PCM(void);

		bool open(uint32_t sample_rate, uint8_t bit_depth, uint8_t channels);
		void close(void);
		bool isOpen(void) const;

		bool mix(char *buffer, size_t bsize);
		bool override(char *buffer, size_t bsize);

		uint32_t rate(void) const;
		uint8_t depth(void) const;
		uint8_t channels(void) const;

		uint8_t frameSize(void) const;
		size_t bufferSize(void) const;

		void tick(float delta);
	};
	typedef Core::Shared<PCM> SharedPCM;
	typedef Core::Weak<PCM> WeakPCM;

} /********************************************************** Audio Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
