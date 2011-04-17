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

#include "math/vector2.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <math.h>

MARSHMALLOW_NAMESPACE_USE;
using namespace Math;

Vector2::Vector2(double ax, double ay)
{
	m_value[0] = ax;
	m_value[1] = ay;
}

Vector2::Vector2(const Vector2 &copy)
{
	m_value[0] = copy.m_value[0];
	m_value[1] = copy.m_value[1];
}

Vector2
Vector2::normalized(void) const
{
	Vector2 n(*this);
	n.normalize();
	return(n);
}

Vector2 &
Vector2::normalize(void)
{
	const float m = magnitude();
	m_value[0] /= m;
	m_value[1] /= m;
	return(*this);
}

double
Vector2::magnitude(void)
{
	return(sqrt(pow(m_value[0], 2)
	          + pow(m_value[1], 2)));
}

double
Vector2::magnitude2(void)
{
	return(pow(m_value[0], 2)
	     + pow(m_value[1], 2));
}

double
Vector2::cross(const Vector2 &b) const
{
	return((rx() * b.ry()) - (ry() * b.rx()));
}

double
Vector2::dot(const Vector2 &b) const
{
	return((rx() * b.rx()) + (ry() * b.ry()));
}

Vector2 &
Vector2::operator+=(const Vector2 &rhs)
{
	m_value[0] += rhs.m_value[0];
	m_value[1] += rhs.m_value[1];
	return(*this);
}

Vector2 &
Vector2::operator-=(const Vector2 &rhs)
{
	m_value[0] -= rhs.m_value[0];
	m_value[1] -= rhs.m_value[1];
	return(*this);
}

Vector2
Vector2::operator+(const Vector2 &rhs) const
{
	return(Vector2(m_value[0] + rhs.m_value[0],
	               m_value[1] + rhs.m_value[1]));
}

Vector2
Vector2::operator-(const Vector2 &rhs) const
{
	return(Vector2(m_value[0] - rhs.m_value[0],
	               m_value[1] - rhs.m_value[1]));
}

