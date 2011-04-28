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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef MATH_VECTOR3_H
#define MATH_VECTOR3_H 1

#include "core/global.h"

struct b2Vec3;

MARSHMALLOW_NAMESPACE_BEGIN

namespace Math
{

	/*! @brief 3D Vector */
	class MATH_EXPORT Vector3
	{
		float m_value[3];

	public:
		Vector3(float x = 0., float y = 0., float z = 0.);
		Vector3(const Vector3 &copy);

		float & rx(void)
		    { return(m_value[0]); }
		float & ry(void)
		    { return(m_value[1]); }
		float & rz(void)
		    { return(m_value[2]); }

		const float & rx(void) const
		    { return(m_value[0]); }
		const float & ry(void) const
		    { return(m_value[1]); }
		const float & rz(void) const
		    { return(m_value[2]); }

		float x(void) const
		    { return(m_value[0]); }
		float y(void) const
		    { return(m_value[1]); }
		float z(void) const
		    { return(m_value[2]); }

		float & operator[](int i)
		    { return(m_value[i % 3]); }

		Vector3 normalized(void) const;
		Vector3 & normalize(void);

		float magnitude(void) const;
		float magnitude2(void) const;

	public: /* operators */

		operator bool(void) const
		    { return(m_value[0] || m_value[1] || m_value[2]); }
		operator b2Vec3(void) const;

		bool operator==(const Vector3 &rhs) const
		    { return(m_value[0] == rhs.m_value[0] &&
		             m_value[1] == rhs.m_value[1] &&
		             m_value[2] == rhs.m_value[2]); }

		Vector3 & operator+=(const Vector3 &rhs);
		Vector3 & operator-=(const Vector3 &rhs);
		Vector3 operator+(const Vector3 &rhs) const;
		Vector3 operator-(const Vector3 &rhs) const;

	public: /* static */

		static const Vector3 Null;
		static const Vector3 One;
	};

}

MARSHMALLOW_NAMESPACE_END

#endif
