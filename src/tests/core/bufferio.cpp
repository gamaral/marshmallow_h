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

void
buffer_readonly_test(void)
{
	char l_scratch[15];
	size_t l_bytes_read;
	Core::BufferIO l_buffer(s_content, sizeof(s_content));

	TEST("BUFFER READONLY ISOPEN", l_buffer.isOpen());

	memset(l_scratch, 0, sizeof(l_scratch));
	l_bytes_read = l_buffer.read(l_scratch, 4);
	TEST("BUFFER READONLY READ 4 OK", 4 == l_bytes_read);
	TEST("BUFFER READONLY READ 4 COMPARE", 0 == strcmp(l_scratch, "this"));

	l_bytes_read = l_buffer.seek(1, Core::DIOCurrent);
	TEST("BUFFER READONLY SEEK CUR 1 OK", l_bytes_read);

	memset(l_scratch, 0, sizeof(l_scratch));
	l_bytes_read = l_buffer.read(l_scratch, 2);
	TEST("BUFFER READONLY READ 2 OK", 2 == l_bytes_read);
	TEST("BUFFER READONLY READ 2 COMPARE", 0 == strcmp(l_scratch, "is"));

	l_bytes_read = l_buffer.seek(-6, Core::DIOEnd);
	TEST("BUFFER READONLY SEEK END -6 OK", l_bytes_read);

	memset(l_scratch, 0, sizeof(l_scratch));
	l_bytes_read = l_buffer.read(l_scratch, 6);
	TEST("BUFFER READONLY READ 5 OK", 6 == l_bytes_read);
	TEST("BUFFER READONLY READ 6 COMPARE", 0 == strcmp(l_scratch, "test!"));
}

void
buffer_write_test(void)
{
	char l_scratch[15];
	size_t l_bytes_written;
	Core::BufferIO l_buffer(l_scratch, sizeof(l_scratch));

	memset(l_scratch, 0, sizeof(l_scratch));

	TEST("BUFFER WRITE ISOPEN", l_buffer.isOpen());

	l_bytes_written = l_buffer.write("thiss", 5);
	TEST("BUFFER WRITE WRITE 5 OK", 5 == l_bytes_written);
	TEST("BUFFER WRITE WRITE 5 COMPARE", 0 == strcmp(l_scratch, "thiss"));

	l_bytes_written = l_buffer.seek(-1, Core::DIOCurrent);
	TEST("BUFFER WRITE SEEK CUR -1 OK", l_bytes_written);

	l_bytes_written = l_buffer.write(" ", 1);
	TEST("BUFFER WRITE WRITE 2 OK", 1 == l_bytes_written);
	TEST("BUFFER WRITE WRITE 2 COMPARE", 0 == strcmp(l_scratch, "this "));

	l_bytes_written = l_buffer.seek(-10, Core::DIOEnd);
	TEST("BUFFER WRITE SEEK END -10 OK", l_bytes_written);

	l_bytes_written = l_buffer.write("is a test!", 10);
	TEST("BUFFER WRITE WRITE 10 OK", 10 == l_bytes_written);
	TEST("BUFFER WRITE WRITE 10 COMPARE", 0 == strcmp(l_scratch, s_content));
}

int
MMain(int argc, char *argv[])
{
	MMUNUSED(argc);
	MMUNUSED(argv);

	buffer_readonly_test();
	buffer_write_test();

	return(TEST_EXITCODE);
}

