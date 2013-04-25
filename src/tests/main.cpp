/*
 * Copyright (c) 2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
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

#include "core/platform.h"

#include "common.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

const char * g_test_format = "[%s] %s:%d \"%s\"\n";
const char * g_str_passed  = "PASS";
const char * g_str_failed  = "FAIL";
bool g_passed = true;

extern test_proc tests[];

int
main(int, char *[])
{
	if (-1 == MMCHDIR(MARSHMALLOW_TESTS_DIRECTORY)) {
		fprintf(stderr, "Failed to change directory!");
		return(2);
	}

	int i = 0;
	while (tests[i] != 0 && g_passed)
		tests[i++]();

	return(g_passed ? 0 : 1);
}

