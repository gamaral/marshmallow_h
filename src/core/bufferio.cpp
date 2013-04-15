/*
 * Copyright 2011-2013 Marshmallow Engine. All rights reserved.
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
	uint8_t       *buffer;
	const uint8_t *const_buffer;
	DIOMode        mode;
	long           cursor;
	long           size;
	bool           eof;
};

BufferIO::BufferIO(void *b, size_t s)
    : m_p(new Private)
{
	assert(s <= LONG_MAX && "Buffer too large!");

	m_p->buffer = reinterpret_cast<uint8_t *>(b);
	m_p->const_buffer = reinterpret_cast<uint8_t *>(b);
	m_p->mode = DIOReadWrite;
	m_p->eof = false;
	m_p->cursor = 0;
	m_p->size = long(s);
}

BufferIO::BufferIO(const void *cb, size_t s)
    : m_p(new Private)
{
	assert(s <= LONG_MAX && "Buffer too large!");

	m_p->buffer = 0;
	m_p->const_buffer = reinterpret_cast<const uint8_t *>(cb);
	m_p->mode = DIOReadOnly;
	m_p->eof = false;
	m_p->cursor = 0;
	m_p->size = long(s);
}

BufferIO::BufferIO(IDataIO *source)
    : m_p(new Private)
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

	if (!source->seek(0, DIOEnd)) {
		MMERROR("Failed to seek to end.");
		return;
	}

	const long l_size = source->tell();

	if (l_size <= 0) {
		MMERROR("Source DIO has invalid size.");
		return;
	}

	if (!source->seek(0, DIOSet)) {
		MMERROR("Failed to seek back to beginning.");
		return;
	}

	uint8_t *l_buffer = new uint8_t[l_size];

	if (l_size != long(source->read(l_buffer, size_t(l_size)))) {
		MMERROR("Failed to read data from source DIO.");
		return;
	}

	m_p->buffer = l_buffer;
	m_p->const_buffer = l_buffer;
	m_p->mode = DIOReadWrite;
	m_p->eof = false;
	m_p->cursor = 0;
	m_p->size = l_size;

	if (!source->seek(l_cursor, DIOSet))
		MMERROR("Failed to return source DIO to original position.");
}

BufferIO::BufferIO(const BufferIO &source)
    : m_p(new Private)
{
	uint8_t *l_buffer = new uint8_t[source.m_p->size];
	memcpy(l_buffer, source.m_p->const_buffer, size_t(source.m_p->size));

	m_p->buffer = l_buffer;
	m_p->const_buffer = l_buffer;
	m_p->mode = DIOReadWrite;
	m_p->eof = false;
	m_p->cursor = 0;
	m_p->size = source.m_p->size;
}

BufferIO::~BufferIO(void)
{
	close();

	delete m_p, m_p = 0;
}

size_t
BufferIO::size(void) const
{
	return(size_t(m_p->size));
}

bool
BufferIO::open(DIOMode)
{
	MMWARNING("BufferIO open was called, it will be ignored.");
	return(isOpen());
}

void
BufferIO::close(void)
{
	m_p->buffer = 0;
	m_p->const_buffer = 0;
	m_p->mode = DIOInvalid;
	m_p->eof = false;
	m_p->cursor = 0;
	m_p->size = 0;
}

DIOMode
BufferIO::mode(void) const
{
	return(m_p->mode);
}

bool
BufferIO::isOpen(void) const
{
	return(m_p->const_buffer && m_p->mode != DIOInvalid);
}

bool
BufferIO::atEOF(void) const
{
	return(m_p->eof);
}

size_t
BufferIO::read(void *b, size_t bs)
{
	assert(bs <= LONG_MAX && "Buffer too large!");

	if (!m_p->const_buffer &&
	    (m_p->cursor >= 0 || m_p->cursor >= m_p->size)) return(0);

	size_t l_rcount =
	    (m_p->cursor + long(bs) < m_p->size ?  bs : size_t(m_p->size - m_p->cursor));

	memcpy(b, m_p->const_buffer + m_p->cursor, l_rcount);
	m_p->cursor += l_rcount;

	/* set end-of-file flag */
	m_p->eof = (bs > l_rcount);

	return(l_rcount);
}

size_t
BufferIO::write(const void *b, size_t bs)
{
	assert(bs <= LONG_MAX && "Buffer too large!");

	if (!m_p->buffer && m_p->cursor >= 0) return(0);

	size_t l_wcount =
	    (m_p->cursor + long(bs) < m_p->size ? bs : size_t(m_p->size - m_p->cursor));

	memcpy(m_p->buffer + m_p->cursor, b, l_wcount);
	m_p->cursor += l_wcount;

	/* set end-of-file flag */
	m_p->eof = (bs > l_wcount);

	return(l_wcount);
}

bool
BufferIO::seek(long o, DIOSeek on)
{
	long l_cursor = -1;

	switch (on) {
	case DIOSet:
		if (o >= 0 && o <= m_p->size)
			l_cursor = o;
		break;
	case DIOEnd:
		if (m_p->size + o >= 0)
			l_cursor = m_p->size + o;
		break;
	case DIOCurrent:
		if ((m_p->cursor + o) >= 0 &&
		    m_p->cursor + o < m_p->size)
			l_cursor = m_p->cursor + o;
		break;
	default: return(false);
	}

	if (-1 == l_cursor)
		return(false);

	/* reset end-of-file flag */
	m_p->eof = false;

	m_p->cursor = l_cursor;
	return(true);
}

long
BufferIO::tell(void) const
{
	return(m_p->cursor);
}

} /*********************************************************** Core Namespace */
MARSHMALLOW_NAMESPACE_END

