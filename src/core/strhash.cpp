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

#include "core/type.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <cassert>
#include <cstring>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Core { /******************************************** Core Namespace */

const StrHash StrHash::Null;

struct StrHash::Private
{
	std::string str;
};

StrHash::StrHash(void)
    : Hash()
    , m_p(new Private)
{
}

StrHash::StrHash(const char *s)
    : Hash()
    , m_p(new Private)
{
	m_p->str = s;
	rehash(s, m_p->str.length(), ~static_cast<MMUID>(0));
}

StrHash::StrHash(const std::string &s)
    : Hash()
    , m_p(new Private)
{
	m_p->str = s;
	rehash(m_p->str.c_str(), m_p->str.length(), ~static_cast<MMUID>(0));
}

StrHash::StrHash(const StrHash &c)
    : Hash(c)
    , m_p(new Private)
{
	m_p->str = c.m_p->str;
}

StrHash::~StrHash(void)
{
	delete m_p, m_p = 0;
}

const std::string &
StrHash::str(void) const
{
	return(m_p->str);
}

StrHash::operator const std::string & (void) const
{
	return(m_p->str);
}

StrHash::operator const char * (void) const
{
	return(m_p->str.c_str());
}

StrHash &
StrHash::operator=(const StrHash &rhs)
{
	if (this != &rhs) {
		Hash::operator=(rhs);
		m_p->str = rhs.m_p->str;
	}
	return(*this);
}

} /*********************************************************** Core Namespace */
MARSHMALLOW_NAMESPACE_END

