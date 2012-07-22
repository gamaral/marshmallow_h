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

#include "graphics/camera.h"
#include "graphics/quadmesh.h"
#include "graphics/transform.h"
#include "graphics/viewport.h"

#include <stack>

#include "extensions.h"
#include "texturecoordinatedata.h"
#include "texturedata.h"
#include "vertexdata.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace { /******************************************** Anonymous Namespace */

const char s_vertex_shader[] =
  "attribute vec2 a_texCoord;\n"
  "attribute vec4 a_position;\n"

  "uniform mat4 u_matrix;\n"
  "uniform mat4 u_model;\n"

  "varying vec2 v_texCoord;\n"

  "void main() {"
     "gl_Position = u_matrix * u_model * a_position;"
     "v_texCoord = a_texCoord;"
  "}\n";

const char s_fragment_shader[] =
#ifdef MARSHMALLOW_OPENGL_ES2_EGL
  "precision mediump float;\n"
#endif
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

struct PainterData
{
	GLuint location_position;
	GLuint location_texcoord;
	GLuint program_object;

	GLint location_color;
	GLint location_matrix;
	GLint location_model;
	GLint location_sampler;
	GLint location_usecolor;

	Core::Identifier last_texture_id;

	Math::Matrix4 matrix_current;
	std::stack<Math::Matrix4> matrix_stack;

	Graphics::Color bgcolor;

	bool location_matrix_invalidated;
} s_data;

