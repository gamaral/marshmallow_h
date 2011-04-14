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
	class MATH_EXPORT Size3
	{
		double m_value[3];

	public:
		Size3(double width = 0., double height = 0., double depth = 0.);
		Size3(const Size3 &copy);

		double & rwidth(void)
		    { return(m_value[0]); }
		double & rheight(void)
		    { return(m_value[1]); }
		double & rdepth(void)
		    { return(m_value[2]); }

		const double & rwidth(void) const
		    { return(m_value[0]); }
		const double & rheight(void) const
		    { return(m_value[1]); }
		const double & rdepth(void) const
		    { return(m_value[2]); }

		double width(void) const
		    { return(m_value[0]); }
		double height(void) const
		    { return(m_value[1]); }
		double depth(void) const
		    { return(m_value[2]); }

		double & operator[](int i)
		    { return(m_value[i % 3]); }

		long double volume(void) const;
	};

}

MARSHMALLOW_NAMESPACE_END

#endif
