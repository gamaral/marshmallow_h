/*
 * Copyright (c) 2011-2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
 * All rights reserved.
 *
 * This file is part of Marshmallow Game Engine.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the project as a whole.
 */

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef MARSHMALLOW_MATH_VECTOR2_H
#define MARSHMALLOW_MATH_VECTOR2_H 1

#include <core/environment.h>
#include <core/namespace.h>

#include <cmath>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Math { /******************************************** Math Namespace */

	/*! @brief 2D Vector */
	struct MARSHMALLOW_MATH_EXPORT
	Vector2
	{
		enum Position { X, Y, MAX };

		/*
		 * direct accessors
		 */
		float x, y;

	public:

		inline Vector2(float x, float y);
		inline explicit Vector2(float *data);
		inline explicit Vector2(float value = 0.f);

		inline Vector2 & set(float x, float y);

		inline Vector2 normalized(float magnitude) const;
		inline Vector2 & normalize(float magnitude);

		inline float magnitude(void) const;
		inline float magnitude2(void) const;

		inline float cross(const Vector2 &b) const;
		inline float dot(const Vector2 &b) const;

	public: /* operators */

		inline bool operator==(const Vector2 &rhs) const
		    { return(x == rhs.x && y == rhs.y); }

		inline float & operator[](int i)
		    { return(i % MAX ? y : x); }

		inline const float & operator[](int i) const
		    { return(i % MAX ? y : x); }

		inline operator bool(void) const
		    { return(x != .0f || y != .0f); }

		inline Vector2 & operator*=(float rhs);
		inline Vector2 & operator-=(float rhs);
		inline Vector2 & operator+=(float rhs);

		inline Vector2 & operator*=(const Vector2 &rhs);
		inline Vector2 & operator+=(const Vector2 &rhs);
		inline Vector2 & operator-=(const Vector2 &rhs);

		inline Vector2 operator*(float rhs) const;
		inline Vector2 operator+(float rhs) const;
		inline Vector2 operator-(float rhs) const;

		inline Vector2 operator*(const Vector2 &rhs) const;
		inline Vector2 operator+(const Vector2 &rhs) const;
		inline Vector2 operator-(const Vector2 &rhs) const;

	public: /* static */

		static const Vector2 & Zero(void)
		    { static Vector2 s_zero(0.f, 0.f);
		      return(s_zero); }

		static const Vector2 & Identity(void)
		    { static Vector2 s_identity(1.f, 1.f);
		      return(s_identity); }
	};

	Vector2::Vector2(float ax, float ay)
	    : x(ax), y(ay) {}

	Vector2::Vector2(float value)
	    : x(value), y(value) {}

	Vector2::Vector2(float *data)
	    : x(data[X]), y(data[Y]) {}

	Vector2 &
	Vector2::set(float x_, float y_)
	{
		x = x_, y = y_;
		return(*this);
	}

	Vector2
	Vector2::normalized(float m) const
	{
		return(Vector2(m != .0f ? x / m : .0f,
		               m != .0f ? y / m : .0f));
	}

	Vector2 &
	Vector2::normalize(float m)
	{
		if (m != 0) x /= m, y /= m;
		else x = y = .0f;

		return(*this);
	}

	float
	Vector2::magnitude(void) const
	{
		return(sqrtf(powf(x, 2) + powf(y, 2)));
	}

	float
	Vector2::magnitude2(void) const
	{
		return(powf(x, 2) + powf(y, 2));
	}

	float
	Vector2::cross(const Vector2 &b) const
	{
		return((x * b.y) - (y * b.x));
	}

	float
	Vector2::dot(const Vector2 &b) const
	{
		return((x * b.x) + (y * b.y));
	}

	Vector2 &
	Vector2::operator*=(float rhs)
	{
		x *= rhs, y *= rhs;
		return(*this);
	}

	Vector2 &
	Vector2::operator+=(float rhs)
	{
		x += rhs, y += rhs;
		return(*this);
	}

	Vector2 &
	Vector2::operator-=(float rhs)
	{
		x -= rhs, y -= rhs;
		return(*this);
	}

	Vector2 &
	Vector2::operator*=(const Vector2 &rhs)
	{
		x *= rhs.x, y *= rhs.y;
		return(*this);
	}

	Vector2 &
	Vector2::operator+=(const Vector2 &rhs)
	{
		x += rhs.x, y += rhs.y;
		return(*this);
	}

	Vector2 &
	Vector2::operator-=(const Vector2 &rhs)
	{
		x -= rhs.x, y -= rhs.y;
		return(*this);
	}

	Vector2
	Vector2::operator*(float rhs) const
	{
		return(Vector2(x * rhs, y * rhs));
	}

	Vector2
	Vector2::operator+(float rhs) const
	{
		return(Vector2(x + rhs, y + rhs));
	}

	Vector2
	Vector2::operator-(float rhs) const
	{
		return(Vector2(x - rhs, y - rhs));
	}

	Vector2
	Vector2::operator*(const Vector2 &rhs) const
	{
		return(Vector2(x * rhs.x, y * rhs.y));
	}

	Vector2
	Vector2::operator+(const Vector2 &rhs) const
	{
		return(Vector2(x + rhs.x, y + rhs.y));
	}

	Vector2
	Vector2::operator-(const Vector2 &rhs) const
	{
		return(Vector2(x - rhs.x, y - rhs.y));
	}

} /*********************************************************** Math Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
