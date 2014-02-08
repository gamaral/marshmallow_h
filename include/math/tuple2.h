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

#ifndef MARSHMALLOW_MATH_TUPLE2_H
#define MARSHMALLOW_MATH_TUPLE2_H 1

#include <core/environment.h>
#include <core/global.h>
#include <core/namespace.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Math { /******************************************** Math Namespace */

	/*! @brief 2D Tuple */
	class MARSHMALLOW_MATH_EXPORT
	Tuple2
	{
		float m_value[2];
	public:

		Tuple2(float first = .0f, float second = .0f);

		const float & first(void) const
		    { return(m_value[0]); }
		const float & second(void) const
		    { return(m_value[1]); }

		inline void set(float a, float b)
		    { m_value[0] = a, m_value[1] = b; }

	public: /* operators */

		bool operator==(const Tuple2 &rhs) const;

		float & operator[](int i)
		    { return(m_value[i % 2]); }

		const float & operator[](int i) const
		    { return(m_value[i % 2]); }

		operator bool(void) const;

		Tuple2 & operator*=(float rhs);
		Tuple2 & operator-=(float rhs);
		Tuple2 & operator+=(float rhs);

		Tuple2 & operator*=(const Tuple2 &rhs);
		Tuple2 & operator+=(const Tuple2 &rhs);
		Tuple2 & operator-=(const Tuple2 &rhs);

		Tuple2 operator*(float rhs) const;
		Tuple2 operator+(float rhs) const;
		Tuple2 operator-(float rhs) const;

		Tuple2 operator*(const Tuple2 &rhs) const;
		Tuple2 operator+(const Tuple2 &rhs) const;
		Tuple2 operator-(const Tuple2 &rhs) const;

	public: /* static */

		static const Tuple2 & Zero(void)
		    { static Tuple2 s_zero(.0f, .0f);
		      return(s_zero); }

		static const Tuple2 & One(void)
		    { static Tuple2 s_one(1.f, 1.f);
		      return(s_one); }
	};
	typedef Tuple2 Pair;

} /*********************************************************** Math Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
