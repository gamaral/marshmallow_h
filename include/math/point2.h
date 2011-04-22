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

#include "core/global.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Math
{

	class Vector2;

	/*! @brief 2D Point */
	class MATH_EXPORT Point2
	{
		float m_value[2];

	public:
		Point2(float x = 0., float y = 0.);
		Point2(const Point2 &copy);

		float & rx(void)
		    { return(m_value[0]); }
		float & ry(void)
		    { return(m_value[1]); }

		const float & rx(void) const
		    { return(m_value[0]); }
		const float & ry(void) const
		    { return(m_value[1]); }

		float x(void) const
		    { return(m_value[0]); }
		float y(void) const
		    { return(m_value[1]); }

		float & operator[](int i)
		    { return(m_value[i % 2]); }

		Vector2 difference(const Point2 &rhs) const;

	public: /* operators */

		Point2 & operator+=(const Vector2 &rhs);
		Point2 & operator-=(const Vector2 &rhs);
		Point2 operator+(const Vector2 &rhs) const;
		Point2 operator-(const Vector2 &rhs) const;

	public: /* static */

		static const Point2 Null;

	};

}

MARSHMALLOW_NAMESPACE_END

#endif
