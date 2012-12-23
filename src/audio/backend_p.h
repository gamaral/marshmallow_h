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

#ifndef MARSHMALLOW_AUDIO_BACKEND_P_H
#define MARSHMALLOW_AUDIO_BACKEND_P_H 1

#include "audio/backend.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Audio { /****************************************** Audio Namespace */

/*! @brief Audio Backend Interface
 *
 */
namespace Backend { /******************************* Audio::Backend Namespace */

	MARSHMALLOW_AUDIO_EXPORT
	bool Initialize(void);

	MARSHMALLOW_AUDIO_EXPORT
	void Finalize(void);

	MARSHMALLOW_AUDIO_EXPORT
	void Tick(float delta);

/*! @brief Audio Backend PCM Interface
 *
 */
namespace PCM { /****************************** Audio::Backend::PCM Namespace */

	/*!
	 * Implementation-dependant class containing backend PCM information.
	 */
	struct Handle;

	/*!
	 * Open PCM device
	 */
	Handle * Open(uint32_t sample_rate, uint8_t bit_depth, uint8_t channels);

	/*!
	 * Close PCM device
	 */
	void Close(Handle *pcm_handle);

	/*!
	 * Write to PCM device
	 */
	bool Write(Handle *pcm_handle);

	/*!
	 * Pause PCM device playback
	 */
	bool Pause(Handle *pcm_handle, bool state = true);

	/*!
	 * Get PCM buffer info
	 */
	void Buffer(Handle *pcm_handle, char *&buffer, size_t &bsize);

} /******************************************** Audio::Backend::PCM Namespace */
} /************************************************* Audio::Backend Namespace */
} /********************************************************** Audio Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
