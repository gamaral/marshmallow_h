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

#ifndef MARSHMALLOW_MATH_RECT2_H
#define MARSHMALLOW_MATH_RECT2_H 1

#include <core/global.h>
#include <math/size2.h>
#include <math/vector2.h>

MARSHMALLOW_NAMESPACE_BEGIN

namespace Math
{
	/*! @brief 2D Rect */
	class MARSHMALLOW_MATH_EXPORT
	Rect2
	{
		float m_side[4];

	public:
	
		enum Side
		{
			  Left = 0,
			   Top = 1,
			 Right = 2,
			Bottom = 3,
			 Sides = 4
		};

	public:

		Rect2(float width, float height);
		Rect2(float x, float y, float width, float height);
		explicit Rect2(const Size2f &size);
		explicit Rect2(const Vector2 &offset, const Size2f &size);
		Rect2(const Rect2 &copy);

		Size2f size(void) const;

		Vector2 topLeft(void) const
		    { return(Vector2(m_side[Left], m_side[Top])); }
		Vector2 topRight(void) const
		    { return(Vector2(m_side[Right], m_side[Top])); }
		Vector2 bottomLeft(void) const
		    { return(Vector2(m_side[Left], m_side[Bottom])); }
		Vector2 bottomRight(void) const
		    { return(Vector2(m_side[Right], m_side[Bottom])); }

		float area(void) const
		    { return((m_side[Right]  - m_side[Left]) *
		             (m_side[Bottom] - m_side[Top])); }

	public: /* operators */

		Rect2 & operator=(const Rect2 &rhs);

		float & operator[](int i)
		    { return(m_side[i % Sides]); }

		const float & operator[](int i) const
		    { return(m_side[i % Sides]); }
	};
}

MARSHMALLOW_NAMESPACE_END

#endif
