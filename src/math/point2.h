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

#ifndef MATH_POINT2_H
#define MATH_POINT2_H 1

#include "core/environment.h"
#include "core/namespace.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Math
{
	class Vector2;

	/*! @brief 2D Point */
	class MATH_EXPORT Point2
	{
		float m_value[2];

	public:
		Point2(float x = 0.f, float y = 0.f);
		Point2(const Point2 &copy);

		const float & x(void) const
		    { return(m_value[0]); }
		const float & y(void) const
		    { return(m_value[1]); }

		Vector2 difference(const Point2 &rhs) const;

	public: /* operators */

		Point2 & operator=(const Point2 &rhs);
		bool operator==(const Point2 &rhs) const;

		float & operator[](int i)
		    { return(m_value[i % 2]); }

		const float & operator[](int i) const
		    { return(m_value[i % 2]); }

		operator bool(void) const
		    { return(m_value[0] || m_value[1]); }

		operator Vector2(void) const;

		Point2 & operator*=(float rhs);
		Point2 & operator-=(float rhs);
		Point2 & operator+=(float rhs);

		Point2 & operator*=(const Point2 &rhs);
		Point2 & operator+=(const Point2 &rhs);
		Point2 & operator-=(const Point2 &rhs);

		Point2 & operator*=(const Vector2 &rhs);
		Point2 & operator+=(const Vector2 &rhs);
		Point2 & operator-=(const Vector2 &rhs);

		Point2 operator*(float rhs) const;
		Point2 operator+(float rhs) const;
		Point2 operator-(float rhs) const;

		Point2 operator*(const Point2 &rhs) const;
		Point2 operator+(const Point2 &rhs) const;
		Point2 operator-(const Point2 &rhs) const;

		Point2 operator*(const Vector2 &rhs) const;
		Point2 operator+(const Vector2 &rhs) const;
		Point2 operator-(const Vector2 &rhs) const;

	public: /* static */

		static const Point2 & Zero(void)
		    { static Point2 s_zero(0.f, 0.f);
		      return(s_zero); }

		static const Point2 & One(void)
		    { static Point2 s_one(1.f, 1.f);
		      return(s_one); }
	};
}

MARSHMALLOW_NAMESPACE_END

#endif
