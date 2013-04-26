/*
 * Copyright (c) 2012-2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
 * All rights reserved.
 *
 * This file is part of Marshmallow Game Engine.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the project as a whole.
 */

#include "../backend_p.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <cassert>

#include "core/global.h"
#include "core/identifier.h"
#include "core/logger.h"
#include "core/platform.h"

#include "game/config.h"

#define WRITE_TIMEOUT_MS 8

MARSHMALLOW_NAMESPACE_BEGIN
namespace Audio { /****************************************** Audio Namespace */
namespace { /************************** Audio::Backend::<anonymous> Namespace */

	static bool s_initialized;

	static bool
	IsBackendInitialized()
	{
		return(s_initialized);
	}

} /************************************ Audio::Backend::<anonymous> Namespace */


bool
Backend::Initialize(void)
{
	MMDEBUG("Dummy audio backend initialized");
	return(s_initialized = true);
}

void
Backend::Finalize(void)
{
	s_initialized = false;
	MMDEBUG("Dummy audio backend finalized");
}

void
Backend::Tick(float delta)
{
	MMUNUSED(delta);
}

const Core::Identifier &
Backend::Implementation(void)
{
	static const Core::Identifier s_implementation("DUMMY");
	return(s_implementation);
}

namespace Backend { /******************************* Audio::Backend Namespace */

struct PCM::Handle
{
	size_t bytes_per_frame;
	size_t frames;
	MMTIME last_write;
};

PCM::Handle *
PCM::Open(uint32_t sample_rate, uint8_t bit_depth, uint8_t channels)
{
	assert(IsBackendInitialized() && "Audio backend has not initialized yet!");

	MMUNUSED(sample_rate);

	PCM::Handle *l_handle(new Handle);

	l_handle->bytes_per_frame = (bit_depth/8) * channels;
	l_handle->frames = (sample_rate/MARSHMALLOW_ENGINE_FRAMERATE);
	l_handle->last_write = NOW();

	MMDEBUG("Dummy PCM device opened.");

	return(l_handle);
}

void
PCM::Close(Handle *pcm_handle)
{
	assert(pcm_handle && "Tried to use invalid PCM device!");

	delete pcm_handle;

	MMDEBUG("Dummy PCM device closed.");
}

bool
PCM::Write(Handle *pcm_handle, const char *buffer, size_t frames)
{
	assert(IsBackendInitialized() && "Audio backend finalized!");
	assert(pcm_handle && "Tried to use invalid PCM device!");

	MMUNUSED(buffer);
	MMUNUSED(frames);

	if (NOW() - pcm_handle->last_write < WRITE_TIMEOUT_MS)
		return(false);

	MMVERBOSE("Wrote " << frames << " frames into imaginary PCM device.");
	pcm_handle->last_write = NOW();
	return(true);
}

size_t
PCM::MaxFrames(Handle *pcm_handle)
{
	assert(IsBackendInitialized() && "Audio backend finalized!");
	assert(pcm_handle && "Tried to use invalid PCM device!");

	return(pcm_handle->frames);
}

size_t
PCM::AvailableFrames(Handle *pcm_handle)
{
	assert(IsBackendInitialized() && "Audio backend finalized!");
	assert(pcm_handle && "Tried to use invalid PCM device!");

	if (NOW() - pcm_handle->last_write >= WRITE_TIMEOUT_MS)
		return(pcm_handle->frames);
	return(0);
}

} /************************************************* Audio::Backend Namespace */
} /********************************************************** Audio Namespace */
MARSHMALLOW_NAMESPACE_END

