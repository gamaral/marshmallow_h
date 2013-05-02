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

#include "audio/oggtrack.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <vorbis/vorbisfile.h>

#include <cassert>
#include <cstring>

#include "core/idataio.h"
#include "core/logger.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Audio { /****************************************** Audio Namespace */
namespace { /*********************************** Audio::<anonymous> Namespace */

/*
 * The following are callbacks to bridge Ogg Vorbis with out DataIO interface.
 */

static size_t
OVRead(void *buffer, size_t byte_size, size_t byte_count, void *data)
{
	if (!data) return(0);

	const Core::IDataIO *dio = reinterpret_cast<const Core::IDataIO *>(data);

	return(dio->read(buffer, byte_size * byte_count));
}

static int
OVSeek(void *data, ogg_int64_t offset, int origin)
{
	if (!data) return(-1);

	const Core::IDataIO *dio = reinterpret_cast<const Core::IDataIO *>(data);

	/* translate origin */
	Core::DIOSeek l_seek;
	switch (origin) {
	case SEEK_SET:
		l_seek = Core::DIOSet;
		break;
	case SEEK_END:
		l_seek = Core::DIOEnd;
		break;
	case SEEK_CUR:
		l_seek = Core::DIOCurrent;
		break;
	}

	const bool l_result = dio->seek(offset, l_seek);

	return(l_result ? 0 : -1);
}

static long
OVTell(void *data)
{
	if (!data) return(-1);

	const Core::IDataIO *dio = reinterpret_cast<const Core::IDataIO *>(data);

	return(dio->tell());
}

} /********************************************* Audio::<anonymous> Namespace */

struct OggTrack::Private
{
	Private(void)
	    : dio(0)
	    , flags(0)
	    , rate(0)
	    , channels(0)
	    , valid(false)
	{}

	~Private(void);

	inline bool
	isValid(void) const
	{
		return(valid && dio && dio->isOpen());
	}

	inline void reset(void);
	inline size_t read(void *buffer, size_t bsize);
	inline bool seek(long frame);

	OggVorbis_File handle;

	Core::IDataIO *dio;
	int      flags;
	uint32_t rate;
	uint8_t  channels;
	bool     valid;
};

OggTrack::Private::~Private(void)
{
	if (flags & tfDataFree)
	    delete dio, dio = 0;

	if (!valid) return;
		
	valid = false;
	ov_clear(&handle);
}

void
OggTrack::Private::reset(void)
{
	if (!OggTrack::Validate(dio)) {
		MMERROR("Tried to open invalid Ogg file.");
		return;
	}

	/* dataio callbacks */
	ov_callbacks l_callbacks;
	l_callbacks.read_func  = OVRead;
	l_callbacks.close_func = 0; /* NO CLOSE */
	l_callbacks.seek_func  = OVSeek;
	l_callbacks.tell_func  = OVTell;

	/* setup callbacks */
	int l_rc;
	if ((l_rc = ov_open_callbacks(dio, &handle, 0, 0, l_callbacks))) {
		switch (l_rc) {
		case OV_EREAD:
			MMERROR("A read from media returned an error.");
			break;
		case OV_ENOTVORBIS:
			MMERROR("Bitstream does not contain any Vorbis data.");
			break;
		case OV_EVERSION:
			MMERROR("Vorbis version mismatch.");
			break;
		case OV_EBADHEADER:
			MMERROR("Invalid Vorbis bitstream header.");
			break;
		case OV_EFAULT:
			MMERROR("Internal logic fault; indicates a bug or heap/stack corruption.");
			break;
		}

		MMERROR("Failed to setup Ogg Vorbis <=> DataIO callbacks.");
		return;
	}

	/* get track info */
	vorbis_info *l_info = ov_info(&handle, -1);
	rate = uint32_t(l_info->rate);
	channels = uint8_t(l_info->channels);

	valid = true;
}

size_t
OggTrack::Private::read(void *buffer, size_t bsize)
{
	int l_current_section;

	size_t l_total = 0;

	long l_read = 0;
	do {
		l_read = ov_read(&handle,
		                 reinterpret_cast<char *>(buffer) + l_total,
		                 int(bsize - l_total), 0, 2, 1,
		                 &l_current_section);

		if (l_read > 0) l_total += size_t(l_read);
		else break;
	} while(bsize > l_total);

	/* check for errors */
	if (l_read < 0) {
		MMERROR("Error occurred while reading data!");
		switch (l_read) {
		case OV_HOLE:
			MMDEBUG("OV_HOLE: Data interruption.");
			break;
		case OV_EBADLINK:
			MMDEBUG("OV_EBADLINK: Invalid stream.");
			break;
		case OV_EINVAL:
			MMDEBUG("OV_EINVAL: Invalid header.");
			break;
		}
	}

	return(l_total);
}

bool
OggTrack::Private::seek(long frame)
{
	return(ov_pcm_seek(&handle, frame) == 0);
}

/****************************************************************** OggTrack */

OggTrack::OggTrack(void)
    : PIMPL_CREATE
{
}

OggTrack::~OggTrack(void)
{
	PIMPL_DESTROY;
}

void
OggTrack::setData(Core::IDataIO *d, bool f)
{
	PIMPL->dio = d;
	PIMPL->reset();
	if (f) PIMPL->flags |= tfDataFree;
	else PIMPL->flags &= ~tfDataFree;
}

Core::IDataIO *
OggTrack::data(void) const
{
	return(PIMPL->dio);
}

bool
OggTrack::isValid(void) const
{
	return(PIMPL->isValid());
}

uint32_t
OggTrack::rate(void) const
{
	assert(isValid() && "Invalid Track!");
	return(PIMPL->rate);
}

uint8_t
OggTrack::depth(void) const
{
	assert(isValid() && "Invalid Track!");
	return(16);
}

uint8_t
OggTrack::channels(void) const
{
	assert(isValid() && "Invalid Track!");
	return(PIMPL->channels);
}

size_t
OggTrack::read(void *buffer, size_t bsize) const
{
	assert(isValid() && "Invalid Track!");
	return(PIMPL->read(buffer, bsize));
}

bool
OggTrack::rewind(void) const
{
	assert(isValid() && "Invalid Track!");
	return(PIMPL->seek(0));
}

bool
OggTrack::seek(long offset) const
{
	assert(isValid() && "Invalid Track!");
	return(PIMPL->seek(offset));
}

bool
OggTrack::Validate(const Core::IDataIO *dio)
{
	/* sanity check */
	if (!dio->isOpen()) {
		MMERROR("Audio stream is closed!");
		return(false);
	}

	/*
	 * Reset location
	 */
	if (!dio->seek(0, Core::DIOSet)) {
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
	if (strcmp(l_ident, "OggS")) {
		MMDEBUG("Invalid DataIO (not Ogg container)");
		return(false);
	}

	MMDEBUG("Detected Ogg file.");

	if (!dio->seek(0, Core::DIOSet)) {
		MMDEBUG("Invalid DataIO (reset failed).");
		return(false);
	}

	return(true);
}

} /********************************************************** Audio Namespace */
MARSHMALLOW_NAMESPACE_END