GLuint
LoadShader(GLenum type, const char *src)
{
	using namespace Graphics::OpenGL;

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
LoadProgram(void)
{
	using namespace Graphics::OpenGL;

	GLuint l_vertex_shader;
	GLuint l_fragment_shader;
	GLuint l_program_object;
	GLint  l_linked;

	if ((l_vertex_shader = LoadShader(GL_VERTEX_SHADER, s_vertex_shader)) == 0)
		return(0);

	if ((l_fragment_shader = LoadShader(GL_FRAGMENT_SHADER, s_fragment_shader)) == 0)
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

inline void
UpdateLocationMatrix(void)
{
	using namespace Graphics::OpenGL;

	if (!s_data.location_matrix_invalidated)
		return;

	s_data.location_matrix_invalidated = false;
	glUniformMatrix4fv(s_data.location_matrix, 1, GL_FALSE,
			   s_data.matrix_current.data());
}

void
BeginDrawQuadMesh(const Graphics::QuadMesh &g, bool tcoords)
{
	using namespace Graphics::OpenGL;
	using Graphics::OpenGL::SharedTextureCoordinateData;
	using Graphics::OpenGL::SharedVertexData;
	using Graphics::OpenGL::TextureCoordinateData;
	using Graphics::OpenGL::VertexData;

	SharedVertexData l_vdata =
	    g.vertexData().staticCast<VertexData>();

	if (!l_vdata) return;

	SharedTextureCoordinateData l_tcdata =
	    g.textureCoordinateData() .staticCast<TextureCoordinateData>();

	/* ** vertex ** */

	/* bind buffer */
	if (l_vdata->isBuffered())
		glBindBuffer(GL_ARRAY_BUFFER, l_vdata->bufferId());

	glVertexAttribPointer(s_data.location_position, 2, GL_FLOAT, GL_FALSE, 0,
	    (l_vdata->isBuffered() ? 0 : l_vdata->data()));

	/* ** texture coordinates ** */

	if (tcoords) {
		/* bind buffer */
		if (l_tcdata->isBuffered())
			glBindBuffer(GL_ARRAY_BUFFER, l_tcdata->bufferId());
		else if (l_vdata->isBuffered())
			glBindBuffer(GL_ARRAY_BUFFER, 0);

		glVertexAttribPointer(s_data.location_texcoord, 2, GL_FLOAT, GL_FALSE, 0,
		    (l_tcdata->isBuffered() ? 0 : l_tcdata->data()));

		/* cleanup */
		if (l_tcdata->isBuffered())
			glBindBuffer(GL_ARRAY_BUFFER, 0);
	} else if (l_vdata->isBuffered())
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(s_data.location_position);
	if (tcoords) glEnableVertexAttribArray(s_data.location_texcoord);
}

inline void
DrawQuadMesh(void)
{
	glDrawArrays(GL_TRIANGLE_STRIP, 0, MARSHMALLOW_QUAD_VERTEXES);
}

inline void
EndDrawQuadMesh(bool tcoords)
{
	using namespace Graphics::OpenGL;

	if (tcoords) glDisableVertexAttribArray(s_data.location_texcoord);
	glDisableVertexAttribArray(s_data.location_position);
}

} /****************************************************** Anonymous Namespace */

namespace Graphics { /************************************ Graphics Namespace */

void
Painter::Initialize(void)
{
	using namespace Graphics::OpenGL;

	static const char s_error_msg[] = "Failed to load primary shaders";

	s_data.location_matrix_invalidated = true;

	if ((s_data.program_object = LoadProgram()) == 0)
		MMFATAL(s_error_msg);

	GLint l_location;

	if ((l_location = glGetAttribLocation(s_data.program_object, "a_position")) == -1)
		MMFATAL(s_error_msg);
	s_data.location_position = static_cast<GLuint>(l_location);

	if ((l_location = glGetAttribLocation(s_data.program_object, "a_texCoord")) == -1)
		MMFATAL(s_error_msg);
	s_data.location_texcoord = static_cast<GLuint>(l_location);

	s_data.location_color = glGetUniformLocation(s_data.program_object, "u_color");
	s_data.location_matrix = glGetUniformLocation(s_data.program_object, "u_matrix");
	s_data.location_model = glGetUniformLocation(s_data.program_object, "u_model");
	s_data.location_sampler = glGetUniformLocation(s_data.program_object, "s_data.texture");
	s_data.location_usecolor = glGetUniformLocation(s_data.program_object, "u_usecolor");

	s_data.bgcolor = Graphics::Color::Black();

	glUseProgram(s_data.program_object);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	Reset();

	Viewport::SwapBuffer();
}

void
Painter::Finalize(void)
{
}

void
Painter::Render(void)
{
	UpdateLocationMatrix();
}

void
Painter::Reset(void)
{
	using namespace Math;

	s_data.location_matrix_invalidated = true;

	const Color &bgcolor = s_data.bgcolor;
	glClearColor(bgcolor.red(),
	             bgcolor.green(),
	             bgcolor.blue(),
	             bgcolor.alpha());
	glClear(GL_COLOR_BUFFER_BIT);
	LoadProjection();

	s_data.matrix_current *= Camera::Transform().matrix(Transform::mtView);
}

const Color &
Painter::BackgroundColor(void)
{
	return(s_data.bgcolor);
}

void
Painter::SetBackgroundColor(const Color &color)
{
	s_data.bgcolor = color;
}

Math::Matrix4 &
Painter::Matrix(void)
{
	return(s_data.matrix_current);
}

void
Painter::LoadIdentity(void)
{
	s_data.location_matrix_invalidated = true;
	s_data.matrix_current = Math::Matrix4::Identity();
}

void
Painter::LoadProjection(void)
{
	using namespace Math;

	s_data.location_matrix_invalidated = true;
	s_data.matrix_current = Matrix4::Identity();
	s_data.matrix_current[Matrix4::m11] =  2.f / Viewport::Size().width();
	s_data.matrix_current[Matrix4::m22] =  2.f / Viewport::Size().height();
	s_data.matrix_current[Matrix4::m33] = -1.f;
}

void
Painter::PushMatrix(void)
{
	s_data.matrix_stack.push(s_data.matrix_current);
}

void
Painter::PopMatrix(void)
{
	s_data.location_matrix_invalidated = true;

	if (s_data.matrix_stack.size() <= 0) {
		MMWARNING("Matrix stack is empty! Ignoring pop matrix.");
		s_data.matrix_current = Math::Matrix4::Identity();
		return;
	}

	s_data.matrix_current = s_data.matrix_stack.top();
	s_data.matrix_stack.pop();
}

void
Painter::Draw(const IMesh &m, const Math::Point2 &o)
{
	Draw(m, &o, 1);
}

void
Painter::Draw(const IMesh &m, const Math::Point2 *o, int c)
{
	using namespace Graphics::OpenGL;
	using Graphics::OpenGL::SharedTextureData;
	using Graphics::OpenGL::TextureData;

	float l_scale[2];
	m.scale(l_scale[0], l_scale[1]);

	UpdateLocationMatrix();

	/* set blending */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* color */
	const Graphics::Color &l_color = m.color();
	glUniform4f(s_data.location_color, l_color.red(), l_color.green(), l_color.blue(), l_color.alpha());

	/* set texture */
	glActiveTexture(GL_TEXTURE0);
	if (s_data.last_texture_id != m.textureData()->id()) {
		s_data.last_texture_id = m.textureData()->id();
		if (m.textureData()->isLoaded()) {
			SharedTextureData l_data =
			    m.textureData().staticCast<TextureData>();
			glBindTexture(GL_TEXTURE_2D, l_data->textureId());
			glUniform1i(s_data.location_usecolor, 0);
		}
		else glUniform1i(s_data.location_usecolor, 1);
	}

	/* prepare model amatrix */
	Graphics::Transform l_model;
	l_model.setRotation(m.rotation());
	l_model.setScale(Math::Size2f(l_scale[0], l_scale[1]));

	/* check mesh for texture coordinates */
	const bool l_tcoords = s_data.last_texture_id.uid() && m.textureCoordinateData();

	/* prepare to draw mesh */
	if (QuadMesh::Type() == m.type())
		BeginDrawQuadMesh(static_cast<const QuadMesh &>(m), l_tcoords);
	else MMWARNING("Unknown mesh type");

	/* draw mesh(es) */
	for (int i = 0; i < c; ++i) {
		/* update model matrix */
		l_model.setTranslation(o[i]);
		glUniformMatrix4fv(s_data.location_model, 1, GL_FALSE, l_model.matrix().data());

		/* actually draw graphic */
		if (QuadMesh::Type() == m.type())
			DrawQuadMesh();
	}

	/* cleanup */
	if (QuadMesh::Type() == m.type())
		EndDrawQuadMesh(l_tcoords);

	glDisable(GL_BLEND);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

