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

#ifndef MARSHMALLOW_MATH_MATRIX4_H
#define MARSHMALLOW_MATH_MATRIX4_H 1

#include <core/environment.h>
#include <core/namespace.h>

MARSHMALLOW_NAMESPACE_BEGIN

namespace Math
{
	/*! @brief Math Matrix 4x4 */
	class MARSHMALLOW_MATH_EXPORT
	Matrix4
	{
		float m_value[16];
	public:
		explicit Matrix4(const float *values = 0);
		Matrix4(const Matrix4 &copy);

		float cell(int row, int col) const
		    { return(m_value[(row * 4) + col]); }

		const float *data(void) const
		    { return(&m_value[0]); }

		void identity(void);

	public: /* operators */

		Matrix4 & operator=(const Matrix4 &rhs);
		bool operator==(const Matrix4 &rhs) const;

		Matrix4 operator*(const Matrix4 &rhs) const;
		Matrix4 & operator*=(const Matrix4 &rhs);

		float & operator[](int i)
		    { return(m_value[i]); }

		float operator[](int i) const
		    { return(m_value[i]); }

	public: /* static */

		static const Matrix4 & Identity(void)
		    { static Matrix4 s_identity;
		      return(s_identity); }
	};
}

MARSHMALLOW_NAMESPACE_END

#endif
