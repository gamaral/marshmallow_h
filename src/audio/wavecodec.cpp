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

#include "audio/wavecodec.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <cstring>

#include "core/logger.h"
#include "core/shared.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Audio { /****************************************** Audio Namespace */

struct WaveCodec::Private
{
	Private(void)
	    : cursor(0)
	    , start(0)
	    , rate(0)
	    , depth(0)
	    , channels(0)
	    , opened(false)
	{}

	inline bool open(const Core::SharedDataIO &_dio);
	inline void close(void);
	inline size_t read(void *buffer, size_t bsize);
	inline void reset(void);

	Core::SharedDataIO dio;
	long cursor;
	long start;
	uint32_t rate;
	uint8_t  depth;
	uint8_t  channels;
	bool opened;
};

bool
WaveCodec::Private::open(const Core::SharedDataIO &_dio)
{
	if (!WaveCodec::Validate(_dio)) {
		MMERROR("Tried to open invalid WAVE file.");
		return(false);
	}

	char l_ident[5] = { 0, 0, 0, 0, 0 };

	/*
	 * FMT SUBCHUNK
	 */
	if (4 != _dio->read(l_ident, 4)) {
		MMDEBUG("Invalid WAVE (short read).");
		return(false);
	}

	/* compare wave chunk id */
	if (strcmp(l_ident, "fmt ")) {
		MMDEBUG("Invalid WAVE format (unknown chunk).");
		return(false);
	}

	/* read chunk size */
	uint32_t l_chunck_size = 0;
	if (4 != _dio->read(&l_chunck_size, 4)) {
		MMDEBUG("Invalid WAVE (short read).");
		return(false);
	}

	/* fmt subchunk data */
	struct
	{
		uint16_t audio_format;
		uint16_t channels;       /* channels */
		uint32_t sample_rate;    /* rate */
		uint32_t byte_rate;
		uint16_t block_align;
		uint16_t bps;            /* depth */
	} l_fmt_subchunk;

	/* match subchunch size */
	if (l_chunck_size != sizeof(l_fmt_subchunk)) {
		MMDEBUG("Invalid WAVE (fmt subchunk size mismatch).");
		return(false);
	}

	/* read fmt wave chunk */
	if (l_chunck_size != _dio->read(&l_fmt_subchunk, l_chunck_size)) {
		MMDEBUG("Invalid WAVE (short read).");
		return(false);
	}

	/* compare wave format (PCM) */
#define WAVE_PCM_FORMAT 1
	if (l_fmt_subchunk.audio_format != WAVE_PCM_FORMAT) {
		MMDEBUG("Invalid WAVE format (non-PCM).");
		return(false);
	}

	/* update sound parameters */
	rate = l_fmt_subchunk.sample_rate;
	depth = static_cast<uint8_t>(l_fmt_subchunk.bps);
	channels = static_cast<uint8_t>(l_fmt_subchunk.channels);
	MMDEBUG("WAVE Rate: " << rate);
	MMDEBUG("WAVE Depth: " << int(depth));
	MMDEBUG("WAVE Channels: " << int(channels));

	/*
	 * DATA SUBCHUNK
	 */
	_dio->read(l_ident, 4);
	if (strcmp(l_ident, "data")) {
		MMDEBUG("Non-standard WAVE format.");
		return(false);
	}

	/* skip length */
	_dio->seek(4, Core::DIOCurrent);

	/* data start */
	cursor = start = _dio->tell();

	/* store data io */
	dio = _dio;

	return(opened = true);
}

void
WaveCodec::Private::close(void)
{
	if (!opened) return;

	cursor = 0;
	start = 0;
	opened = false;
}

size_t
WaveCodec::Private::read(void *buffer, size_t bsize)
{
	if (!opened) return(-1);

	dio->seek(cursor, Core::DIOStart);
	const size_t l_read = dio->read(buffer, bsize);
	cursor = dio->tell();

	return(l_read);
}

void
WaveCodec::Private::reset(void)
{
	if (!opened) return;
	cursor = start;
}


WaveCodec::WaveCodec(void)
    : m_p(new Private)
{
}

WaveCodec::~WaveCodec(void)
{
	close();
	delete m_p, m_p = 0;
}

bool
WaveCodec::open(const Core::SharedDataIO &dio)
{
	return(m_p->open(dio));
}

void
WaveCodec::close(void)
{
	m_p->close();
}

bool
WaveCodec::isOpen(void) const
{
	return(m_p->opened);
}

uint32_t
WaveCodec::rate(void) const
{
	return(m_p->rate);
}

uint8_t
WaveCodec::depth(void) const
{
	return(m_p->depth);
}

uint8_t
WaveCodec::channels(void) const
{
	return(m_p->channels);
}

size_t
WaveCodec::read(void *buffer, size_t bsize)
{
	return(m_p->read(buffer, bsize));
}

void
WaveCodec::reset(void)
{
	m_p->reset();
}

bool
WaveCodec::Validate(const Core::SharedDataIO &dio)
{
	/* sanity check */
	if (!dio->isOpen() && !dio->open(Core::DIOReadOnly)) {
		MMERROR("Failed to open audio stream.");
		return(false);
	}

	/*
	 * Reset location
	 */
	if (!dio->seek(0, Core::DIOStart)) {
		MMDEBUG("Invalid DataIO (failed seek).");
		return(false);
	}

	char l_ident[5] = { 0, 0, 0, 0, 0 };
	
	/* read type */
	if (4 != dio->read(l_ident, 4)) {
		MMDEBUG("Invalid DataIO (short read).");
		return(false);
	}
	
	/* compare chunk type */
	if (strcmp(l_ident, "RIFF")) {
		MMDEBUG("Invalid DataIO (chunk not RIFF)");
		return(false);
	}

	MMDEBUG("Detected RIFF file.");

	/* skip length */
	if (!dio->seek(4, Core::DIOCurrent)) {
		MMDEBUG("Invalid RIFF (failed seek).");
		return(false);
	}

	/* read type */
	if (4 != dio->read(l_ident, 4)) {
		MMDEBUG("Invalid RIFF (short read).");
		return(false);
	}

	/* compare type */
	if (strcmp(l_ident, "WAVE")) {
		MMDEBUG("Invalid RIFF (unknown type).");
		return(false);
	}

	MMDEBUG("Confirmed RIFF WAVE file.");

	return(true);
}

} /********************************************************** Audio Namespace */
MARSHMALLOW_NAMESPACE_END

