/*
 * Copyright 2012 Marshmallow Engine. All rights reserved.
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

#include "math/matrix4.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <cstring>

MARSHMALLOW_NAMESPACE_USE
using namespace Math;

Matrix4::Matrix4(const float *v)
{
	if (v) memcpy(&m_value[0], v, 16 * sizeof(float));
	else identity();
}

Matrix4::Matrix4(const Matrix4 &c)
{
	memcpy(&m_value, &c.m_value, 16 * sizeof(float));
}

void
Matrix4::identity(void)
{
	m_value[ 1] = m_value[ 2] = m_value[ 3] = m_value[ 4] =
	m_value[ 6] = m_value[ 7] = m_value[ 8] = m_value[ 9] =
	m_value[11] = m_value[12] = m_value[13] = m_value[14] = 0.f;

	m_value[ 0] = m_value[ 5] = m_value[10] = m_value[15] = 1.f;
}

Matrix4 &
Matrix4::operator=(const Matrix4 &rhs)
{
	memcpy(&m_value, &rhs.m_value, 16 * sizeof(float));
	return(*this);
}

bool
Matrix4::operator==(const Matrix4 &rhs) const
{
	return((m_value[ 0] == rhs.m_value[ 0]) && (m_value[ 1] == rhs.m_value[ 1]) &&
	       (m_value[ 2] == rhs.m_value[ 2]) && (m_value[ 3] == rhs.m_value[ 3]) &&
	       (m_value[ 4] == rhs.m_value[ 4]) && (m_value[ 5] == rhs.m_value[ 5]) &&
	       (m_value[ 6] == rhs.m_value[ 6]) && (m_value[ 7] == rhs.m_value[ 7]) &&
	       (m_value[ 8] == rhs.m_value[ 8]) && (m_value[ 9] == rhs.m_value[ 9]) &&
	       (m_value[10] == rhs.m_value[10]) && (m_value[11] == rhs.m_value[11]) &&
	       (m_value[12] == rhs.m_value[12]) && (m_value[13] == rhs.m_value[13]) &&
	       (m_value[14] == rhs.m_value[14]) && (m_value[15] == rhs.m_value[15]));
}

Matrix4
Matrix4::operator*(const Matrix4 &rhs) const
{
	/* multiplication reversed: this is B, rhs is A */

	float l_value[16];

	l_value[ 0] = (m_value[ 0] * rhs.m_value[ 0]) + (m_value[ 4] * rhs.m_value[ 1]) + (m_value[ 8] * rhs.m_value[ 2]) + (m_value[12] * rhs.m_value[ 3]);
	l_value[ 1] = (m_value[ 1] * rhs.m_value[ 0]) + (m_value[ 5] * rhs.m_value[ 1]) + (m_value[ 9] * rhs.m_value[ 2]) + (m_value[13] * rhs.m_value[ 3]);
	l_value[ 2] = (m_value[ 2] * rhs.m_value[ 0]) + (m_value[ 6] * rhs.m_value[ 1]) + (m_value[10] * rhs.m_value[ 2]) + (m_value[14] * rhs.m_value[ 3]);
	l_value[ 3] = (m_value[ 3] * rhs.m_value[ 0]) + (m_value[ 7] * rhs.m_value[ 1]) + (m_value[11] * rhs.m_value[ 2]) + (m_value[15] * rhs.m_value[ 3]);

	l_value[ 4] = (m_value[ 0] * rhs.m_value[ 4]) + (m_value[ 4] * rhs.m_value[ 5]) + (m_value[ 8] * rhs.m_value[ 6]) + (m_value[12] * rhs.m_value[ 7]);
	l_value[ 5] = (m_value[ 1] * rhs.m_value[ 4]) + (m_value[ 5] * rhs.m_value[ 5]) + (m_value[ 9] * rhs.m_value[ 6]) + (m_value[13] * rhs.m_value[ 7]);
	l_value[ 6] = (m_value[ 2] * rhs.m_value[ 4]) + (m_value[ 6] * rhs.m_value[ 5]) + (m_value[10] * rhs.m_value[ 6]) + (m_value[14] * rhs.m_value[ 7]);
	l_value[ 7] = (m_value[ 3] * rhs.m_value[ 4]) + (m_value[ 7] * rhs.m_value[ 5]) + (m_value[11] * rhs.m_value[ 6]) + (m_value[15] * rhs.m_value[ 7]);

	l_value[ 8] = (m_value[ 0] * rhs.m_value[ 8]) + (m_value[ 4] * rhs.m_value[ 9]) + (m_value[ 8] * rhs.m_value[10]) + (m_value[12] * rhs.m_value[11]);
	l_value[ 9] = (m_value[ 1] * rhs.m_value[ 8]) + (m_value[ 5] * rhs.m_value[ 9]) + (m_value[ 9] * rhs.m_value[10]) + (m_value[13] * rhs.m_value[11]);
	l_value[10] = (m_value[ 2] * rhs.m_value[ 8]) + (m_value[ 6] * rhs.m_value[ 9]) + (m_value[10] * rhs.m_value[10]) + (m_value[14] * rhs.m_value[11]);
	l_value[11] = (m_value[ 3] * rhs.m_value[ 8]) + (m_value[ 7] * rhs.m_value[ 9]) + (m_value[11] * rhs.m_value[10]) + (m_value[15] * rhs.m_value[11]);

	l_value[12] = (m_value[ 0] * rhs.m_value[12]) + (m_value[ 4] * rhs.m_value[13]) + (m_value[ 8] * rhs.m_value[14]) + (m_value[12] * rhs.m_value[15]);
	l_value[13] = (m_value[ 1] * rhs.m_value[12]) + (m_value[ 5] * rhs.m_value[13]) + (m_value[ 9] * rhs.m_value[14]) + (m_value[13] * rhs.m_value[15]);
	l_value[14] = (m_value[ 2] * rhs.m_value[12]) + (m_value[ 6] * rhs.m_value[13]) + (m_value[10] * rhs.m_value[14]) + (m_value[14] * rhs.m_value[15]);
	l_value[15] = (m_value[ 3] * rhs.m_value[12]) + (m_value[ 7] * rhs.m_value[13]) + (m_value[11] * rhs.m_value[14]) + (m_value[15] * rhs.m_value[15]);

	return(Matrix4(l_value));
}

Matrix4 &
Matrix4::operator*=(const Matrix4 &rhs)
{
	*this = (*this * rhs);
	return(*this);
}

