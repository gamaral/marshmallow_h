/*
 * Copyright 2012-2013 Marshmallow Engine. All rights reserved.
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

#include "game/config.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Audio { /****************************************** Audio Namespace */

bool
Backend::Initialize(void)
{
	MMDEBUG("Dummy audio backend initialized");
	return(true);
}

void
Backend::Finalize(void)
{
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
	char  *buffer;
	size_t buffer_size;
};

PCM::Handle *
PCM::Open(uint32_t sample_rate, uint8_t bit_depth, uint8_t channels)
{
	MMUNUSED(sample_rate);

	PCM::Handle *l_handle(new Handle);

	l_handle->buffer_size = (sample_rate/MARSHMALLOW_ENGINE_FRAMERATE)
	                        * (bit_depth/8) * channels;
	l_handle->buffer = new char[l_handle->buffer_size];

	MMDEBUG("Dummy PCM device opened.");

	return(l_handle);
}

void
PCM::Close(Handle *pcm_handle)
{
	assert(pcm_handle && "Tried to use invalid PCM device!");
	assert(pcm_handle->buffer && "Buffer missing from PCM handle!");

	delete[] pcm_handle->buffer, pcm_handle->buffer = 0;
	delete pcm_handle;

	MMDEBUG("Dummy PCM device closed.");
}

bool
PCM::Write(Handle *pcm_handle, size_t)
{
	assert(pcm_handle && "Tried to use invalid PCM device!");
	return(pcm_handle);
}

void
PCM::Buffer(Handle *pcm_handle, char *&buffer, size_t &bsize)
{
	assert(pcm_handle && "Tried to use invalid PCM device!");
	buffer = pcm_handle->buffer;
	bsize  = pcm_handle->buffer_size;
}

} /************************************************* Audio::Backend Namespace */
} /********************************************************** Audio Namespace */
MARSHMALLOW_NAMESPACE_END

