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

#include "core/shared.h"
#include "core/weak.h"

#include "tests/common.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

MARSHMALLOW_NAMESPACE_USE

void
shared_basic_test(void)
{
	ASSERT_INVALID("Core::Shared::Shared() INVALID", Core::Shared<int>());

	Core::Shared<int> valid(new int);
	ASSERT_VALID("Core::Shared::Shared()", valid);

	/* clear valid shared pointer */
	valid.clear();
	ASSERT_INVALID("Core::Shared::clear()", valid);
}

void
weak_basic_test(void)
{
	ASSERT_INVALID("Core::Weak::Weak() INVALID", Core::Weak<int>());

	Core::Shared<int> svalid(new int);
	Core::Weak<int> valid(svalid);
	ASSERT_VALID("Core::Weak::Weak()", valid);

	/* clear valid weak pointer */
	valid.clear();
	ASSERT_INVALID("Core::Weak::clear()", valid);
}

int
main(int, char *[])
{
	MMCHDIR(MARSHMALLOW_TESTS_DIRECTORY);

	RUN_TEST(shared_basic_test);
	RUN_TEST(weak_basic_test);

	return(TEST_EXITCODE);
}

