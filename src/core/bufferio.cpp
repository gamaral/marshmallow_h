/*
 * Copyright (c) 2011-2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
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

#include "core/bufferio.h"

/*!
 * @buffer
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <cassert>
#include <climits>
#include <cstring>

#include "core/logger.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Core { /******************************************** Core Namespace */

struct BufferIO::Private
{
	enum Flags
	{
		ReachedEOF = (1 << 0),
		FreeBuffer = (1 << 1),
		None       = 0
	};

	int            mode;
	long           cursor;
	long           size;
	int            flags;
	uint8_t       *buffer;
	const uint8_t *const_buffer;
};

BufferIO::BufferIO(void *b, size_t s)
    : PIMPL_CREATE
{
	assert(s <= LONG_MAX && "Buffer too large!");

	PIMPL->buffer = reinterpret_cast<uint8_t *>(b);
	PIMPL->const_buffer = reinterpret_cast<uint8_t *>(b);
	PIMPL->mode = ReadWrite;
	PIMPL->cursor = 0;
	PIMPL->size = long(s);
	PIMPL->flags = Private::None;
}

BufferIO::BufferIO(const void *cb, size_t s)
    : PIMPL_CREATE
{
	assert(s <= LONG_MAX && "Buffer too large!");

	PIMPL->buffer = 0;
	PIMPL->const_buffer = reinterpret_cast<const uint8_t *>(cb);
	PIMPL->mode = ReadOnly;
	PIMPL->cursor = 0;
	PIMPL->size = long(s);
	PIMPL->flags = Private::None;
}

BufferIO::BufferIO(IDataIO *source)
    : PIMPL_CREATE
{
	if (!source->isOpen()) {
		MMERROR("Source DIO is closed!");
		return;
	}

	const long l_cursor = source->tell();
	if (l_cursor == -1) {
		MMERROR("Failed to get current position for source DIO.");
		return;
	}

	if (!source->seek(0, End)) {
		MMERROR("Failed to seek to end.");
		return;
	}

	const long l_size = source->tell();

	if (l_size <= 0) {
		MMERROR("Source DIO has invalid size.");
		return;
	}

	if (!source->seek(0, Set)) {
		MMERROR("Failed to seek back to beginning.");
		return;
	}

	uint8_t *l_buffer = new uint8_t[l_size];

	if (l_size != long(source->read(l_buffer, size_t(l_size)))) {
		MMERROR("Failed to read data from source DIO.");
		return;
	}

	PIMPL->buffer = l_buffer;
	PIMPL->const_buffer = l_buffer;
	PIMPL->mode = ReadWrite;
	PIMPL->cursor = 0;
	PIMPL->size = l_size;
	PIMPL->flags = Private::FreeBuffer;

	if (!source->seek(l_cursor, Set))
		MMERROR("Failed to return source DIO to original position.");
}

BufferIO::BufferIO(const BufferIO &source)
    : PIMPL_CREATE
{
	uint8_t *l_buffer = new uint8_t[source.PIMPL->size];
	memcpy(l_buffer, source.PIMPL->const_buffer, size_t(source.PIMPL->size));

	PIMPL->buffer = l_buffer;
	PIMPL->const_buffer = l_buffer;
	PIMPL->mode = ReadWrite;
	PIMPL->cursor = 0;
	PIMPL->size = source.PIMPL->size;
	PIMPL->flags = Private::FreeBuffer;
}

BufferIO::~BufferIO(void)
{
	close();

	PIMPL_DESTROY;
}

size_t
BufferIO::size(void) const
{
	return(size_t(PIMPL->size));
}

bool
BufferIO::open(int)
{
	MMWARNING("BufferIO open was called, it will be ignored.");
	return(isOpen());
}

void
BufferIO::close(void)
{
	if (PIMPL->flags & Private::FreeBuffer)
		delete[] PIMPL->buffer;

	PIMPL->buffer = 0;
	PIMPL->const_buffer = 0;
	PIMPL->mode = Invalid;
	PIMPL->cursor = 0;
	PIMPL->size = 0;
	PIMPL->flags = 0;
}

int
BufferIO::mode(void) const
{
	return(PIMPL->mode);
}

bool
BufferIO::isOpen(void) const
{
	return(PIMPL->const_buffer && PIMPL->mode != Invalid);
}

bool
BufferIO::atEOF(void) const
{
	return(PIMPL->flags & Private::ReachedEOF);
}

size_t
BufferIO::read(void *b, size_t bs) const
{
	assert(bs <= LONG_MAX && "Buffer too large!");

	if (!PIMPL->const_buffer &&
	    (PIMPL->cursor >= 0 || PIMPL->cursor >= PIMPL->size)) return(0);

	long l_rcount =
	    PIMPL->cursor + long(bs) < PIMPL->size ?  long(bs) : PIMPL->size - PIMPL->cursor;

	memcpy(b, PIMPL->const_buffer + PIMPL->cursor, l_rcount);
	PIMPL->cursor += l_rcount;

	/* set end-of-file flag */
	if (bs > l_rcount)
		PIMPL->flags |= Private::ReachedEOF;

	return(l_rcount);
}

size_t
BufferIO::write(const void *b, size_t bs)
{
	assert(bs <= LONG_MAX && "Buffer too large!");

	if (!PIMPL->buffer && PIMPL->cursor >= 0) return(0);

	long l_wcount =
	    PIMPL->cursor + long(bs) < PIMPL->size ? long(bs) : PIMPL->size - PIMPL->cursor;

	memcpy(PIMPL->buffer + PIMPL->cursor, b, l_wcount);
	PIMPL->cursor += l_wcount;

	/* set end-of-file flag */
	if (bs > l_wcount)
		PIMPL->flags |= Private::ReachedEOF;

	return(l_wcount);
}

bool
BufferIO::seek(long o, Seek on) const
{
	long l_cursor = -1;

	switch (on) {
	case Set:
		if (o >= 0 && o <= PIMPL->size)
			l_cursor = o;
		break;
	case End:
		if (PIMPL->size + o >= 0)
			l_cursor = PIMPL->size + o;
		break;
	case Current:
		if ((PIMPL->cursor + o) >= 0 &&
		    PIMPL->cursor + o < PIMPL->size)
			l_cursor = PIMPL->cursor + o;
		break;
	default: return(false);
	}

	if (-1 == l_cursor)
		return(false);

	/* reset end-of-file flag */
	PIMPL->flags &= ~Private::ReachedEOF;

	PIMPL->cursor = l_cursor;
	return(true);
}

long
BufferIO::tell(void) const
{
	return(PIMPL->cursor);
}

} /*********************************************************** Core Namespace */
MARSHMALLOW_NAMESPACE_END

