/*
 * Copyright 2011-2012 Marshmallow Engine. All rights reserved.
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

#include <cstring>

#include "core/logger.h"

MARSHMALLOW_NAMESPACE_USE
using namespace Core;

struct BufferIO::Private
{
	char       *buffer;
	const char *const_buffer;
	DIOMode     mode;
	long        cursor;
	size_t      size;
};

BufferIO::BufferIO(char *b, size_t s)
    : m_p(new Private)
{
	m_p->buffer = b;
	m_p->const_buffer = b;
	m_p->mode = DIOReadWrite;
	m_p->cursor = 0;
	m_p->size = s;
}

BufferIO::BufferIO(const char *cb, size_t s)
    : m_p(new Private)
{
	m_p->buffer = 0;
	m_p->const_buffer = cb;
	m_p->mode = DIOReadOnly;
	m_p->cursor = 0;
	m_p->size = s;
}

BufferIO::~BufferIO(void)
{
	close();
	delete m_p;
	m_p = 0;
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

size_t
BufferIO::read(char *b, size_t bs)
{
	if (!m_p->const_buffer && m_p->cursor >= 0) return(0);

	size_t l_rcount =
	    (m_p->cursor + bs < m_p->size ? bs : m_p->size - m_p->cursor);

	memcpy(b, m_p->const_buffer + m_p->cursor, l_rcount);
	m_p->cursor += l_rcount;

	return(l_rcount);
}

size_t
BufferIO::write(const char *b, size_t bs)
{
	if (!m_p->buffer && m_p->cursor >= 0) return(0);

	size_t l_rcount =
	    (m_p->cursor + bs < m_p->size ? bs : m_p->size - m_p->cursor);

	memcpy(m_p->buffer + m_p->cursor, b, l_rcount);
	m_p->cursor += l_rcount;

	return(l_rcount);
}

bool
BufferIO::seek(long o, DIOSeek on)
{
	long l_cursor = -1;

	switch (on) {
	case DIOStart:
		if (o >= 0 && static_cast<size_t>(o) < m_p->size)
			l_cursor = o;
		break;
	case DIOEnd:
		if (static_cast<long>(m_p->size + o) >= 0)
			l_cursor = m_p->size + o;
		break;
	case DIOCurrent:
		if ((m_p->cursor + o) >= 0 &&
		    (m_p->cursor + o) < m_p->size)
			l_cursor = m_p->cursor + o;
		break;
	default: return(false);
	}

	if (-1 == l_cursor)
		return(false);

	m_p->cursor = l_cursor;
	return(true);
}

long
BufferIO::tell(void) const
{
	return(m_p->cursor);
}

size_t
BufferIO::size(void) const
{
	return(m_p->size);
}

