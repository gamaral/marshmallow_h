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

#include "core/logger.h"
#include "math/point2.h"
#include "graphics/linemesh.h"
#include "graphics/quadmesh.h"
#include "graphics/trianglemesh.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Graphics;

struct Painter::Internal
{
	void
	drawLineMesh(const LineMesh &g)
	{
		glVertexPointer(2, GL_FLOAT, 0, g.vertexDataArray());
		glDrawArrays(GL_LINES, 0, 4);
	}

	void
	drawTriangleMesh(const TriangleMesh &g)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, g.vertexDataArray());
		glTexCoordPointer(2, GL_FLOAT, 0, g.textureCoordArray());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	void
	drawQuadMesh(const QuadMesh &g)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, g.vertexDataArray());
		glTexCoordPointer(2, GL_FLOAT, 0, g.textureCoordArray());
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

} MGP;

void
Painter::Initialize(void)
{
	glEnableClientState(GL_VERTEX_ARRAY);
}

void
Painter::Finalize(void)
{
	glDisableClientState(GL_VERTEX_ARRAY);
}

void
Painter::Draw(const IMesh &g, const Math::Point2 &o)
{
	const bool  l_texture = (g.texture());
	const float l_rotate_angle = g.rotation();

	float l_scale[2];
	g.scale(l_scale[0], l_scale[1]);

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(o.rx(), o.ry(), 0.f);

	/* set blending */
	Blend(AlphaBlending);

	/* set color */
	const Graphics::Color &l_color = g.color();
	glColor4f(l_color[0], l_color[1], l_color[2], l_color[3]);

	/* set texture */
	if (l_texture)
		glBindTexture(GL_TEXTURE_2D, g.texture()->tid());

	/* set rotation */
	if (l_rotate_angle)
		glRotatef(l_rotate_angle, 0, 0, 1);

	/* set scale */
	if (l_scale[0] != 1 || l_scale[1] != 1)
		glScalef(l_scale[0], l_scale[1], 1);

	/* actually draw graphic */
	if (g.type() == QuadMesh::Type)
		MGP.drawQuadMesh(static_cast<const QuadMesh &>(g));
	else if (g.type() == LineMesh::Type)
		MGP.drawLineMesh(static_cast<const LineMesh &>(g));
	else if (g.type() == TriangleMesh::Type)
		MGP.drawTriangleMesh(static_cast<const TriangleMesh &>(g));
	else WARNING1("Unknown mesh type");

	if (l_texture)
		glBindTexture(GL_TEXTURE_2D, 0);

	Blend(NoBlending);

	glPopMatrix();
}

void
Painter::Blend(BlendTypes b)
{
	switch (b) {
	case AdditiveBlending:
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	break;
	case AlphaBlending:
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	break;
	case MultiplyBlending:
		glEnable(GL_BLEND);
		glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	break;
	case NoBlending:
		glDisable(GL_BLEND);
	break;
	}
}

