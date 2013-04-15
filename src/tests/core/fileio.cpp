/*
 * Copyright 2013 Marshmallow Engine. All rights reserved.
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

#include "core/fileio.h"
#include "core/identifier.h"

#include "tests/common.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

MARSHMALLOW_NAMESPACE_USE

static const char s_content[] = "this is a test!";
static const size_t s_content_size = sizeof(s_content);
static const Core::Identifier s_data_file("core/data/fileio.dat");

void
fileio_readonly_test(void)
{
	char l_scratch[s_content_size];

	Core::FileIO l_file(s_data_file, Core::DIOReadOnly);
	ASSERT_TRUE("Core::FileIO::open()", l_file.isOpen());
	if (!l_file.isOpen()) return;

	const size_t l_size = l_file.size();
	ASSERT_EQUAL("Core::FileIO::size() CONFIRM", s_content_size, l_size);

	size_t l_read = l_file.read(l_scratch, s_content_size);
	ASSERT_EQUAL("Core::FileIO::read() ALL BYTES", s_content_size, l_read);
	ASSERT_ZERO("Core::FileIO::read() CONFIRM DATA OK",
	    strncmp(l_scratch, s_content, s_content_size));
	ASSERT_FALSE("Core::FileIO::isEOF() == FALSE", l_file.atEOF());

	l_read = l_file.read(l_scratch, 1);
	ASSERT_ZERO("Core::FileIO::read() FAIL/EOF", l_read);
	ASSERT_TRUE("Core::FileIO::isEOF() == TRUE", l_file.atEOF());

	bool l_seeked = l_file.seek(-6, Core::DIOCurrent);
	ASSERT_TRUE("Core::FileIO::seek() -6 BYTES FROM CURRENT", l_seeked);
	ASSERT_EQUAL("Core::FileIO::tell() CONFIRM NEW POSITION",
	    s_content_size -6, l_file.tell());
	l_read = l_file.read(l_scratch, 6);
	ASSERT_EQUAL("Core::FileIO::read() 6 BYTES", l_read, 6);
	ASSERT_ZERO("Core::FileIO::read() CONFIRM DATA OK",
	    strcmp(l_scratch, &s_content[s_content_size -6]));
	ASSERT_FALSE("Core::FileIO::isEOF() == FALSE", l_file.atEOF());

	l_seeked = l_file.seek(-5, Core::DIOEnd);
	ASSERT_TRUE("Core::FileIO::seek() -5 BYTES FROM END", l_seeked);
	ASSERT_EQUAL("Core::FileIO::tell() CONFIRM NEW POSITION",
	    s_content_size -5, l_file.tell());
	l_read = l_file.read(l_scratch, 5);
	ASSERT_EQUAL("Core::FileIO::read() 5 BYTES", l_read, 5);
	ASSERT_ZERO("Core::FileIO::read() CONFIRM DATA OK",
	    strcmp(l_scratch, &s_content[s_content_size -5]));
	ASSERT_FALSE("Core::FileIO::isEOF() == FALSE", l_file.atEOF());

	l_seeked = l_file.seek(4, Core::DIOSet);
	ASSERT_TRUE("Core::FileIO::seek() SET CURSOR POSITION TO 4", l_seeked);
	ASSERT_EQUAL("Core::FileIO::tell() CONFIRM NEW POSITION", 4, l_file.tell());
	l_read = l_file.read(l_scratch, 4);
	ASSERT_EQUAL("Core::FileIO::read() 4 BYTES", l_read, 4);
	ASSERT_ZERO("Core::FileIO::read() CONFIRM DATA OK",
	    strncmp(l_scratch, &s_content[4], 4));
	ASSERT_FALSE("Core::FileIO::isEOF() == FALSE", l_file.atEOF());

	l_seeked = l_file.seek(0, Core::DIOEnd);
	ASSERT_TRUE("Core::FileIO::seek() SET CURSOR TO EOF", l_seeked);
	l_read = l_file.read(l_scratch, 1);
	ASSERT_ZERO("Core::FileIO::read() FAIL/EOF", l_read);
	ASSERT_TRUE("Core::FileIO::isEOF() == TRUE", l_file.atEOF());

	l_file.close();
	ASSERT_FALSE("Core::FileIO::close()", l_file.isOpen());
}

void
fileio_writeonly_test(void)
{
	Core::FileIO l_file(s_data_file, Core::DIOTruncate);
	ASSERT_TRUE("Core::FileIO::open() TRUNCATE", l_file.isOpen());
	if (!l_file.isOpen()) return;

	size_t l_written = l_file.write(s_content, s_content_size);
	ASSERT_EQUAL("Core::FileIO::write() WRITE TEST DATA", l_written, s_content_size);

	ASSERT_EQUAL("Core::FileIO::tell() CONFIRM POSITION",
	    s_content_size, l_file.tell());

	ASSERT_EQUAL("Core::FileIO::tell() CONFIRM POSITION",
	    s_content_size, l_file.tell());

	bool l_seeked = l_file.seek(0, Core::DIOSet);
	ASSERT_TRUE("Core::FileIO::seek() SET CURSOR TO BEGINNING OF FILE", l_seeked);
	ASSERT_ZERO("Core::FileIO::tell() CONFIRM POSITION", l_file.tell());

	l_file.close();
	ASSERT_FALSE("Core::FileIO::close()", l_file.isOpen());
}

int
main(int, char *[])
{
	MMCHDIR(MARSHMALLOW_TESTS_DIRECTORY);

	RUN_TEST(fileio_writeonly_test);
	RUN_TEST(fileio_readonly_test);

	return(TEST_EXITCODE);
}

