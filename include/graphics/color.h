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

#ifndef MARSHMALLOW_GRAPHICS_COLOR_H
#define MARSHMALLOW_GRAPHICS_COLOR_H 1

#include <core/environment.h>
#include <core/global.h>
#include <core/namespace.h>

#include <graphics/config.h>

MARSHMALLOW_NAMESPACE_BEGIN

namespace Graphics
{

	/*! @brief Graphics Color */
	class MARSHMALLOW_GRAPHICS_EXPORT
	Color
	{
		float m_rgba[4];

	public:

		Color(float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f );
		Color(const Color &copy);
		~Color(void);

		float & rred(void)
		    { return(m_rgba[0]); }
		float & rgreen(void)
		    { return(m_rgba[1]); }
		float & rblue(void)
		    { return(m_rgba[2]); }
		float & ralpha(void)
		    { return(m_rgba[3]); }

		float red(void) const
		    { return(m_rgba[0]); }
		float green(void) const
		    { return(m_rgba[1]); }
		float blue(void) const
		    { return(m_rgba[2]); }
		float alpha(void) const
		    { return(m_rgba[3]); }

	public: /* operators */

		float operator[](int index) const
		    { return(m_rgba[index % 4]); }

		float & operator[](int index)
		    { return(m_rgba[index % 4]); }

		Color & operator=(const Color &rhs);

	public: /* static */

		static const Color & Visible(void)
		    { static const Color s_visible(1.f, 1.f, 1.f, 1.f);
		      return(s_visible); }

		static const Color & Invisible(void)
		    { static const Color s_invisible(1.f, 1.f, 1.f, 0.f);
		      return(s_invisible); }
	};

}

MARSHMALLOW_NAMESPACE_END

#endif
