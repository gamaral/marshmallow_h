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

#include "math/rect2.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "math/vector2.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Math;

Rect2::Rect2(const Point2 &o, const Size2 &s)
    : m_origin(o),
      m_size(s)
{
}

Rect2::Rect2(const Rect2 &c)
    : m_origin(c.m_origin),
      m_size(c.m_size)
{
}

Vector2
Rect2::topLeft(void) const
{
	return(Vector2(-m_size.width()  / 2.f,
	               -m_size.height() / 2.f));
}

Vector2
Rect2::topRight(void) const
{
	return(Vector2(m_size.width()  / 2.f,
	              -m_size.height() / 2.f));
}

Vector2
Rect2::bottomLeft(void) const
{
	return(Vector2(-m_size.width()  / 2.f,
	                m_size.height() / 2.f));
}

Vector2
Rect2::bottomRight(void) const
{
	return(Vector2(m_size.width()  / 2.f,
	               m_size.height() / 2.f));
}

