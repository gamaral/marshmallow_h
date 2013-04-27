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

#include "audio/wavetrack.h"

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

struct WaveTrack::Private
{
	Private(const Core::IDataIO &dio);

	inline size_t read(void *buffer, size_t bsize);
	inline bool seek(long frame) const;

	const Core::IDataIO &dio;
	long     start;
	uint32_t rate;
	uint8_t  depth;
	uint8_t  channels;
	bool     valid;
};

WaveTrack::Private::Private(const Core::IDataIO &_dio)
    : dio(_dio)
    , start(0)
    , rate(0)
    , depth(0)
    , channels(0)
    , valid(false)
{
	if (!WaveTrack::Validate(dio)) {
		MMERROR("Tried to open invalid WAVE file.");
		return;
	}

	char l_ident[5] = { 0, 0, 0, 0, 0 };

	/*
	 * FMT SUBCHUNK
	 */
	if (4 != dio.read(l_ident, 4)) {
		MMDEBUG("Invalid WAVE (short read).");
		return;
	}

	/* compare wave chunk id */
	if (strcmp(l_ident, "fmt ")) {
		MMDEBUG("Invalid WAVE format (unknown chunk).");
		return;
	}

	/* read chunk size */
	uint32_t l_chunck_size = 0;
	if (4 != dio.read(&l_chunck_size, 4)) {
		MMDEBUG("Invalid WAVE (short read).");
		return;
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
		return;
	}

	/* read fmt wave chunk */
	if (l_chunck_size != dio.read(&l_fmt_subchunk, l_chunck_size)) {
		MMDEBUG("Invalid WAVE (short read).");
		return;
	}

	/* compare wave format (PCM) */
#define WAVE_PCM_FORMAT 1
	if (l_fmt_subchunk.audio_format != WAVE_PCM_FORMAT) {
		MMDEBUG("Invalid WAVE format (non-PCM).");
		return;
	}

	/*
	 * DATA SUBCHUNK
	 */
	dio.read(l_ident, 4);
	if (strcmp(l_ident, "data")) {
		MMDEBUG("Non-standard WAVE format.");
		return;
	}

	/* skip length */
	dio.seek(4, Core::DIOCurrent);

	/* populate parameters */
	rate = l_fmt_subchunk.sample_rate;
	depth = static_cast<uint8_t>(l_fmt_subchunk.bps);
	channels = static_cast<uint8_t>(l_fmt_subchunk.channels);
	start = dio.tell();

	MMDEBUG("WAVE Rate: " << rate);
	MMDEBUG("WAVE Depth: " << int(depth));
	MMDEBUG("WAVE Channels: " << int(channels));

	/* valid wave */
	valid = true;
}

size_t
WaveTrack::Private::read(void *buffer, size_t bsize)
{
	return(dio.read(buffer, bsize));
}

bool
WaveTrack::Private::seek(long frame) const
{
	return(dio.seek(start + (frame * (channels * (depth / 8))),
	    Core::DIOSet));
}

/****************************************************************** WaveCode */

WaveTrack::WaveTrack(const Core::IDataIO &dio)
    : m_p(new Private(dio))
{
}

WaveTrack::~WaveTrack(void)
{
	delete m_p, m_p = 0;
}

bool
WaveTrack::isValid(void) const
{
	return(m_p->valid && m_p->dio.isOpen());
}

uint32_t
WaveTrack::rate(void) const
{
	assert(isValid() && "Invalid Track!");
	return(m_p->rate);
}

uint8_t
WaveTrack::depth(void) const
{
	assert(isValid() && "Invalid Track!");
	return(m_p->depth);
}

uint8_t
WaveTrack::channels(void) const
{
	assert(isValid() && "Invalid Track!");
	return(m_p->channels);
}

size_t
WaveTrack::read(void *buffer, size_t bsize) const
{
	assert(isValid() && "Invalid Track!");
	return(m_p->read(buffer, bsize));
}

bool
WaveTrack::rewind(void) const
{
	assert(isValid() && "Invalid Track!");
	return(m_p->seek(0));
}

bool
WaveTrack::seek(long offset) const
{
	assert(isValid() && "Invalid Track!");
	return(m_p->seek(offset));
}

bool
WaveTrack::Validate(const Core::IDataIO &dio)
{
	/* sanity check */
	if (!dio.isOpen()) {
		MMERROR("Audio stream is closed!");
		return(false);
	}

	/*
	 * Reset location
	 */
	if (!dio.seek(0, Core::DIOSet)) {
		MMDEBUG("Invalid DataIO (failed seek).");
		return(false);
	}

	char l_ident[5] = { 0, 0, 0, 0, 0 };
	
	/* read type */
	if (4 != dio.read(l_ident, 4)) {
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
	if (!dio.seek(4, Core::DIOCurrent)) {
		MMDEBUG("Invalid RIFF (failed seek).");
		return(false);
	}

	/* read type */
	if (4 != dio.read(l_ident, 4)) {
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

