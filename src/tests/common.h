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

#include <cstdio>

#include "config.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

static const char * s_test_format = "[%s] %s:%d \"%s\"\n";
static const char * s_str_passed  = "PASS";
static const char * s_str_failed  = "FAIL";
static bool s_passed = true;

#define ASSERT_TRUE(x, y) fprintf(stderr, s_test_format, (y) ? s_str_passed : s_str_failed, __FUNCTION__, __LINE__, x), s_passed &= (y)
#define ASSERT_FALSE(x, y) ASSERT_TRUE(x, !(y))
#define ASSERT_VALID(x, y) ASSERT_TRUE(x, y)
#define ASSERT_INVALID(x, y) ASSERT_TRUE(x, !y)
#define ASSERT_ZERO(x, y) ASSERT_TRUE(x, 0 == (y))
#define ASSERT_NOT_ZERO(x, y) ASSERT_TRUE(x, 0 != (y))
#define ASSERT_NULL(x, y) ASSERT_TRUE(x, null == y)
#define ASSERT_NOT_NULL(x, y) ASSERT_TRUE(x, null != y)
#define ASSERT_EQUAL(x, y, z) ASSERT_TRUE(x, (y) == (z))
#define ASSERT_NOT_EQUAL(x, y, z) ASSERT_TRUE(x, (y) != (z))

typedef void (*test_proc)(void);
#define TESTS_BEGIN test_proc tests[] = {
#define TEST(x) x,
#define TESTS_END 0 };

#define TEST_EXITCODE s_passed ? 0 : -1

