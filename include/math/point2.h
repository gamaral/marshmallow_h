/*
 * Copyright 2011-2012 Marshmallow Engine. All rights reserved.
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

#ifndef MARSHMALLOW_MATH_POINT2_H
#define MARSHMALLOW_MATH_POINT2_H 1

#include <core/environment.h>
#include <core/namespace.h>

#include <math/vector2.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Math { /******************************************** Math Namespace */

	struct Vector2;

	/*! @brief 2D Point */
	struct Point2
	{
		enum Position { X, Y, MAX };

		/*
		 * direct accessors
		 */
		float x, y;

	public:
		inline Point2(float x = 0.f, float y = 0.f);
		inline explicit Point2(float *data);

		inline Point2 & set(float x, float y);
		inline Vector2 difference(const Point2 &rhs) const;

	public: /* operators */

		inline bool operator==(const Point2 &rhs) const
		    { return(x == rhs.x && y == rhs.y); }

		inline float & operator[](int i)
		    { return(i % MAX ? y : x); }

		inline const float & operator[](int i) const
		    { return(i % MAX ? y : x); }

		inline operator bool(void) const
		    { return(x != .0f || y != .0f); }

		inline operator Vector2(void) const;

		inline Point2 & operator*=(float rhs);
		inline Point2 & operator-=(float rhs);
		inline Point2 & operator+=(float rhs);

		inline Point2 & operator*=(const Point2 &rhs);
		inline Point2 & operator+=(const Point2 &rhs);
		inline Point2 & operator-=(const Point2 &rhs);

		inline Point2 & operator*=(const Vector2 &rhs);
		inline Point2 & operator+=(const Vector2 &rhs);
		inline Point2 & operator-=(const Vector2 &rhs);

		inline Point2 operator*(float rhs) const;
		inline Point2 operator+(float rhs) const;
		inline Point2 operator-(float rhs) const;

		inline Point2 operator*(const Point2 &rhs) const;
		inline Point2 operator+(const Point2 &rhs) const;
		inline Point2 operator-(const Point2 &rhs) const;

		inline Point2 operator*(const Vector2 &rhs) const;
		inline Point2 operator+(const Vector2 &rhs) const;
		inline Point2 operator-(const Vector2 &rhs) const;

	public: /* static */

		static const Point2 & Zero(void)
		    { static Point2 s_zero(0.f, 0.f);
		      return(s_zero); }

		static const Point2 & One(void)
		    { static Point2 s_one(1.f, 1.f);
		      return(s_one); }
	};

	Point2::Point2(float ax, float ay)
	    : x(ax), y(ay) {}

	Point2::Point2(float *data)
	    : x(data[X]), y(data[Y]) {}

	Point2 &
	Point2::set(float x_, float y_)
	{
		x = x_, y = y_;
		return(*this);
	}

	Vector2
	Point2::difference(const Point2 &rhs) const
	{
		return(Vector2(rhs.x - x, rhs.y - y));
	}

	Point2 &
	Point2::operator*=(float rhs)
	{
		x *= rhs, y *= rhs;
		return(*this);
	}

	Point2 &
	Point2::operator+=(float rhs)
	{
		x += rhs, y += rhs;
		return(*this);
	}

	Point2 &
	Point2::operator-=(float rhs)
	{
		x -= rhs, y -= rhs;
		return(*this);
	}

	Point2 &
	Point2::operator*=(const Point2 &rhs)
	{
		x *= rhs.x, y *= rhs.y;
		return(*this);
	}

	Point2 &
	Point2::operator+=(const Point2 &rhs)
	{
		x += rhs.x, y += rhs.y;
		return(*this);
	}

	Point2 &
	Point2::operator-=(const Point2 &rhs)
	{
		x -= rhs.x, y -= rhs.y;
		return(*this);
	}

	Point2 &
	Point2::operator*=(const Vector2 &rhs)
	{
		x *= rhs.x, y *= rhs.y;
		return(*this);
	}

	Point2 &
	Point2::operator+=(const Vector2 &rhs)
	{
		x += rhs.x, y += rhs.y;
		return(*this);
	}

	Point2 &
	Point2::operator-=(const Vector2 &rhs)
	{
		x -= rhs.x, y -= rhs.y;
		return(*this);
	}

	Point2
	Point2::operator*(float rhs) const
	{
		return(Point2(x * rhs, y * rhs));
	}

	Point2
	Point2::operator+(float rhs) const
	{
		return(Point2(x + rhs, y + rhs));
	}

	Point2
	Point2::operator-(float rhs) const
	{
		return(Point2(x - rhs, y - rhs));
	}

	Point2
	Point2::operator*(const Point2 &rhs) const
	{
		return(Point2(x * rhs.x, y * rhs.y));
	}

	Point2
	Point2::operator+(const Point2 &rhs) const
	{
		return(Point2(x + rhs.x, y + rhs.y));
	}

	Point2
	Point2::operator-(const Point2 &rhs) const
	{
		return(Point2(x - rhs.x, y - rhs.y));
	}

	Point2
	Point2::operator*(const Vector2 &rhs) const
	{
		return(Point2(x * rhs.x, y * rhs.y));
	}

	Point2
	Point2::operator+(const Vector2 &rhs) const
	{
		return(Point2(x + rhs.x, y + rhs.y));
	}

	Point2
	Point2::operator-(const Vector2 &rhs) const
	{
		return(Point2(x - rhs.x, y - rhs.y));
	}

	Point2::operator Vector2(void) const
	{
		return(Vector2(x, y));
	}

} /*********************************************************** Math Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
