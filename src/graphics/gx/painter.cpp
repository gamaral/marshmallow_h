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
#include "math/pair.h"
#include "math/point2.h"

#include "graphics/quadmesh.h"
#include "graphics/transform.h"
#include "graphics/viewport.h"

#include "texturecoordinatedata.h"
#include "texturedata.h"
#include "vertexdata.h"

#include <cstdlib>
#include <stack>

MARSHMALLOW_NAMESPACE_USE
using namespace Graphics::GX;
using namespace Graphics;

/******************************************************************************/

namespace
{
	Math::Matrix4 s_matrix_current;
	std::stack<Math::Matrix4> s_matrix_stack;

#if 0
	static const char s_vertex_shader[] =
	  "attribute vec2 a_texCoord;\n"
	  "attribute vec4 a_position;\n"

	  "uniform mat4 u_matrix;\n"
	  "uniform mat4 u_model;\n"

	  "varying vec2 v_texCoord;\n"

	  "void main() {"
	     "gl_Position = u_matrix * u_model * a_position;"
	     "v_texCoord = a_texCoord;"
	  "}\n";

	static const char s_fragment_shader[] =
	  "precision mediump float;\n"

	  "uniform bool u_usecolor;\n"
	  "uniform sampler2D s_texture;\n"
	  "uniform vec4 u_color;\n"

	  "varying vec2 v_texCoord;\n"
	  "void main() {"
	     "if (u_usecolor) "
	     "  gl_FragColor = u_color;"
	     "else "
	     "  gl_FragColor = texture2D(s_texture, v_texCoord) * u_color;"
	  "}\n";

	uint32_t s_program_object;

	GLint s_location_color;
	GLint s_location_matrix;
	GLint s_location_model;
	GLint s_location_position;
	GLint s_location_sampler;
	GLint s_location_texcoord;
	GLint s_location_usecolor;

	static Core::Identifier s_last_texture_id;

	GLuint
	loadShader(GLenum type, const char *src)
	{
		GLuint l_shader;
		GLint  l_compiled;

		if ((l_shader = glCreateShader(type)) == 0)
			return(0);

		glShaderSource(l_shader, 1, &src, 0);
		glCompileShader(l_shader);

		glGetShaderiv(l_shader, GL_COMPILE_STATUS, &l_compiled);
		if (!l_compiled) {
			GLint l_length = 0;
			glGetShaderiv(l_shader, GL_INFO_LOG_LENGTH, &l_length);

			if (l_length > 1) {
				char *l_log = new char[l_length];
				glGetShaderInfoLog(l_shader, l_length, 0, l_log);
				MMERROR("Error while compiling shader: " << l_log);
				delete[] l_log;
			}

			glDeleteShader(l_shader);
			return(0);
		}

		return(l_shader);
	}

	GLuint
	loadProgram(void)
	{
		GLuint l_vertex_shader;
		GLuint l_fragment_shader;
		GLuint l_program_object;
		GLint  l_linked;

		if ((l_vertex_shader = loadShader(GL_VERTEX_SHADER, s_vertex_shader)) == 0)
			return(0);

		if ((l_fragment_shader = loadShader(GL_FRAGMENT_SHADER, s_fragment_shader)) == 0)
		{
			glDeleteShader(l_vertex_shader);
			return(0);
		}

		if ((l_program_object = glCreateProgram()) == 0)
			return(0);

		glAttachShader(l_program_object, l_vertex_shader);
		glAttachShader(l_program_object, l_fragment_shader);

		glLinkProgram(l_program_object);

		glGetProgramiv(l_program_object, GL_LINK_STATUS, &l_linked);

		if (!l_linked) {
			GLint l_length = 0;

			glGetProgramiv(l_program_object, GL_INFO_LOG_LENGTH, &l_length);
			if (l_length > 1) {
				char *l_log = new char[l_length];
				glGetShaderInfoLog(l_program_object, l_length, 0, l_log);
				MMERROR("Error while linking shader: " << l_log);
				delete[] l_log;
			}

			glDeleteProgram(l_program_object);
			return(0);
		}

		glDeleteShader(l_vertex_shader);
		glDeleteShader(l_fragment_shader);

		return(l_program_object);
	}

