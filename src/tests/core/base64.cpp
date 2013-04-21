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

#include "core/base64.h"
#include "core/global.h"

#include "tests/common.h"

static const char  base64a[] = "MTIzNDU2";
static const char ubase64a[] = "123456";
static const char  base64b[] = "MTIzNDU=";
static const char ubase64b[] = "12345";
static const char  base64c[] = "MTIzNA==";
static const char ubase64c[] = "1234";

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

MARSHMALLOW_NAMESPACE_USE

void
base64_decode_test(void)
{
	char *out = 0;

	Core::Base64::Decode(base64a, strlen(base64a), &out);
	ASSERT_ZERO("Core::Base64::Decode() TEST A", strcmp(out, ubase64a));
	delete[] out;

	Core::Base64::Decode(base64b, strlen(base64b), &out);
	ASSERT_ZERO("Core::Base64::Decode() TEST B", strcmp(out, ubase64b));
	delete[] out;

	Core::Base64::Decode(base64c, strlen(base64c), &out);
	ASSERT_ZERO("Core::Base64::Decode() TEST C", strcmp(out, ubase64c));
	delete[] out;
}

void
base64_encode_test(void)
{
	char *out;

	Core::Base64::Encode(ubase64a, strlen(ubase64a), &out);
	ASSERT_ZERO("Core::Base64::Encode() TEST A", strcmp(out, base64a));
	delete[] out;

	Core::Base64::Encode(ubase64b, strlen(ubase64b), &out);
	ASSERT_ZERO("Core::Base64::Encode() TEST B", strcmp(out, base64b));
	delete[] out;

	Core::Base64::Encode(ubase64c, strlen(ubase64c), &out);
	ASSERT_ZERO("Core::Base64::Encode() TEST C", strcmp(out, base64c));
	delete[] out;
}

int
main(int, char *[])
{
	MMCHDIR(MARSHMALLOW_TESTS_DIRECTORY);

	RUN_TEST(base64_decode_test);
	RUN_TEST(base64_encode_test);

	return(TEST_EXITCODE);
}

