/*
 * Copyright 2011 Marshmallow Engine. All rights reserved.
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

MARSHMALLOW_NAMESPACE_USE;
using namespace Core;

BufferIO::BufferIO(void)
    : m_buffer(0)
    , m_const_buffer(0)
    , m_cursor(0)
    , m_size(0)
{
}

BufferIO::BufferIO(char *b, size_t s)
    : m_buffer(b)
    , m_const_buffer(b)
    , m_cursor(0)
    , m_size(s)
{
}

BufferIO::BufferIO(const char *cb, size_t s)
    : m_buffer(0)
    , m_const_buffer(cb)
    , m_cursor(0)
    , m_size(s)
{
}

BufferIO::~BufferIO(void)
{
	close();
}

bool
BufferIO::open(const Identifier &, DIOMode)
{
	MMWARNING1("BufferIO open was called, it will be ignored.");
	return(isOpen());
}

void
BufferIO::close(void)
{
	m_buffer = 0;
	m_const_buffer = 0;
	m_cursor = 0;
	m_size = 0;
}

bool
BufferIO::isOpen(void) const
{
	return(m_const_buffer);
}

size_t
BufferIO::read(char *b, size_t bs)
{
	if (!m_const_buffer && m_cursor >= 0) return(0);

	size_t l_rcount =
	    (m_cursor + bs < m_size ? bs : m_size - m_cursor);

	memcpy(b, m_const_buffer + m_cursor, l_rcount);
	m_cursor += l_rcount;

	return(l_rcount);
}

size_t
BufferIO::write(const char *b, size_t bs)
{
	if (!m_buffer && m_cursor >= 0) return(0);

	size_t l_rcount =
	    (m_cursor + bs < m_size ? bs : m_size - m_cursor);

	memcpy(m_buffer + m_cursor, b, l_rcount);
	m_cursor += l_rcount;

	return(l_rcount);
}

bool
BufferIO::seek(long o, DIOSeek on)
{
	long l_cursor = -1;

	switch (on) {
	case   DIOStart: if (o < m_size) l_cursor = o; break;
	case     DIOEnd: if (m_size + o >= 0) l_cursor = m_size + o; break;
	case DIOCurrent: if (m_cursor + o < m_size) l_cursor = m_cursor + o; break;
	default: return(false);
	}

	if (-1 == l_cursor)
		return(false);

	m_cursor = l_cursor;
	return(true);
}

