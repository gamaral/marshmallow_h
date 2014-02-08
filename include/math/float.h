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

#ifndef MARSHMALLOW_MATH_FLOAT_H
#define MARSHMALLOW_MATH_FLOAT_H 1

#include <core/environment.h>
#include <core/namespace.h>

#include <cmath>
#include <limits>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Math { /******************************************** Math Namespace */

	/*! @brief Float Wrapper */
	struct Float
	{
		/*
		 * direct accessors
		 */
		float value;

	public:
		inline Float(float value = .0f);
		inline Float(const Float &copy);

	public: /* operators */

		inline bool operator==(float rhs) const;
		inline bool operator!=(float rhs) const;

		inline bool operator==(const Float &rhs) const;
		inline bool operator!=(const Float &rhs) const;

		inline operator float(void) const
		    { return value; }

		inline operator bool(void) const;

		inline operator Float(void) const;

		inline Float & operator/=(float rhs);
		inline Float & operator*=(float rhs);
		inline Float & operator-=(float rhs);
		inline Float & operator+=(float rhs);

		inline Float & operator/=(const Float &rhs);
		inline Float & operator*=(const Float &rhs);
		inline Float & operator+=(const Float &rhs);
		inline Float & operator-=(const Float &rhs);

		inline Float operator/(float rhs) const;
		inline Float operator*(float rhs) const;
		inline Float operator+(float rhs) const;
		inline Float operator-(float rhs) const;

		inline Float operator/(const Float &rhs) const;
		inline Float operator*(const Float &rhs) const;
		inline Float operator+(const Float &rhs) const;
		inline Float operator-(const Float &rhs) const;

	public: /* static */

		static const Float & Zero(void)
		    { static Float s_zero(.0f);
		      return(s_zero); }

		static const Float & One(void)
		    { static Float s_one(1.f);
		      return(s_one); }
	};

	Float::Float(float _value)
	    : value(_value) {}

	Float::Float(const Float &copy)
	    : value(copy.value) {}

	bool
	Float::operator==(float rhs) const
	{
		static const float e = std::numeric_limits<float>::epsilon();
		return(std::fabs(value - rhs) < e);
	}

	bool
	Float::operator!=(float rhs) const
	{
		static const float e = std::numeric_limits<float>::epsilon();
		return(std::fabs(value - rhs) > e);
	}

	bool
	Float::operator==(const Float &rhs) const
	{
		static const float e = std::numeric_limits<float>::epsilon();
		return(std::fabs(value - rhs.value) < e);
	}

	bool
	Float::operator!=(const Float &rhs) const
	{
		static const float e = std::numeric_limits<float>::epsilon();
		return(std::fabs(value - rhs.value) > e);
	}

	Float::operator bool(void) const
	{
		static const float e = std::numeric_limits<float>::epsilon();
		return (value > e);
	}

	Float &
	Float::operator/=(float rhs)
	{
		value /= rhs;
		return(*this);
	}

	Float &
	Float::operator*=(float rhs)
	{
		value *= rhs;
		return(*this);
	}

	Float &
	Float::operator+=(float rhs)
	{
		value += rhs;
		return(*this);
	}

	Float &
	Float::operator-=(float rhs)
	{
		value -= rhs;
		return(*this);
	}

	Float &
	Float::operator/=(const Float &rhs)
	{
		value /= rhs.value;
		return(*this);
	}

	Float &
	Float::operator*=(const Float &rhs)
	{
		value *= rhs.value;
		return(*this);
	}

	Float &
	Float::operator+=(const Float &rhs)
	{
		value += rhs.value;
		return(*this);
	}

	Float &
	Float::operator-=(const Float &rhs)
	{
		value -= rhs.value;
		return(*this);
	}

	Float
	Float::operator/(float rhs) const
	{
		return(Float(value / rhs));
	}

	Float
	Float::operator*(float rhs) const
	{
		return(Float(value * rhs));
	}

	Float
	Float::operator+(float rhs) const
	{
		return(Float(value + rhs));
	}

	Float
	Float::operator-(float rhs) const
	{
		return(Float(value - rhs));
	}

	Float
	Float::operator/(const Float &rhs) const
	{
		return(Float(value / rhs.value));
	}

	Float
	Float::operator*(const Float &rhs) const
	{
		return(Float(value * rhs.value));
	}

	Float
	Float::operator+(const Float &rhs) const
	{
		return(Float(value + rhs.value));
	}

	Float
	Float::operator-(const Float &rhs) const
	{
		return(Float(value - rhs.value));
	}

} /*********************************************************** Math Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
