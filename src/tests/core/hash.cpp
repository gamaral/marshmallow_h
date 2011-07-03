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

#include "core/hash.h"
#include "core/strhash.h"
#include "../common.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

MARSHMALLOW_NAMESPACE_USE;

void
hash_compare_test(void)
{
	TEST("ZERO SAME AS NULL-HASH",
	     0 == Core::Hash());
	TEST("ZERO NOT SAME AS TEST HASH",
	     0 != Core::Hash("test", 4, ~static_cast<UID>(0)));
}

void
strhash_compare_test(void)
{
	TEST("STRHASH TEST SAME AS STRHASH TEST",
	     Core::StrHash("test") == Core::StrHash("test"));
	TEST("STRHASH TSET NOT SAME AS STRHASH TEST",
	     Core::StrHash("tset") != Core::StrHash("test"));
}

MARSHMALLOW_NAMESPACE_BEGIN

int
Main(int argc, const char *argv[])
{
	UNUSED(argc);
	UNUSED(argv);

	hash_compare_test();
	strhash_compare_test();

	return(TEST_EXITCODE);
}

MARSHMALLOW_NAMESPACE_END

