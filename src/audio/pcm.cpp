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

#include "audio/pcm.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "backend_p.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Audio { /****************************************** Audio Namespace */

struct PCM::Private
{
	Private(void)
	    : handle(0)
	    , sample_rate(0)
	    , bit_depth(0)
	    , channels(0)
	    , frame_size(0)
	    , frames_max(0)
	{
	}

	~Private(void)
	{
		close();
	}

	bool open(uint32_t sample_rate, uint8_t bit_depth, uint8_t channels);
	void close(void);

	Backend::PCM::Handle *handle;
	uint32_t sample_rate;
	uint8_t bit_depth;
	uint8_t channels;
	uint8_t frame_size;
	size_t frames_max;
};

bool
PCM::Private::open(uint32_t _sample_rate, uint8_t _bit_depth, uint8_t _channels)
{
	close();

	handle = Backend::PCM::Open(_sample_rate, _bit_depth, _channels);
	if (!handle) return(false);

	sample_rate = _sample_rate;
	bit_depth   = _bit_depth;
	channels    = _channels;
	frame_size  = static_cast<uint8_t>((_bit_depth/8) * _channels);
	frames_max  = Backend::PCM::MaxFrames(handle);

	return(true);
}

void
PCM::Private::close(void)
{
	if (!handle) return;

	sample_rate = 0;
	bit_depth   = 0;
	channels    = 0;
	frame_size  = 0;
	frames_max  = 0;

	Backend::PCM::Close(handle), handle = 0;
}

/********************************************************************* PCM */

PCM::PCM(void)
    : m_p(new Private)
{
}

PCM::PCM(uint32_t _sample_rate, uint8_t _bit_depth, uint8_t _channels)
    : m_p(new Private)
{
	m_p->open(_sample_rate, _bit_depth, _channels);
}

PCM::~PCM(void)
{
	delete m_p, m_p = 0;
}

bool
PCM::open(uint32_t _sample_rate, uint8_t _bit_depth, uint8_t _channels)
{
	return(m_p->open(_sample_rate, _bit_depth, _channels));
}

void
PCM::close(void)
{
	m_p->close();
}

bool
PCM::isOpen(void) const
{
	return(m_p->handle);
}

bool
PCM::write(const char *buffer, size_t bsize)
{
	return(Backend::PCM::Write(m_p->handle, buffer, bsize));
}

size_t
PCM::framesAvailable(void) const
{
	return(Backend::PCM::AvailableFrames(m_p->handle));
}

size_t
PCM::framesMax(void) const
{
	return(m_p->frames_max);
}

uint8_t
PCM::frameSize(void) const
{
	return(m_p->frame_size);
}

uint32_t
PCM::rate(void) const
{
	return(m_p->sample_rate);
}

uint8_t
PCM::depth(void) const
{
	return(m_p->bit_depth);
}

uint8_t
PCM::channels(void) const
{
	return(m_p->channels);
}

} /********************************************************** Audio Namespace */
MARSHMALLOW_NAMESPACE_END

