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
#include "core/type.h"

#include "math/point2.h"

#include "graphics/opengl/extensions/vbo.h"
#include "graphics/opengl/texturecoordinatedata.h"
#include "graphics/opengl/texturedata.h"
#include "graphics/opengl/vertexdata.h"
#include "graphics/quadmesh.h"
#include "graphics/viewport.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Graphics;
using namespace OpenGL;

struct Painter::Internal
{
	Core::Identifier last_texture_id;

	Internal(void)
	    : last_texture_id()
	{
	}

	void
	drawQuadMesh(const QuadMesh &g)
	{
		OpenGL::SharedVertexData l_vdata =
			g.vertexData()
			    .staticCast<OpenGL::VertexData>();

		if (!l_vdata) return;

		OpenGL::SharedTextureCoordinateData l_tcdata =
			g.textureCoordinateData()
			    .staticCast<OpenGL::TextureCoordinateData>();

		glEnableClientState(GL_VERTEX_ARRAY);

		if (l_tcdata)
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		/* vertex vbo check */
		if (l_vdata->isBuffered())
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, l_vdata->bufferId());

		glVertexPointer(2, GL_FLOAT, 0, l_vdata->data());

		/* texture coordinate vbo check */
		if (l_tcdata && l_tcdata->isBuffered())
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, l_tcdata->bufferId());
		/* else vbo cleanup */
		else if (l_vdata->isBuffered())
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

		if (l_tcdata) {
			glTexCoordPointer(2, GL_FLOAT, 0, l_tcdata->data());

			/* vbo cleanup */
			if (l_tcdata->isBuffered())
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
		}

		glDrawArrays(GL_TRIANGLE_STRIP, 0, QUAD_VERTEXES);

		if (l_tcdata)
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisableClientState(GL_VERTEX_ARRAY);

	}

	bool
	isMeshVisible(const IMesh &m, const Math::Point2 &o)
	{
		/*
		 * get visible coordinates (l,t,r,b)
		 */
		const float *l_visible = Graphics::Viewport::VisibleArea();
		float l_width, l_height;

		if (QuadMesh::Type() == m.type()) {
			const Math::Vector2 &l_tl = m.vertex(0 /* top-left */ );
			const Math::Vector2 &l_br = m.vertex(3 /* bottom-right */);

			l_width = l_br.x() - l_tl.x();
			l_height = l_tl.y() - l_br.y();
		}
		else {
			MMWARNING1("Unknown mesh type, ignoring width/height");
			l_width = l_height = 0;
		}

		return (o.x() >= l_visible[0] - l_width  && o.x() <= l_visible[2] + l_width
		     && o.y() <= l_visible[1] + l_height && o.y() >= l_visible[3] - l_height);
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
Painter::Draw(const IMesh &m, const Math::Point2 &o)
{
	if (!MGP.isMeshVisible(m, o))
		return;

	const float l_rotate_angle = m.rotation();

	float l_scale[2];
	m.scale(l_scale[0], l_scale[1]);

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(o.x(), o.y(), 0.f);

	/* set blending */
	Blend(AlphaBlending);

	/* set color */
	const Graphics::Color &l_color = m.color();
	glColor4f(l_color[0], l_color[1], l_color[2], l_color[3]);

	/* set texture */
	if (MGP.last_texture_id != m.textureData()->id()) {
		MGP.last_texture_id = m.textureData()->id();
		if (m.textureData()->isLoaded()) {
			OpenGL::SharedTextureData l_data =
				m.textureData()
				    .staticCast<OpenGL::TextureData>();
			glBindTexture(GL_TEXTURE_2D, l_data->textureId());
		} else glBindTexture(GL_TEXTURE_2D, 0);
	}

	/* set rotation */
	if (l_rotate_angle)
		glRotatef(l_rotate_angle, 0, 0, 1);

	/* set scale */
	if (l_scale[0] != 1.f || l_scale[1] != 1.f)
		glScalef(l_scale[0], l_scale[1], 1);

	/* actually draw graphic */
	if (QuadMesh::Type() == m.type())
		MGP.drawQuadMesh(static_cast<const QuadMesh &>(m));
	else MMWARNING1("Unknown mesh type");

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

