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

#ifndef MATH_SIZE3_H
#define MATH_SIZE3_H 1

#include "core/global.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Math
{

	/*! @brief 3D Size */
	template <typename T = float>
	class MATH_EXPORT Size3
	{
		T m_value[3];

	public:
		Size3(T width = 0., T height = 0., T depth = 0.);
		Size3(const Size3 &copy);

		T & rwidth(void)
		    { return(m_value[0]); }
		T & rheight(void)
		    { return(m_value[1]); }
		T & rdepth(void)
		    { return(m_value[2]); }

		const T & rwidth(void) const
		    { return(m_value[0]); }
		const T & rheight(void) const
		    { return(m_value[1]); }
		const T & rdepth(void) const
		    { return(m_value[2]); }

		T width(void) const
		    { return(m_value[0]); }
		T height(void) const
		    { return(m_value[1]); }
		T depth(void) const
		    { return(m_value[2]); }

		T & operator[](int i)
		    { return(m_value[i % 3]); }

		T volume(void) const;

		template <typename U>
		Size3<U> staticCast(void) const;
	};
	typedef Size3<float> Size3f;
	typedef Size3<int>   Size3i;

	template <typename T>
	Size3<T>::Size3(T aw, T ah, T ad)
	{
		m_value[0] = aw;
		m_value[1] = ah;
		m_value[2] = ad;
	}

	template <typename T>
	Size3<T>::Size3(const Size3 &copy)
	{
		m_value[0] = copy.m_value[0];
		m_value[1] = copy.m_value[1];
		m_value[2] = copy.m_value[2];
	}

	template <typename T> T
	Size3<T>::volume(void) const
	{
		return(m_value[0] * m_value[1] * m_value[2]);
	}

	template <typename T>
	template <typename U> Size3<U>
	Size3<T>::staticCast(void) const
	{
		return(Size3<U>(static_cast<U>(m_value[0]),
		                static_cast<U>(m_value[1]),
		                static_cast<U>(m_value[2]));
	}

}

MARSHMALLOW_NAMESPACE_END

#endif
