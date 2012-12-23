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

#include "audio/track.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>

#include "core/logger.h"
#include "core/shared.h"

#include "audio/icodec.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Audio { /****************************************** Audio Namespace */

struct Track::Private
{
	Private(const SharedCodec &_codec)
	    : codec(_codec)
	    , frames(0)
	    , size(0)
	    , buffer(0)
	    , params(0)
	    , handle(0)
	    , iterations(0)
	    , skip_decode(false)
	{
		open();
	}

	~Private(void)
	{
		close();
	}

	bool open(void);
	void close(void);

	bool play(int iterations);
	void stop(bool _force);

	void update(float delta);

	SharedCodec codec;
	snd_pcm_uframes_t frames;
	size_t size;
	char *buffer;
	snd_pcm_hw_params_t *params;
	snd_pcm_t *handle;
	int iterations;
	bool skip_decode;
};

bool
Track::Private::open(void)
{
	int l_rc;

	if (!codec || !codec->isOpen()) {
		MMERROR("Invalid codec assigned to audio player.");
		return(false);
	}

	if ((l_rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK))) {
		MMERROR("Unable to open device: " << snd_strerror(l_rc));
		return(false);
	}

	int l_dir = 0;
	unsigned int l_rate = codec->rate();

	snd_pcm_hw_params_alloca(&params);
	snd_pcm_hw_params_any(handle, params);
	snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

	if (codec->depth() == 8) {
		snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S8);
		size = codec->channels();
	}
	else if (codec->depth() == 16) {
		snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
		size = 2 * codec->channels();
	}

	snd_pcm_hw_params_set_channels(handle, params, codec->channels());
	snd_pcm_hw_params_set_rate_near(handle, params, &l_rate, &l_dir);

	if ((l_rc = snd_pcm_hw_params(handle, params))) {
		MMERROR("Unable to set hw parameters: " << snd_strerror(l_rc));
		return(false);
	}

	snd_pcm_hw_params_get_period_size_max(params, &frames, &l_dir);

	size *= frames;
	buffer = new char[size];

	skip_decode = false;

	return(true);
}

void
Track::Private::close(void)
{
	snd_pcm_drain(handle);
	snd_pcm_hw_free(handle);
	snd_pcm_close(handle);
	delete [] buffer, buffer = 0;
}

bool
Track::Private::play(int _iterations)
{
	iterations = _iterations;
	codec->reset();
	snd_pcm_prepare(handle);
	update(0);

	return(iterations != 0);
}

void
Track::Private::stop(bool _force)
{
	iterations = _force ? 0 : 1;
}

void
Track::Private::update(float)
{
	if (!iterations) return;

	if (!skip_decode) {
		memset(buffer, 0, size);
		size_t l_read = codec->read(buffer, size);

		/*
		 * Reached the end (loop)
		 */
		if (l_read < size) {
			if (iterations == -1 || --iterations) {
				codec->reset();
				l_read += codec->read(buffer + l_read, size - l_read);
			}
		}
	}
	else skip_decode = false;

	int l_rc;
	if ((l_rc = snd_pcm_writei(handle, buffer, frames) < 0)) {
		switch(l_rc) {
		case -EPIPE:
			MMERROR("Underflow occured!");
			snd_pcm_prepare(handle);
			break;
		}
		skip_decode = true;
	}
}

/********************************************************************* Track */

Track::Track(const Audio::SharedCodec &_codec)
    : m_p(new Private(_codec))
{
}

Track::~Track(void)
{
	delete m_p, m_p = 0;
}

bool
Track::play(int _iterations)
{
	return(m_p->play(_iterations));
}

void
Track::stop(bool _force)
{
	m_p->stop(_force);
}

bool
Track::isPlaying(void) const
{
	return(m_p->iterations);
}

void
Track::tick(float delta)
{
	m_p->update(delta);
}

bool
Track::isValid(void) const
{
	return(m_p->codec && m_p->codec->isOpen());
}

} /********************************************************** Audio Namespace */
MARSHMALLOW_NAMESPACE_END

