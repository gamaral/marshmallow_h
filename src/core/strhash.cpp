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
    , PIMPL_CREATE
{
}

StrHash::StrHash(const char *s)
    : Hash()
    , PIMPL_CREATE
{
	PIMPL->str = s;
	rehash(s, PIMPL->str.length(), ~static_cast<MMUID>(0));
}

StrHash::StrHash(const std::string &s)
    : Hash()
    , PIMPL_CREATE
{
	PIMPL->str = s;
	rehash(PIMPL->str.c_str(), PIMPL->str.length(), ~static_cast<MMUID>(0));
}

StrHash::StrHash(const StrHash &c)
    : Hash(c)
    , PIMPL_CREATE
{
	PIMPL->str = c.PIMPL->str;
}

StrHash::~StrHash(void)
{
	PIMPL_DESTROY;
}

const std::string &
StrHash::str(void) const
{
	return(PIMPL->str);
}

StrHash::operator const std::string & (void) const
{
	return(PIMPL->str);
}

StrHash::operator const char * (void) const
{
	return(PIMPL->str.c_str());
}

StrHash &
StrHash::operator=(const StrHash &rhs)
{
	if (this != &rhs) {
		Hash::operator=(rhs);
		PIMPL->str = rhs.PIMPL->str;
	}
	return(*this);
}

} /*********************************************************** Core Namespace */
MARSHMALLOW_NAMESPACE_END

