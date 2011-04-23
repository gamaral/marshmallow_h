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

#include "math/vector3.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <math.h>

MARSHMALLOW_NAMESPACE_USE;
using namespace Math;

const Vector3 Vector3::Null(0, 0, 0);
const Vector3 Vector3::One(1, 1, 1);

Vector3::Vector3(float ax, float ay, float az)
{
	m_value[0] = ax;
	m_value[1] = ay;
	m_value[2] = az;
}

Vector3::Vector3(const Vector3 &copy)
{
	m_value[0] = copy.m_value[0];
	m_value[1] = copy.m_value[1];
	m_value[2] = copy.m_value[2];
}

Vector3
Vector3::normalized(void) const
{
	Vector3 n(*this);
	n.normalize();
	return(n);
}

Vector3 &
Vector3::normalize(void)
{
	const float m = magnitude();
	m_value[0] /= m;
	m_value[1] /= m;
	m_value[2] /= m;
	return(*this);
}

float
Vector3::magnitude(void) const
{
	return(sqrtf(powf(m_value[0], 2)
	           + powf(m_value[1], 2)
	           + powf(m_value[2], 2)));
}

float
Vector3::magnitude2(void) const
{
	return(powf(m_value[0], 2)
	     + powf(m_value[1], 2)
	     + powf(m_value[2], 2));
}

Vector3 &
Vector3::operator+=(const Vector3 &rhs)
{
	m_value[0] += rhs.m_value[0];
	m_value[1] += rhs.m_value[1];
	m_value[2] += rhs.m_value[2];
	return(*this);
}

Vector3 &
Vector3::operator-=(const Vector3 &rhs)
{
	m_value[0] -= rhs.m_value[0];
	m_value[1] -= rhs.m_value[1];
	m_value[2] -= rhs.m_value[2];
	return(*this);
}

Vector3
Vector3::operator+(const Vector3 &rhs) const
{
	return(Vector3(m_value[0] + rhs.m_value[0],
	               m_value[1] + rhs.m_value[1],
	               m_value[2] + rhs.m_value[2]));
}

Vector3
Vector3::operator-(const Vector3 &rhs) const
{
	return(Vector3(m_value[0] - rhs.m_value[0],
	               m_value[1] - rhs.m_value[1],
	               m_value[2] - rhs.m_value[2]));
}