	void
	DrawQuadMesh(const QuadMesh &g)
	{
		GX::SharedVertexData l_vdata =
			g.vertexData()
			    .staticCast<GX::VertexData>();

		if (!l_vdata) return;

		GX::SharedTextureCoordinateData l_tcdata =
			g.textureCoordinateData()
			    .staticCast<GX::TextureCoordinateData>();

		/* ** vertex ** */

		/* bind buffer */
		if (l_vdata->isBuffered())
			glBindBuffer(GL_ARRAY_BUFFER, l_vdata->bufferId());

		glVertexAttribPointer(s_location_position, 2, GL_FLOAT, GL_FALSE, 0,
		    (l_vdata->isBuffered() ? 0 : l_vdata->data()));

		/* cleanup */
		if (l_vdata->isBuffered() && !l_tcdata->isBuffered())
			glBindBuffer(GL_ARRAY_BUFFER, 0);

		/* ** texture ** */

		if (l_tcdata) {
			/* bind buffer */
			if (l_tcdata->isBuffered())
				glBindBuffer(GL_ARRAY_BUFFER, l_tcdata->bufferId());

			glVertexAttribPointer(s_location_texcoord, 2, GL_FLOAT, GL_FALSE, 0,
			    (l_tcdata->isBuffered() ? 0 : l_tcdata->data()));

			/* cleanup */
			if (l_tcdata->isBuffered())
				glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		glEnableVertexAttribArray(s_location_position);
		if (l_tcdata) glEnableVertexAttribArray(s_location_texcoord);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, MARSHMALLOW_QUAD_VERTEXES);

		if (l_tcdata) glDisableVertexAttribArray(s_location_texcoord);
		glDisableVertexAttribArray(s_location_position);
	}
#endif
} // namespace

/******************************************************************************/

void
Painter::Initialize(void)
{
#if 0
	if ((s_program_object = loadProgram()) == 0)
		MMFATAL("Failed to load primary shaders");

	s_location_position = glGetAttribLocation(s_program_object, "a_position");
	s_location_texcoord = glGetAttribLocation(s_program_object, "a_texCoord");

	s_location_color = glGetUniformLocation(s_program_object, "u_color");
	s_location_matrix = glGetUniformLocation(s_program_object, "u_matrix");
	s_location_model = glGetUniformLocation(s_program_object, "u_model");
	s_location_sampler = glGetUniformLocation(s_program_object, "s_texture");
	s_location_usecolor = glGetUniformLocation(s_program_object, "u_usecolor");

	glUseProgram(s_program_object);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	Reset();

	Viewport::SwapBuffer();
#endif
}

void
Painter::Finalize(void)
{
}

void
Painter::Render(void)
{
#if 0
	glUniformMatrix4fv(s_location_matrix, 1, GL_FALSE, s_matrix_current.data());
#endif
}

void
Painter::Reset(void)
{
#if 0
	glClearColor(.0f, .0f, .0f, .0f);
	glClear(GL_COLOR_BUFFER_BIT);
	ProjectionMatrix();

	Math::Matrix4 l_view = Viewport::Camera().matrix();
	l_view[12] *= -1.f;
	l_view[13] *= -1.f;
	s_matrix_current *= l_view;
#endif
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
Painter::ProjectionMatrix(void)
{
	s_matrix_current = Math::Matrix4::Identity();
	s_matrix_current[0]  =  2.f / Viewport::Size().width();
	s_matrix_current[5]  =  2.f / Viewport::Size().height();
	s_matrix_current[10] = -1.f;
}

void
Painter::PushMatrix(void)
{
	s_matrix_stack.push(s_matrix_current);
}

void
Painter::PopMatrix(void)
{
	if (s_matrix_stack.size() <= 0) {
		MMWARNING("Matrix stack is empty! Ignoring pop matrix.");
		s_matrix_current = Math::Matrix4::Identity();
		return;
	}

	s_matrix_current = s_matrix_stack.top();
	s_matrix_stack.pop();
}

void
Painter::Draw(const IMesh &m, const Math::Point2 &o)
{
#if 0
	float l_scale[2];
	m.scale(l_scale[0], l_scale[1]);

	Graphics::Transform l_model;
	l_model.setTranslation(o);
	l_model.setRotation(m.rotation());
	l_model.setScale(Math::Pair(l_scale[0], l_scale[1]));

	glUniformMatrix4fv(s_location_model, 1, GL_FALSE, l_model.matrix().data());

	/* set blending */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* color */
	const Graphics::Color &l_color = m.color();
	glUniform4f(s_location_color, l_color.red(), l_color.green(), l_color.blue(), l_color.alpha() );

	/* set texture */
	glActiveTexture(GL_TEXTURE0);
	if (s_last_texture_id != m.textureData()->id()) {
		s_last_texture_id = m.textureData()->id();
		if (m.textureData()->isLoaded()) {
			GX::SharedTextureData l_data =
				m.textureData()
				    .staticCast<GX::TextureData>();
			glBindTexture(GL_TEXTURE_2D, l_data->textureId());
			glUniform1i(s_location_usecolor, 0);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, 0);
			glUniform1i(s_location_usecolor, 1);
		}
	}

	/* actually draw graphic */
	if (QuadMesh::Type() == m.type())
		DrawQuadMesh(static_cast<const QuadMesh &>(m));
	else MMWARNING("Unknown mesh type");

	glDisable(GL_BLEND);
#endif
}

