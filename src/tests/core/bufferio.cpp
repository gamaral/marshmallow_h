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

#include <cstring>

#include "core/bufferio.h"

#include "tests/common.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

MARSHMALLOW_NAMESPACE_USE

static const char s_content[] = "this is a test!";
static const size_t s_content_size = sizeof(s_content);

void
buffer_readonly_test(void)
{
	char l_scratch[15];
	const size_t l_scratch_size = sizeof(l_scratch);
	memset(l_scratch, 0, l_scratch_size);

	size_t l_bytes_read;
	Core::BufferIO l_buffer(s_content, s_content_size);

	ASSERT_TRUE("Core::BufferIO::isOpen() == TRUE", l_buffer.isOpen());

	const size_t l_size = l_buffer.size();
	ASSERT_EQUAL("Core::BufferIO::size() CONFIRM", s_content_size, l_size);

	l_bytes_read = l_buffer.read(l_scratch, 4);
	ASSERT_EQUAL("Core::BufferIO::read() 4 BYTES", 4, l_bytes_read);
	ASSERT_ZERO("Core::BufferIO::read() CONFIRM DATA OK",
	    strncmp(l_scratch, s_content, 4));

	l_bytes_read = l_buffer.seek(1, Core::DIOCurrent);
	ASSERT_TRUE("Core::BufferIO::seek() 1 BYTE FROM CURRENT", l_bytes_read);

	memset(l_scratch, 0, l_scratch_size);
	l_bytes_read = l_buffer.read(l_scratch, 2);
	ASSERT_EQUAL("Core::BufferIO::read() 2 BYTES", 2, l_bytes_read);
	ASSERT_ZERO("Core::BufferIO::read() CONFIRM DATA OK",
	    strncmp(l_scratch, &s_content[5], 2));

	l_bytes_read = l_buffer.seek(-6, Core::DIOEnd);
	ASSERT_TRUE("Core::BufferIO::seek() -6 BYTES FROM END", l_bytes_read);

	memset(l_scratch, 0, l_scratch_size);
	l_bytes_read = l_buffer.read(l_scratch, 6);
	ASSERT_EQUAL("Core::BufferIO::read() 6 BYTES", 6, l_bytes_read);
	ASSERT_ZERO("Core::BufferIO::read() CONFIRM DATA OK",
	    strncmp(l_scratch, &s_content[s_content_size-6], 6));
	
	ASSERT_FALSE("Core::BufferIO::atEOF() == FALSE", l_buffer.atEOF());
	l_buffer.read(l_scratch, 1);
	ASSERT_TRUE("Core::BufferIO::atEOF() == TRUE", l_buffer.atEOF());

	l_buffer.close();
	ASSERT_FALSE("Core::BufferIO::close()", l_buffer.isOpen());
}

void
buffer_write_test(void)
{
	char l_scratch[16];
	const size_t l_scratch_size = sizeof(l_scratch);
	memset(l_scratch, 0, l_scratch_size);

	size_t l_bytes_written;
	Core::BufferIO l_buffer(l_scratch, l_scratch_size);

	ASSERT_TRUE("Core::BufferIO::isOpen() == TRUE", l_buffer.isOpen());

	l_bytes_written = l_buffer.write("thiss", 5);
	ASSERT_EQUAL("Core::BufferIO::write() 5 BYTES", 5, l_bytes_written);
	ASSERT_ZERO("Core::BufferIO::write() CONFIRM DATA OK", strcmp(l_scratch, "thiss"));

	l_bytes_written = l_buffer.seek(-1, Core::DIOCurrent);
	ASSERT_TRUE("Core::BufferIO::seek() -1 FROM CURRENT", l_bytes_written);

	l_bytes_written = l_buffer.write(" ", 1);
	ASSERT_EQUAL("Core::BufferIO::write() 1 BYTE", 1, l_bytes_written);
	ASSERT_ZERO("Core::BufferIO::write() CONFIRM DATA OK", strcmp(l_scratch, "this "));

	l_bytes_written = l_buffer.seek(-11, Core::DIOEnd);
	ASSERT_TRUE("Core::BufferIO::seek() -11 BYTES FROM END", l_bytes_written);

	l_bytes_written = l_buffer.write("is a test!", 10);
	ASSERT_EQUAL("Core::BufferIO::write() 10 BYTES", 10, l_bytes_written);
	ASSERT_ZERO("Core::BufferIO::write() CONFIRM DATA OK", strcmp(l_scratch, s_content));

	l_buffer.close();
	ASSERT_FALSE("Core::BufferIO::close()", l_buffer.isOpen());
}

void
buffer_clone_test(void)
{
	char l_scratch[16];
	const size_t l_scratch_size = sizeof(l_scratch);
	memset(l_scratch, 0, l_scratch_size);

	Core::BufferIO l_buffer_orig(s_content, s_content_size);
	Core::BufferIO l_buffer_copy(l_buffer_orig);
	ASSERT_TRUE("Core::BufferIO::isOpen() == TRUE", l_buffer_copy.isOpen());

	size_t l_read = l_buffer_copy.read(l_scratch, 15);
	ASSERT_EQUAL("Core::BufferIO::read() 15 BYTES", 15, l_read);
	ASSERT_ZERO("Core::BufferIO::read() CONFIRM DATA OK", strcmp(l_scratch, s_content));

	Core::BufferIO l_buffer_clone(&l_buffer_orig);
	ASSERT_TRUE("Core::BufferIO::isOpen() == TRUE", l_buffer_clone.isOpen());

	memset(l_scratch, 0, l_scratch_size);
	l_read = l_buffer_clone.read(l_scratch, 15);
	ASSERT_EQUAL("Core::BufferIO::read() 15 BYTES", 15, l_read);
	ASSERT_ZERO("Core::BufferIO::read() CONFIRM DATA OK", strcmp(l_scratch, s_content));

	l_buffer_orig.close();
	ASSERT_FALSE("Core::BufferIO::close()", l_buffer_orig.isOpen());
}

int
main(int, char *[])
{
	MMCHDIR(MARSHMALLOW_TESTS_DIRECTORY);

	RUN_TEST(buffer_readonly_test);
	RUN_TEST(buffer_write_test);
	RUN_TEST(buffer_clone_test);

	return(TEST_EXITCODE);
}

