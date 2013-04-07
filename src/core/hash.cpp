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

#include "core/hash.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace Core { /******************************************** Core Namespace */

struct Hash::Private
{
	MMUID result;
};

Hash::Hash(void)
    : m_p(new Private)
{
	m_p->result = 0;
}

Hash::Hash(const char *d, size_t length, MMUID mask)
    : m_p(new Private)
{
	m_p->result = Algorithm(d, length, mask);
}

Hash::Hash(const Hash &copy)
    : m_p(new Private)
{
	m_p->result = copy.m_p->result;
}

Hash::~Hash(void)
{
	delete m_p, m_p = 0;
}

MMUID
Hash::result(void) const
{
	return(m_p->result);
}

Hash::operator MMUID() const
{
	return(m_p->result);
}

Hash &
Hash::operator=(const Core::Hash &rhs)
{
	if (this != &rhs)
		m_p->result = rhs.m_p->result;
	return(*this);
}

bool
Hash::operator==(const Hash &rhs) const
{
	return(m_p->result == rhs.m_p->result);
}

bool
Hash::operator!=(const Hash &rhs) const
{
	return(m_p->result != rhs.m_p->result);
}

bool
Hash::operator<(const Hash &rhs) const
{
	return(m_p->result < rhs.m_p->result);
}

MMUID
Hash::Algorithm(const char *data, size_t length, MMUID mask)
{
	if (!data) return(0);

	MMUID l_hash = 0;
	size_t l_i;

	for(l_i = 0; l_i < length; ++l_i) {
		l_hash += static_cast<MMUID>(data[l_i]);
		l_hash += (l_hash << 0x0A);
		l_hash ^= (l_hash >> 0x06);
	}

	l_hash += (l_hash << 0x03);
	l_hash ^= (l_hash >> 0x0B);
	l_hash += (l_hash << 0x0F);

	return(l_hash & mask);
}

void
Hash::rehash(const char *d, size_t length, MMUID mask)
{
	m_p->result = Algorithm(d, length, mask);
}

} /*********************************************************** Core Namespace */
MARSHMALLOW_NAMESPACE_END

