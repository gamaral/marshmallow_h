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
	if (v) memcpy(&m_value[0], v, mmm * sizeof(float));
	else identity();
}

void
Matrix4::identity(void)
{
	m_value[m12] = m_value[m13] = m_value[m14] = m_value[m21] =
	m_value[m23] = m_value[m24] = m_value[m31] = m_value[m32] =
	m_value[m34] = m_value[m41] = m_value[m42] = m_value[m43] = 0.f;

	m_value[m11] = m_value[m22] = m_value[m33] = m_value[m44] = 1.f;
}

bool
Matrix4::operator==(const Matrix4 &rhs) const
{
	return((m_value[m11] == rhs.m_value[m11]) && (m_value[m12] == rhs.m_value[m12]) &&
	       (m_value[m13] == rhs.m_value[m13]) && (m_value[m14] == rhs.m_value[m14]) &&

	       (m_value[m21] == rhs.m_value[m21]) && (m_value[m22] == rhs.m_value[m22]) &&
	       (m_value[m23] == rhs.m_value[m23]) && (m_value[m24] == rhs.m_value[m24]) &&

	       (m_value[m31] == rhs.m_value[m31]) && (m_value[m32] == rhs.m_value[m32]) &&
	       (m_value[m33] == rhs.m_value[m33]) && (m_value[m34] == rhs.m_value[m34]) &&

	       (m_value[m41] == rhs.m_value[m41]) && (m_value[m42] == rhs.m_value[m42]) &&
	       (m_value[m43] == rhs.m_value[m43]) && (m_value[m44] == rhs.m_value[m44]));
}

Matrix4
Matrix4::operator*(const Matrix4 &rhs) const
{
	/* multiplication: 'this' is A, 'rhs' is B */

	const float *l_A = &m_value[m11];
	const float *l_B = &rhs.m_value[m11];
	float l_R[mmm];

	l_R[m11] = (l_A[m11] * l_B[m11]) + (l_A[m12] * l_B[m21]) + (l_A[m13] * l_B[m31]) + (l_A[m14] * l_B[m41]);
	l_R[m12] = (l_A[m11] * l_B[m12]) + (l_A[m12] * l_B[m22]) + (l_A[m13] * l_B[m32]) + (l_A[m14] * l_B[m42]);
	l_R[m13] = (l_A[m11] * l_B[m13]) + (l_A[m12] * l_B[m23]) + (l_A[m13] * l_B[m33]) + (l_A[m14] * l_B[m43]);
	l_R[m14] = (l_A[m11] * l_B[m14]) + (l_A[m12] * l_B[m24]) + (l_A[m13] * l_B[m34]) + (l_A[m14] * l_B[m44]);

	l_R[m21] = (l_A[m21] * l_B[m11]) + (l_A[m22] * l_B[m21]) + (l_A[m23] * l_B[m31]) + (l_A[m24] * l_B[m41]);
	l_R[m22] = (l_A[m21] * l_B[m12]) + (l_A[m22] * l_B[m22]) + (l_A[m23] * l_B[m32]) + (l_A[m24] * l_B[m42]);
	l_R[m23] = (l_A[m21] * l_B[m13]) + (l_A[m22] * l_B[m23]) + (l_A[m23] * l_B[m33]) + (l_A[m24] * l_B[m43]);
	l_R[m24] = (l_A[m21] * l_B[m14]) + (l_A[m22] * l_B[m24]) + (l_A[m23] * l_B[m34]) + (l_A[m24] * l_B[m44]);

	l_R[m31] = (l_A[m31] * l_B[m11]) + (l_A[m32] * l_B[m21]) + (l_A[m33] * l_B[m31]) + (l_A[m34] * l_B[m41]);
	l_R[m32] = (l_A[m31] * l_B[m12]) + (l_A[m32] * l_B[m22]) + (l_A[m33] * l_B[m32]) + (l_A[m34] * l_B[m42]);
	l_R[m33] = (l_A[m31] * l_B[m13]) + (l_A[m32] * l_B[m23]) + (l_A[m33] * l_B[m33]) + (l_A[m34] * l_B[m43]);
	l_R[m34] = (l_A[m31] * l_B[m14]) + (l_A[m32] * l_B[m24]) + (l_A[m33] * l_B[m34]) + (l_A[m34] * l_B[m44]);

	l_R[m41] = (l_A[m41] * l_B[m11]) + (l_A[m42] * l_B[m21]) + (l_A[m43] * l_B[m31]) + (l_A[m44] * l_B[m41]);
	l_R[m42] = (l_A[m41] * l_B[m12]) + (l_A[m42] * l_B[m22]) + (l_A[m43] * l_B[m32]) + (l_A[m44] * l_B[m42]);
	l_R[m43] = (l_A[m41] * l_B[m13]) + (l_A[m42] * l_B[m23]) + (l_A[m43] * l_B[m33]) + (l_A[m44] * l_B[m43]);
	l_R[m44] = (l_A[m41] * l_B[m14]) + (l_A[m42] * l_B[m24]) + (l_A[m43] * l_B[m34]) + (l_A[m44] * l_B[m44]);

	return(Matrix4(l_R));
}

Matrix4 &
Matrix4::operator*=(const Matrix4 &rhs)
{
	*this = *this * rhs;
	return(*this);
}

