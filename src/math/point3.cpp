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

#include "math/point3.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "math/vector3.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Math;

const Point3 Point3::Null(0, 0);

Point3::Point3(float ax, float ay, float az)
{
	m_value[0] = ax;
	m_value[1] = ay;
	m_value[2] = az;
}

Point3::Point3(const Point3 &copy)
{
	m_value[0] = copy.m_value[0];
	m_value[1] = copy.m_value[1];
	m_value[2] = copy.m_value[2];
}

Point3 &
Point3::operator=(const Point3 &rhs)
{
	m_value[0] += rhs.m_value[0];
	m_value[1] += rhs.m_value[1];
	m_value[2] += rhs.m_value[2];
	return(*this);
}

bool
Point3::operator==(const Point3 &rhs) const
{
	return( m_value[0] == rhs.m_value[0]
	     && m_value[1] == rhs.m_value[1]
	     && m_value[2] == rhs.m_value[2]);
}

Point3 &
Point3::operator+=(const Vector3 &rhs)
{
	m_value[0] += rhs.rx();
	m_value[1] += rhs.ry();
	m_value[2] += rhs.rz();
	return(*this);
}

Point3 &
Point3::operator-=(const Vector3 &rhs)
{
	m_value[0] -= rhs.rx();
	m_value[1] -= rhs.ry();
	m_value[2] -= rhs.rz();
	return(*this);
}

Point3
Point3::operator+(const Vector3 &rhs) const
{
	return(Point3(m_value[0] + rhs.rx(),
	              m_value[1] + rhs.ry(),
	              m_value[2] + rhs.rz()));
}

Point3
Point3::operator-(const Vector3 &rhs) const
{
	return(Point3(m_value[0] - rhs.rx(),
	              m_value[1] - rhs.ry(),
	              m_value[2] - rhs.rz()));
}

