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

#include "../backend_p.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <cassert>

#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>

#include "core/global.h"
#include "core/identifier.h"
#include "core/logger.h"

#define ALSA_DEFAULT_DEVICE "default"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Audio { /****************************************** Audio Namespace */

bool
Backend::Initialize(void)
{
	return(false);
}

void
Backend::Finalize(void)
{
}

void
Backend::Tick(float delta)
{
	MMUNUSED(delta);
}

const Core::Identifier &
Backend::Implementation(void)
{
	static const Core::Identifier s_implementation("ALSA");
	return(s_implementation);
}

namespace Backend { /******************************* Audio::Backend Namespace */

struct PCM::Handle
{
	snd_pcm_hw_params_t *params;
	snd_pcm_t           *device;
	snd_pcm_uframes_t    frames;

	char  *buffer;
	size_t buffer_size;
};

PCM::Handle *
PCM::Open(uint32_t sample_rate, uint8_t bit_depth, uint8_t channels)
{
	PCM::Handle *l_handle(0);

	/*
	 * PCM format translation
	 */
	snd_pcm_format_t l_format = SND_PCM_FORMAT_UNKNOWN;
	switch(bit_depth) {
	case 8:  l_format = SND_PCM_FORMAT_S8; break;
	case 16: l_format = SND_PCM_FORMAT_S16_LE; break;
	case 24: l_format = SND_PCM_FORMAT_S24_LE; break;
	case 32: l_format = SND_PCM_FORMAT_S32_LE; break;

	default:
		MMERROR("Unknown bit depth: " << bit_depth);
		return(0);
	}

	/*
	 * Create handle
	 */
	l_handle = new Handle;

	/*
	 * Open device
	 */
	int l_rc;
	if ((l_rc = snd_pcm_open(&l_handle->device, ALSA_DEFAULT_DEVICE, SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK))) {
		MMERROR("Unable to open device: " << snd_strerror(l_rc));
		return(false);
	}

	snd_pcm_hw_params_alloca(&l_handle->params);
	snd_pcm_hw_params_any(l_handle->device, l_handle->params);
	snd_pcm_hw_params_set_access(l_handle->device, l_handle->params, SND_PCM_ACCESS_RW_INTERLEAVED);
	snd_pcm_hw_params_set_format(l_handle->device, l_handle->params, l_format);
	snd_pcm_hw_params_set_channels(l_handle->device, l_handle->params, channels);

	int l_dir = 0;
	unsigned int l_rate = sample_rate;
	snd_pcm_hw_params_set_rate_near(l_handle->device, l_handle->params, &l_rate, &l_dir);

	if ((l_rc = snd_pcm_hw_params(l_handle->device, l_handle->params))) {
		MMERROR("Unable to set hw parameters: " << snd_strerror(l_rc));
		return(false);
	}

	snd_pcm_hw_params_get_period_size_max(l_handle->params, &l_handle->frames, &l_dir);

	l_handle->buffer_size = (bit_depth/8) * channels * l_handle->frames;
	l_handle->buffer = new char[l_handle->buffer_size];

	return(l_handle);
}

void
PCM::Close(Handle *pcm_handle)
{
	assert(pcm_handle && "Tried to use invalid PCM device!");

	snd_pcm_drain(pcm_handle->device);
	snd_pcm_hw_free(pcm_handle->device);
	snd_pcm_close(pcm_handle->device);
	delete[] pcm_handle->buffer, pcm_handle->buffer = 0;
	delete pcm_handle;
}

bool
PCM::Write(Handle *pcm_handle)
{
	assert(pcm_handle && "Tried to use invalid PCM device!");

	/*
	 * Reset PCM if it reached underrun (due to pause or system slow down)
	 */
	if (SND_PCM_STATE_XRUN == snd_pcm_state(pcm_handle->device))
		snd_pcm_recover(pcm_handle->device, -EPIPE, 0);

	if (snd_pcm_writei(pcm_handle->device, pcm_handle->buffer, pcm_handle->frames) < 0)
		return(false);

	return(true);
}

bool
PCM::Pause(Handle *pcm_handle, bool state)
{
	assert(pcm_handle && "Tried to use invalid PCM device!");

	if (!snd_pcm_hw_params_can_pause(pcm_handle->params)) {
		MMERROR("Hardware doesn't support PCM pause.");
		return(false);
	}
	
	if (snd_pcm_pause(pcm_handle->device, state ? 1 : 0)) {
		MMERROR("Failed to change PCM pause state.");
		return(false);
	}

	return(true);
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

