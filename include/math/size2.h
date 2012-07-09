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

#ifndef MARSHMALLOW_MATH_SIZE2_H
#define MARSHMALLOW_MATH_SIZE2_H 1

#include <core/environment.h>
#include <core/global.h>
#include <core/namespace.h>

MARSHMALLOW_NAMESPACE_BEGIN

namespace Math
{
	/*! @brief 2D Size */
	template <typename T = float>
	class Size2
	{
		T m_value[2];

	public:
		/*! @brief Size2 template constructor
		 *  @param w Width
		 *  @param h Height
		 */
		Size2(T w = 0, T h = 0)
		    { m_value[0] = w;
		      m_value[1] = h; }

		Size2(const Size2 &copy)
		    { m_value[0] = copy.m_value[0];
		      m_value[1] = copy.m_value[1]; }

		inline const T & width(void) const
		    { return(m_value[0]); }
		inline const T & height(void) const
		    { return(m_value[1]); }

		T area(void) const
		    { return(m_value[0] * m_value[1]); }

		void zero(void)
		    { m_value[0] = m_value[1] = 0; }

		template <typename U>
		Size2<U> cast(void) const
		    { return(Size2<U>(static_cast<U>(m_value[0]), static_cast<U>(m_value[1]))); }

	public: /* operators */

		Size2<T> & operator=(const Size2<T> &rhs)
		    { m_value[0] = rhs.m_value[0];
		      m_value[1] = rhs.m_value[1];
		      return(*this); }

		bool operator==(const Size2<T> &rhs) const
		    { return( m_value[0] == rhs.m_value[0]
		           && m_value[1] == rhs.m_value[1]); }

		inline T & operator[](int i)
		    { return(m_value[i % 2]); }

		inline const T & operator[](int i) const
		    { return(m_value[i % 2]); }

		Size2<T> operator*(const T &c) const
		    { return(Size2<T>(m_value[0] * c, m_value[1] * c)); }

		Size2<T> operator/(const T &c) const
		    { return(Size2<T>(m_value[0] / c, m_value[1] / c)); }

	public: /* static */

		static const Size2<T> &Zero(void)
		    { static Size2<T> s_zero(0, 0);
		      return(s_zero); }

		static const Size2<T> &Identity(void)
		    { static Size2<T> s_identity(1, 1);
		      return(s_identity); }
	};
	typedef Size2<float> Size2f;
	typedef Size2<int>   Size2i;
}

MARSHMALLOW_NAMESPACE_END

#endif
