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

#include "graphics/painter.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <GL/gl.h>

#include "core/platform.h"
#include "graphics/linegraphic.h"
#include "graphics/pointgraphic.h"
#include "graphics/trianglegraphic.h"
#include "graphics/quadgraphic.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Graphics;

struct Painter::Internal
{
	void
	drawPointGraphic(const PointGraphic &g)
	{
		const Math::Vector2 &l_p = g.position();
		glBegin(GL_POINTS);
		glVertex2f(static_cast<GLfloat>(l_p.rx()),
		           static_cast<GLfloat>(l_p.ry()));
		glEnd();
	}

	void
	drawLineGraphic(const LineGraphic &g)
	{
		glBegin(GL_LINES);
		for (int i = 0; i < 2; ++i) {
			const Math::Vector2 &l_p = g[i];
			glVertex2f(static_cast<GLfloat>(l_p.rx()),
			           static_cast<GLfloat>(l_p.ry()));
		}
		glEnd();
	}

	void
	drawTriangleGraphic(const TriangleGraphic &g)
	{
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < 3; ++i) {
			const Math::Vector2 &l_p = g[i];
			glVertex2f(static_cast<GLfloat>(l_p.rx()),
			           static_cast<GLfloat>(l_p.ry()));
		}
		glEnd();
	}

	void
	drawQuadGraphic(const QuadGraphic &g)
	{
		glBegin(GL_QUADS);
		for (int i = 0; i < 4; ++i) {
			const Math::Vector2 &l_p = g[i];
			glVertex2f(static_cast<GLfloat>(l_p.rx()),
			           static_cast<GLfloat>(l_p.ry()));
		}
		glEnd();
	}
} MGP;

void
Painter::Initialize(void)
{
}

void
Painter::Finalize(void)
{
}

void
Painter::Draw(const IGraphic &g)
{
	switch (g.type()) {
	case PointGraphicType:
		MGP.drawPointGraphic(static_cast<const PointGraphic &>(g));
	break;
	case LineGraphicType:
		MGP.drawLineGraphic(static_cast<const LineGraphic &>(g));
	break;
	case TriangleGraphicType:
		MGP.drawTriangleGraphic(static_cast<const TriangleGraphic &>(g));
	break;
	case QuadGraphicType:
		MGP.drawQuadGraphic(static_cast<const QuadGraphic &>(g));
	break;
	default: WARNING1("Unknown graphic type"); break;
	}
}

