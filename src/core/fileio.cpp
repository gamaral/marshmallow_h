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

#include "core/fileio.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/logger.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Core { /******************************************** Core Namespace */

struct FileIO::Private
{
	Identifier filename;
	DIOMode    mode;
	FILE      *handle;
	bool       eof;
};

FileIO::FileIO(void)
    : m_p(new Private)
{
	m_p->handle = 0;
	m_p->mode = DIOInvalid;
	m_p->eof = false;
}

FileIO::FileIO(const Identifier &fn, DIOMode m)
    : m_p(new Private)
{
	m_p->handle = 0;
	m_p->mode = DIOInvalid;
	m_p->eof = false;
	setFileName(fn);
	open(m);
}

FileIO::~FileIO(void)
{
	close();
	delete m_p, m_p = 0;
}

const Identifier &
FileIO::fileName(void) const
{
	return(m_p->filename);
}

void
FileIO::setFileName(const Identifier &fn)
{
	if (isOpen()) {
		MMERROR("Can't change filename on open device.");
		return;
	}

	m_p->filename = fn;
}

bool
FileIO::open(DIOMode m)
{
	if (!m_p->filename) {
		MMWARNING("Tried to open device without a filename.");
		return(false);
	}

	char l_mode[4] = {0, 0, 0, 0};
	unsigned char l_mode_c = 0;

	if (m & DIOReadOnly)
		l_mode[l_mode_c++] = 'r';
	if (m & DIOWriteOnly)
		l_mode[l_mode_c++] = 'w';
	if (m & DIOBinary)
		l_mode[l_mode_c++] = 'b';
	
	m_p->handle = fopen(m_p->filename, l_mode);
	m_p->mode = m;
	m_p->eof = false;
	return(m_p->handle != 0);
}

void
FileIO::close(void)
{
	fclose(m_p->handle);
	m_p->handle = 0;
	m_p->mode = DIOInvalid;
	m_p->eof = false;
	m_p->filename = Identifier();
}

DIOMode
FileIO::mode(void) const
{
	return(m_p->mode);
}

bool
FileIO::isOpen(void) const
{
	return(m_p->handle != 0);
}

bool
FileIO::atEOF(void) const
{
	return(m_p->eof);
}


size_t
FileIO::read(void *b, size_t bs)
{
	size_t l_read = fread(b, 1, bs, m_p->handle);

	if (l_read < bs)
	    m_p->eof = (feof(m_p->handle) != 0);

	return(l_read);
}

size_t
FileIO::write(const void *b, size_t bs)
{
	size_t l_written = fwrite(b, 1, bs, m_p->handle);

	/* update end-of-file flag */
	m_p->eof = (feof(m_p->handle) != 0);

	return(l_written);
}

bool
FileIO::seek(long o, DIOSeek on)
{
	int l_origin;

	switch (on) {
	case   DIOStart: l_origin = SEEK_SET; break;
	case     DIOEnd: l_origin = SEEK_END; break;
	case DIOCurrent: l_origin = SEEK_CUR; break;
	default: return(false);
	}

	if (fseek(m_p->handle, o, l_origin) != 0)
		return(false);

	/* update end-of-file flag */
	m_p->eof = (feof(m_p->handle) != 0);

	return(true);
}

long
FileIO::tell(void) const
{
	return(ftell(m_p->handle));
}

size_t
FileIO::size(void) const
{
	const long l_cursor = ftell(m_p->handle);
	if (l_cursor == -1) {
		MMWARNING("Failed to get current cursor position in file.");
		return(0);
	}

	long l_result = -1;

	if (fseek(m_p->handle, 0, SEEK_END) != 0) {
		MMWARNING("Failed to move cursor to end of file.");
		return(0);
	}

	if ((l_result = ftell(m_p->handle)) == -1) {
		MMWARNING("Failed to move cursor to end of file.");
		return(0);
	}

	if (fseek(m_p->handle, l_cursor, SEEK_SET) != 0)
		MMWARNING("Failed to return cursor to last position in file.");

	return(l_result > 0 ? static_cast<size_t>(l_result) : 0);
}

} /*********************************************************** Core Namespace */
MARSHMALLOW_NAMESPACE_END

