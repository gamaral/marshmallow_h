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

#include <cstring>

#include "core/base64.h"
#include "../common.h"

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

MARSHMALLOW_NAMESPACE_USE;

void
base64_decode_test(void)
{
	char *out;

	Core::Base64::Decode(base64a, strlen(base64a), &out);
	TEST("DECODE TEST A", 0 == strcmp(out, ubase64a));
	delete[] out;

	Core::Base64::Decode(base64b, strlen(base64b), &out);
	TEST("DECODE TEST B", 0 == strcmp(out, ubase64b));
	delete[] out;

	Core::Base64::Decode(base64c, strlen(base64c), &out);
	TEST("DECODE TEST C", 0 == strcmp(out, ubase64c));
	delete[] out;
}

void
base64_encode_test(void)
{
	char *out;

	Core::Base64::Encode(ubase64a, strlen(ubase64a), &out);
	TEST("ENCODE TEST A", 0 == strcmp(out, base64a));
	delete[] out;

	Core::Base64::Encode(ubase64b, strlen(ubase64b), &out);
	TEST("ENCODE TEST B", 0 == strcmp(out, base64b));
	delete[] out;

	Core::Base64::Encode(ubase64c, strlen(ubase64c), &out);
	TEST("ENCODE TEST C", 0 == strcmp(out, base64c));
	delete[] out;
}

int
MMain(int argc, char *argv[])
{
	UNUSED(argc);
	UNUSED(argv);

	base64_decode_test();
	base64_encode_test();

	return(TEST_EXITCODE);
}

