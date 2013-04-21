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

#ifndef MARSHMALLOW_MATH_SIZE2_H
#define MARSHMALLOW_MATH_SIZE2_H 1

#include <core/environment.h>
#include <core/global.h>
#include <core/namespace.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Math { /******************************************** Math Namespace */

	/*! @brief 2D Size */
	template <typename T = float>
	struct Size2
	{
		/*
		 * direct accessors
		 */
		T width, height;

	public:
		/*! @brief Size2 template constructor
		 *  @param w Width
		 *  @param h Height
		 */
		inline Size2(T width_ = 0, T height_ = 0)
		    : width(width_), height(height_) {}

		inline Size2 & set(T width, T height);

		inline T area(void) const
		    { return(width * height); }

		inline void zero(void)
		    { width = height = 0; }

		template <typename U>
		inline Size2<U> cast(void) const
		    { return(Size2<U>(static_cast<U>(width),
		                      static_cast<U>(height))); }

	public: /* operators */

		inline Size2<T> & operator=(const Size2<T> &rhs);

		inline bool operator==(const Size2<T> &rhs) const
		    { return( width == rhs.width && height == rhs.height); }

		inline Size2<T> operator*(const T &scalar) const;
		inline Size2<T> operator/(const T &scalar) const;

		inline Size2<T> operator*(const Size2<T> rhs) const;
		inline Size2<T> operator/(const Size2<T> rhs) const;

		inline Size2<T> & operator*=(const Size2<T> rhs);
		inline Size2<T> & operator/=(const Size2<T> rhs);


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

	template <typename T>
	Size2<T> &
	Size2<T>::set(T width_, T height_)
	{
		width = width_, height = height_;
		return(*this);
	}

	template <typename T>
	Size2<T> &
	Size2<T>::operator=(const Size2<T> &rhs)
	{
		width  = rhs.width, height = rhs.height;
		return(*this);
	}

	template <typename T>
	Size2<T>
	Size2<T>::operator*(const T &scalar) const
	{
		return(Size2<T>(width * scalar, height * scalar));
	}

	template <typename T>
	Size2<T>
	Size2<T>::operator/(const T &scalar) const
	{
		return(Size2<T>(width / scalar, height / scalar));
	}

	template <typename T>
	Size2<T>
	Size2<T>::operator*(const Size2<T> rhs) const
	{
		return(Size2<T>(width * rhs.width, height * rhs.height));
	}

	template <typename T>
	Size2<T>
	Size2<T>::operator/(const Size2<T> rhs) const
	{
		return(Size2<T>(width / rhs.width, height / rhs.height));
	}

	template <typename T>
	Size2<T> &
	Size2<T>::operator*=(const Size2<T> rhs)
	{
		width *= rhs.width, height *= rhs.height;
		return(*this);
	}

	template <typename T>
	Size2<T> &
	Size2<T>::operator/=(const Size2<T> rhs)
	{
		width /= rhs.width, height /= rhs.height;
		return(*this);
	}

} /*********************************************************** Math Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
