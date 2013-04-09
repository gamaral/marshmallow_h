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

#include "game/config.h"

#define ALSA_DEFAULT_DEVICE "default"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Audio { /****************************************** Audio Namespace */

bool
Backend::Initialize(void)
{
	return(true);
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
	snd_pcm_t           *device;
	snd_pcm_uframes_t    frames;

	char  *buffer;
	size_t buffer_size;

	uint16_t bytes_per_frame;
	uint8_t  bit_depth;
	uint8_t  channels;
};

PCM::Handle *
PCM::Open(uint32_t sample_rate, uint8_t bit_depth, uint8_t channels)
{
	PCM::Handle l_handle;
	snd_pcm_hw_params_t *l_params(0);

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
	 * Open device
	 */
	int l_rc;
	if ((l_rc = snd_pcm_open(&l_handle.device, ALSA_DEFAULT_DEVICE, SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK))) {
		MMERROR("Unable to open device: " << snd_strerror(l_rc));
		return(0);
	}

	snd_pcm_hw_params_malloc(&l_params);
	snd_pcm_hw_params_any(l_handle.device, l_params);
	snd_pcm_hw_params_set_access(l_handle.device, l_params, SND_PCM_ACCESS_RW_INTERLEAVED);
	snd_pcm_hw_params_set_channels(l_handle.device, l_params, channels);
	snd_pcm_hw_params_set_format(l_handle.device, l_params, l_format);
	snd_pcm_hw_params_set_rate_resample(l_handle.device, l_params, 1);

	unsigned int l_periods = MARSHMALLOW_ENGINE_FRAMERATE/10;
	snd_pcm_hw_params_set_periods_near(l_handle.device, l_params, &l_periods, 0);

	unsigned int l_rate = sample_rate;
	snd_pcm_hw_params_set_rate_near(l_handle.device, l_params, &l_rate, 0);
	if (l_rate != sample_rate) {
		MMERROR("Sample rate mismatch! Asked for " << sample_rate << " and got " << l_rate);
		return(0);
	}

	if ((l_rc = snd_pcm_hw_params(l_handle.device, l_params))) {
		MMERROR("Unable to set hw parameters: " << snd_strerror(l_rc));
		snd_pcm_hw_params_free(l_params);
		return(0);
	}

	snd_pcm_hw_params_get_period_size_max(l_params, &l_handle.frames, 0);
	snd_pcm_hw_params_free(l_params);
	l_params = 0;

	l_handle.channels = channels;
	l_handle.bit_depth = bit_depth;
	l_handle.bytes_per_frame = channels * (bit_depth/8);
	l_handle.buffer_size = l_handle.frames * l_handle.bytes_per_frame;
	l_handle.buffer = new char[l_handle.buffer_size];

	snd_config_update_free_global();

	MMDEBUG("ALSA PCM device opened.");

	snd_pcm_start(l_handle.device);

	return(new Handle(l_handle));
}

void
PCM::Close(Handle *pcm_handle)
{
	assert(pcm_handle && "Tried to use invalid PCM device!");
	assert(pcm_handle->device && "Device missing from PCM handle!");
	assert(pcm_handle->buffer && "Buffer missing from PCM handle!");

	snd_pcm_drain(pcm_handle->device);
	snd_pcm_hw_free(pcm_handle->device);
	snd_pcm_close(pcm_handle->device);
	delete[] pcm_handle->buffer, pcm_handle->buffer = 0;
	delete pcm_handle;

	MMDEBUG("ALSA PCM device closed.");
}

bool
PCM::Write(Handle *pcm_handle, size_t bsize)
{
	assert(pcm_handle && "Tried to use invalid PCM device!");

	snd_pcm_sframes_t l_available;
	snd_pcm_sframes_t l_written;
	snd_pcm_uframes_t l_frames;

	l_available = snd_pcm_avail_update(pcm_handle->device);
	if (l_available < 0) {
		MMERROR("Failed to get available samples. " << snd_strerror(int(l_available)));
		snd_pcm_recover(pcm_handle->device, -ESTRPIPE, 1);
		return(false);
	}

	l_frames = bsize / pcm_handle->bytes_per_frame;

	/*
	 * Skip if there isn't enough space in buffer for data.
	 */
	if (l_available < snd_pcm_sframes_t(l_frames))
		return(false);

	while (l_frames > 0) {
		l_written = snd_pcm_writei(pcm_handle->device, pcm_handle->buffer, l_frames);
		switch (l_written) {
		case -EAGAIN: continue;

		case -ESTRPIPE:
		case -EPIPE:
		case -EINTR:
			if (snd_pcm_recover(pcm_handle->device, int(l_written), 1))
				return(false);
		break;

		default:
			if (l_written >= 0 && snd_pcm_uframes_t(l_written) <= l_frames)
				l_frames -= snd_pcm_uframes_t(l_written);
			else {  snd_pcm_prepare(pcm_handle->device);
				return(false);
			}
		}
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

