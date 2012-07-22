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

#include "graphics/painter.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"
#include "core/type.h"

#include "math/matrix4.h"
#include "math/point2.h"

#include "graphics/color.h"
#include "graphics/imesh.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace { /******************************************** Anonymous Namespace */
	Math::Matrix4   s_matrix_current;
	Graphics::Color s_bgcolor;
} /****************************************************** Anonymous Namespace */

namespace Graphics { /************************************ Graphics Namespace */

void
Painter::Initialize(void)
{
}

void
Painter::Finalize(void)
{
}

void
Painter::Render(void)
{
}

void
Painter::Reset(void)
{
}

const Color &
Painter::BackgroundColor(void)
{
	return(s_bgcolor);
}

void
Painter::SetBackgroundColor(const Color &color)
{
	s_bgcolor = color;
}

Math::Matrix4 &
Painter::Matrix(void)
{
	return(s_matrix_current);
}

void
Painter::LoadIdentity(void)
{
	s_matrix_current = Math::Matrix4::Identity();
}

void
Painter::LoadProjection(void)
{
}

void
Painter::PushMatrix(void)
{
}

void
Painter::PopMatrix(void)
{
}

void
Painter::Draw(const IMesh &m, const Math::Point2 &p)
{
	Draw(m, &p, 1);
}

void
Painter::Draw(const IMesh &m, const Math::Point2 *p, int c)
{
	/* Unused if not in verbose debug mode */
	MMUNUSED(m);
	MMUNUSED(p);

	for (int i = 0; i < c; ++i)
		MMVERBOSE("Drawing " << m.type().str() << " at (" << p[i].x() << ", " << p[i].y() << ").");
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

