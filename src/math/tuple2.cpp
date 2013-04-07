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

#include "math/tuple2.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace Math { /******************************************** Math Namespace */

Tuple2::Tuple2(float ax, float ay)
{
	m_value[0] = ax;
	m_value[1] = ay;
}

bool
Tuple2::operator==(const Tuple2 &rhs) const
{
	return(m_value[0] == rhs.m_value[0]
	    && m_value[1] == rhs.m_value[1]);
}

Tuple2 &
Tuple2::operator*=(float rhs)
{
	m_value[0] *= rhs;
	m_value[1] *= rhs;
	return(*this);
}

Tuple2 &
Tuple2::operator+=(float rhs)
{
	m_value[0] += rhs;
	m_value[1] += rhs;
	return(*this);
}

Tuple2 &
Tuple2::operator-=(float rhs)
{
	m_value[0] -= rhs;
	m_value[1] -= rhs;
	return(*this);
}

Tuple2 &
Tuple2::operator*=(const Tuple2 &rhs)
{
	m_value[0] *= rhs.m_value[0];
	m_value[1] *= rhs.m_value[1];
	return(*this);
}

Tuple2 &
Tuple2::operator+=(const Tuple2 &rhs)
{
	m_value[0] += rhs.m_value[0];
	m_value[1] += rhs.m_value[1];
	return(*this);
}

Tuple2 &
Tuple2::operator-=(const Tuple2 &rhs)
{
	m_value[0] -= rhs.m_value[0];
	m_value[1] -= rhs.m_value[1];
	return(*this);
}

Tuple2
Tuple2::operator*(float rhs) const
{
	return(Tuple2(m_value[0] * rhs, m_value[1] * rhs));
}

Tuple2
Tuple2::operator+(float rhs) const
{
	return(Tuple2(m_value[0] + rhs, m_value[1] + rhs));
}

Tuple2
Tuple2::operator-(float rhs) const
{
	return(Tuple2(m_value[0] - rhs, m_value[1] - rhs));
}

Tuple2
Tuple2::operator*(const Tuple2 &rhs) const
{
	return(Tuple2(m_value[0] * rhs[0], m_value[1] * rhs[1]));
}

Tuple2
Tuple2::operator+(const Tuple2 &rhs) const
{
	return(Tuple2(m_value[0] + rhs.m_value[0],
	              m_value[1] + rhs.m_value[1]));
}

Tuple2
Tuple2::operator-(const Tuple2 &rhs) const
{
	return(Tuple2(m_value[0] - rhs.m_value[0],
	              m_value[1] - rhs.m_value[1]));
}

} /*********************************************************** Math Namespace */
MARSHMALLOW_NAMESPACE_END

