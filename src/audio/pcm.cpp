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

#include "audio/pcm.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <cstring>

#include "core/logger.h"
#include "core/shared.h"

#include "audio/icodec.h"

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
	    , buffer(0)
	    , bmax(0)
	    , bsize(0)
	    , flushed(true)
	{
	}

	~Private(void)
	{
		close();
	}

	bool open(uint32_t sample_rate, uint8_t bit_depth, uint8_t channels);
	void close(void);
	void flush(void);

	bool mix(char *_buffer, size_t _bsize);
	bool override(char *_buffer, size_t _bsize);

	Backend::PCM::Handle *handle;
	uint32_t sample_rate;
	uint8_t  bit_depth;
	uint8_t  channels;
	uint8_t  frame_size;

	char *buffer;
	size_t bmax;
	size_t bsize;
	bool flushed;
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

	Backend::PCM::Buffer(handle, buffer, bmax);
	memset(buffer, 0, bmax);
	bsize = 0;

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

	buffer = 0;
	bsize = bmax = 0;

	Backend::PCM::Close(handle), handle = 0;
}

void
PCM::Private::flush(void)
{
	if (!handle) return;

	if (bsize == 0) {
		flushed = true;
		return;
	}

	if ((flushed = Backend::PCM::Write(handle, bsize))) {
		memset(buffer, 0, bmax);
		bsize = 0;
	}
}

bool
PCM::Private::mix(char *_buffer, size_t _bsize)
{
	assert(_bsize <= bmax && "Buffer Overrun!");

	if (!flushed)
		return(false);

	/* stretch buffer, if needed */
	if (_bsize > bsize)
		bsize = _bsize;

	if (8 == bit_depth) {
		int16_t l_mixed;

		for (unsigned int i = 0; i < _bsize; ++i) {
			l_mixed = static_cast<int16_t>(buffer[i] + _buffer[i]);

			/* CLIPPING */
#define MM_CHAR_MAX 127
			if (l_mixed > MM_CHAR_MAX)
				l_mixed = MM_CHAR_MAX;
#define MM_CHAR_MIN (-128)
			else if (l_mixed < MM_CHAR_MIN)
				l_mixed = MM_CHAR_MIN;

			buffer[i] = static_cast<char>(l_mixed);
		}
	}
	else if (16 == bit_depth) {
		int32_t l_mixed;
		int16_t *l_buffer  = reinterpret_cast<int16_t *>(buffer);
		int16_t *l_sbuffer = reinterpret_cast<int16_t *>(_buffer);

		const size_t c = _bsize / 2;
		for (unsigned int i = 0; i < c; ++i) {
			l_mixed = static_cast<int32_t>(l_buffer[i] + l_sbuffer[i]);

			/* CLIPPING */
#define MM_INT16_MAX 32767
			if (l_mixed > MM_INT16_MAX)
				l_mixed = MM_INT16_MAX;
#define MM_INT16_MIN (-32768)
			else if (l_mixed < MM_INT16_MIN)
				l_mixed = MM_INT16_MIN;

			l_buffer[i] = static_cast<int16_t>(l_mixed);
		}
	}
	else if (24 == bit_depth) {
		MMERROR("Unsupported bit depth encountered!");
		return(false);
	}
	else if (32 == bit_depth) {
		int64_t l_mixed;
		int32_t *l_buffer  = reinterpret_cast<int32_t *>(buffer);
		int32_t *l_sbuffer = reinterpret_cast<int32_t *>(_buffer);

		const size_t c = _bsize / 4;
		for (unsigned int i = 0; i < c; ++i) {
			l_mixed = static_cast<int64_t>(l_buffer[i] + l_sbuffer[i]);

			/* CLIPPING */
#define MM_INT32_MAX 2147483647
			if (l_mixed > MM_INT32_MAX)
				l_mixed = MM_INT32_MAX;
#define MM_INT32_MIN (-MM_INT32_MAX-1)
			else if (l_mixed < MM_INT32_MIN)
				l_mixed = MM_INT32_MIN;

			l_buffer[i] = static_cast<int32_t>(l_mixed);
		}
	}

	return(true);
}

bool
PCM::Private::override(char *_buffer, size_t _bsize)
{
	assert(_bsize <= bmax && "Buffer Overrun!");

	if (!flushed)
		return(false);

	bsize = _bsize;
	memcpy(buffer, _buffer, _bsize);

	return(true);
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
PCM::mix(char *_buffer, size_t _bsize)
{
	return(m_p->mix(_buffer, _bsize));
}

bool
PCM::override(char *_buffer, size_t _bsize)
{
	return(m_p->override(_buffer, _bsize));
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

uint8_t
PCM::frameSize(void) const
{
	return(m_p->frame_size);
}

size_t
PCM::bufferSize(void) const
{
	return(m_p->bmax);
}

void
PCM::tick(float)
{
	m_p->flush();
}

} /********************************************************** Audio Namespace */
MARSHMALLOW_NAMESPACE_END

