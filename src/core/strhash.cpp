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

#include "core/type.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <cassert>
#include <cstring>

MARSHMALLOW_NAMESPACE_USE;
using namespace Core;

StrHash::StrHash(const char *n)
    : Hash(),
      m_str(0)
{
	assert(n);

	const size_t len = strlen(n)+1;
	m_str = new char[len];
	memcpy(m_str, n, len);
	rehash(m_str, len);
}

StrHash::StrHash(const StrHash &copy)
    : Hash(copy),
      m_str(0)
{
	const size_t len = strlen(copy.m_str)+1;
	m_str = new char[len];
	memcpy(m_str, copy.m_str, len);
}

StrHash::~StrHash(void)
{
	delete[] m_str;
}

StrHash &
StrHash::operator=(const Marshmallow::Core::StrHash &rhs)
{
	if (this != &rhs) {
		Hash::operator=(rhs);

		delete[] m_str;
		const size_t len = strlen(rhs.m_str)+1;
		m_str = new char[len];
		memcpy(m_str, rhs.m_str, len);
	}
	return(*this);
}

