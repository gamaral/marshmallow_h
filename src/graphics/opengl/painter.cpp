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

#include "graphics/opengl/painter_p.h"

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
#include "graphics/viewport_p.h"

#include <stack>
#include <cstring>

#include "extensions.h"
#include "texturecoordinatedata.h"
#include "texturedata.h"
#include "vertexdata.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace { /******************************************** Anonymous Namespace */
namespace GLPainter { /********************************** GLPainter Namespace */

	inline void Initialize(void);
	inline void Finalize(void);

	inline void Reset(void);

	inline GLuint LoadShader(GLenum type, const char *src);
	inline void UnloadShader(GLuint &shader);

	inline GLuint LoadProgram(void);
	inline void UnloadProgram(GLuint &program);

	inline void CommitMatrix(void);
	inline void PopMatrix(void);
	inline void PushMatrix(void);

	inline void LoadIdentity(void);
	inline void LoadProjection(void);
	inline void LoadViewProjection(void);

	inline void Draw(const Graphics::IMesh &mesh,
	                 const Math::Point2 *origins,
	                 size_t count);
	inline void BeginDrawQuadMesh(const Graphics::QuadMesh &mesh, bool tcoords);
	inline void DrawQuadMesh(void);
	inline void EndDrawQuadMesh(bool tcoords);

	enum StateFlag
	{
		sfUninitialized = 0,
		sfInitialized   = (1 << 0),
		sfInvalidMatrix = (1 << 1)
	};

	/* program */
	GLuint program_object;

	/* assets */
	GLuint location_position;
	GLuint location_texcoord;
	Core::Identifier last_texture_id;

	/* shader locations */
	GLint location_color;
	GLint location_matrix;
	GLint location_model;
	GLint location_sampler;
	GLint location_usecolor;

	/* matrix */
	Math::Matrix4 matrix;
	std::stack<Math::Matrix4> matrix_stack;

	/* persistent */
	Graphics::Color bgcolor;

	unsigned int session_id = 0;
	int flags;

} /****************************************************** GLPainter Namespace */

const char s_vertex_shader[] =
  "#version 100\n"

  "attribute vec2 a_texCoord;\n"
  "attribute vec4 a_position;\n"

  "uniform mat4 u_matrix;\n"
  "uniform mat4 u_model;\n"

  "varying vec2 v_texCoord;\n"

  "void main() { "
    "gl_Position = u_matrix * u_model * a_position; "
    "v_texCoord = a_texCoord; "
  "}\n";

const char s_fragment_shader[] =
  "#version 100\n"

#if MARSHMALLOW_OPENGL_ES2
  "precision mediump float;\n"
  "precision mediump int;\n"
#endif

  "uniform bool u_usecolor;\n"
  "uniform sampler2D s_texture;\n"
  "uniform vec4 u_color;\n"

  "varying vec2 v_texCoord;\n"

  "void main() { "
    "if (u_usecolor) "
      "gl_FragColor = u_color; "
    "else "
      "gl_FragColor = texture2D(s_texture, v_texCoord) * u_color; "
  "}\n";

void
GLPainter::Initialize(void)
{
	using namespace Graphics::OpenGL;
	using namespace Graphics;

	flags = sfUninitialized;

	if (0 == (program_object = LoadProgram()))
		MMFATAL("Failed to load GLSL program object.");

	/* populate locations */
	GLint l_location_position =
	    glGetAttribLocation(program_object, "a_position");
	GLint l_location_texcoord =
	    glGetAttribLocation(program_object, "a_texCoord");
	location_sampler =
	    glGetUniformLocation(program_object, "s_texture");
	location_color =
	    glGetUniformLocation(program_object, "u_color");
	location_matrix =
	    glGetUniformLocation(program_object, "u_matrix");
	location_model =
	    glGetUniformLocation(program_object, "u_model");
	location_usecolor =
	    glGetUniformLocation(program_object, "u_usecolor");

	if (-1 == l_location_position
	 || -1 == l_location_texcoord
	 || -1 == location_color
	 || -1 == location_matrix
	 || -1 == location_model
	 || -1 == location_sampler
	 || -1 == location_usecolor)
		MMFATAL("Failed to initialize GLSL painter.");

	location_position = static_cast<GLuint>(l_location_position);
	location_texcoord = static_cast<GLuint>(l_location_texcoord);

	glUseProgram(program_object);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);

	++session_id;
	last_texture_id = Core::Identifier();

	/*
	 * Flag painter as initialized. IMPORTANT
	 */
	flags |= sfInitialized;

	Reset();
}

void
GLPainter::Finalize(void)
{
	using namespace Graphics::OpenGL;

	/*
	 * Mark painter as uninitialized. IMPORTANT
	 */
	flags &= ~(sfInitialized);

	/* clear locations */
	location_color = 0;
	location_matrix = 0;
	location_model = 0;
	location_position = 0;
	location_sampler = 0;
	location_texcoord = 0;
	location_usecolor = 0;

	/* delete program */
	UnloadProgram(program_object);

	last_texture_id = Core::Identifier();
	matrix_stack.empty();

	/* clear all flags */
	flags = sfUninitialized;
}

void
GLPainter::Reset(void)
{
	using namespace Graphics;

	glClearColor(bgcolor.red(),
	             bgcolor.green(),
	             bgcolor.blue(),
	             bgcolor.alpha());
	glClear(GL_COLOR_BUFFER_BIT);

	if (matrix_stack.size() > 0)
		MMERROR("Matrix stack is not empty! COUNT=" << matrix_stack.size());
	matrix_stack.empty();

	Painter::LoadViewProjection();
}

GLuint
GLPainter::LoadShader(GLenum type, const char *src)
{
	using namespace Graphics::OpenGL;

	GLuint l_shader;
	GLint  l_status = GL_FALSE;

	if (0 == (l_shader = glCreateShader(type)))
		return(0);

	glShaderSource(l_shader, 1, &src, 0);
	glCompileShader(l_shader);

	glGetShaderiv(l_shader, GL_COMPILE_STATUS, &l_status);
	if (l_status == GL_FALSE) {
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

void
GLPainter::UnloadShader(GLuint &shader)
{
	using namespace Graphics::OpenGL;
	glDeleteShader(shader), shader = 0;
}

GLuint
GLPainter::LoadProgram(void)
{
	using namespace Graphics::OpenGL;

	GLuint l_program_object;
	GLuint l_fragment_shader;
	GLuint l_vertex_shader;
	GLint  l_status = GL_FALSE;

	if (0 == (l_vertex_shader = LoadShader(GL_VERTEX_SHADER, s_vertex_shader)))
		return(0);

	if (0 == (l_fragment_shader = LoadShader(GL_FRAGMENT_SHADER, s_fragment_shader))) {
		glDeleteShader(l_vertex_shader);
		return(0);
	}

	if (0 == (l_program_object = glCreateProgram())) {
		glDeleteShader(l_vertex_shader);
		glDeleteShader(l_fragment_shader);
		return(0);
	}

	glAttachShader(l_program_object, l_vertex_shader);
	glAttachShader(l_program_object, l_fragment_shader);

	glLinkProgram(l_program_object);
	glGetProgramiv(l_program_object, GL_LINK_STATUS, &l_status);
	if (l_status == GL_FALSE) {
		GLint l_length = 0;

		glGetProgramiv(l_program_object, GL_INFO_LOG_LENGTH, &l_length);
		if (l_length > 1) {
			char *l_log = new char[l_length];
			glGetShaderInfoLog(l_program_object, l_length, 0, l_log);
			MMERROR("Error while linking shader: " << l_log);
			delete[] l_log;
		}

		glDeleteProgram(l_program_object), l_program_object = 0;
	}

	UnloadShader(l_vertex_shader);
	UnloadShader(l_fragment_shader);
	return(l_program_object);
}

void
GLPainter::UnloadProgram(GLuint &program)
{
	using namespace Graphics::OpenGL;
	glDeleteProgram(program), program = 0;
}

void
GLPainter::CommitMatrix(void)
{
	using namespace Graphics::OpenGL;

	if (0 == (flags & sfInvalidMatrix))
		return;

	flags ^= sfInvalidMatrix;

	glUniformMatrix4fv(location_matrix, 1, GL_FALSE, matrix.data());
}

void
GLPainter::PopMatrix(void)
{
	flags |= sfInvalidMatrix;

	if (matrix_stack.size() <= 0) {
		MMWARNING("Matrix stack is empty! Ignoring pop matrix.");
		matrix = Math::Matrix4::Identity();
		return;
	}

	matrix = matrix_stack.top();
	matrix_stack.pop();
}

void
GLPainter::PushMatrix(void)
{
	matrix_stack.push(matrix);
}

void
GLPainter::LoadIdentity(void)
{
	matrix = Math::Matrix4::Identity();

	flags |= sfInvalidMatrix;
}

void
GLPainter::LoadProjection(void)
{
	using namespace Graphics;
	using namespace Math;

	matrix = Matrix4::Identity();
	matrix[Matrix4::m11] =  2.f / Viewport::Size().width;
	matrix[Matrix4::m22] =  2.f / Viewport::Size().height;
	matrix[Matrix4::m33] = -1.f;

	flags |= sfInvalidMatrix;
}

void
GLPainter::LoadViewProjection(void)
{
	using namespace Graphics;

	LoadProjection();
	matrix *= Camera::Transform().matrix(Transform::mtView);
}
void
GLPainter::Draw(const Graphics::IMesh &m, const Math::Point2 *o, size_t c)
{
	using Graphics::OpenGL::SharedTextureData;
	using Graphics::OpenGL::TextureData;
	using namespace Graphics::OpenGL;
	using namespace Graphics;

	if (0 == (flags & sfInitialized))
		return;

	GLPainter::CommitMatrix();

	float l_scale[2];
	m.scale(l_scale[0], l_scale[1]);

	/* set blending */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* color */
	const Graphics::Color &l_color = m.color();
	glUniform4f(location_color, l_color.red(), l_color.green(), l_color.blue(), l_color.alpha());

	/* set texture */
	glActiveTexture(GL_TEXTURE0);

	SharedTextureData l_texture_data =
	    m.textureData().staticCast<TextureData>();
	if (last_texture_id != l_texture_data->id()) {
		last_texture_id = l_texture_data->id();
		if (l_texture_data->isLoaded()) {
			/*
			 * TODO(gamaral): Replace with an invalidate call to
			 * asset manager (when we have one).
			 */
			if (l_texture_data->sessionId() != session_id)
				l_texture_data->reload();

			SharedTextureData l_data =
			    l_texture_data.staticCast<TextureData>();
			glBindTexture(GL_TEXTURE_2D, l_data->textureId());
			glUniform1i(location_usecolor, 0);
		}
		else glUniform1i(location_usecolor, 1);
	}

	/* prepare model amatrix */
	Graphics::Transform l_model;
	l_model.setRotation(m.rotation());
	l_model.setScale(Math::Size2f(l_scale[0], l_scale[1]));

	/* check mesh for texture coordinates */
	const bool l_tcoords = last_texture_id.uid() && m.textureCoordinateData();

	/* prepare to draw mesh */
	if (QuadMesh::Type() == m.type())
		BeginDrawQuadMesh(static_cast<const QuadMesh &>(m), l_tcoords);
	else MMWARNING("Unknown mesh type");

	/* draw mesh(es) */
	for (size_t i = 0; i < c; ++i) {
		/* update model matrix */
		l_model.setTranslation(o[i]);
		glUniformMatrix4fv(location_model, 1, GL_FALSE, l_model.matrix().data());

		/* actually draw graphic */
		if (QuadMesh::Type() == m.type())
			DrawQuadMesh();
	}

	/* cleanup */
	if (QuadMesh::Type() == m.type())
		EndDrawQuadMesh(l_tcoords);

	glDisable(GL_BLEND);
}

inline void
GLPainter::BeginDrawQuadMesh(const Graphics::QuadMesh &g, bool tcoords)
{
	using namespace Graphics::OpenGL;
	using Graphics::OpenGL::Extensions::glBindBuffer;
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
	if (l_vdata->isBuffered()) {
		if (l_vdata->sessionId() != session_id)
			l_vdata->rebuffer();

		glBindBuffer(GL_ARRAY_BUFFER, l_vdata->bufferId());
	}

	glVertexAttribPointer(location_position, 2, GL_FLOAT, GL_FALSE, 0,
	    (l_vdata->isBuffered() ? 0 : l_vdata->data()));

	/* ** texture coordinates ** */

	if (tcoords) {
		/* bind buffer */
		if (l_tcdata->isBuffered()) {
			if (l_tcdata->sessionId() != session_id)
				l_tcdata->rebuffer();

			glBindBuffer(GL_ARRAY_BUFFER, l_tcdata->bufferId());
		} else if (l_vdata->isBuffered())
			glBindBuffer(GL_ARRAY_BUFFER, 0);

		glVertexAttribPointer(location_texcoord, 2, GL_FLOAT, GL_FALSE, 0,
		    (l_tcdata->isBuffered() ? 0 : l_tcdata->data()));

		/* cleanup */
		if (l_tcdata->isBuffered())
			glBindBuffer(GL_ARRAY_BUFFER, 0);
	} else if (l_vdata->isBuffered())
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(location_position);
	if (tcoords) glEnableVertexAttribArray(location_texcoord);
}

inline void
GLPainter::DrawQuadMesh(void)
{
	glDrawArrays(GL_TRIANGLE_STRIP, 0, MARSHMALLOW_QUAD_VERTEXES);
}

inline void
GLPainter::EndDrawQuadMesh(bool tcoords)
{
	using namespace Graphics::OpenGL;

	if (tcoords) glDisableVertexAttribArray(location_texcoord);
	glDisableVertexAttribArray(location_position);
}

} /****************************************************** Anonymous Namespace */

namespace Graphics { /************************************ Graphics Namespace */

void
Painter::Initialize(void)
{
	GLPainter::Initialize();
}

void
Painter::Finalize(void)
{
	GLPainter::Finalize();
}

void
Painter::Render(void)
{
	if (0 == (GLPainter::flags & GLPainter::sfInitialized))
		return;

	GLPainter::CommitMatrix();
}

void
Painter::Reset(void)
{
	GLPainter::Reset();
}

const Color &
Painter::BackgroundColor(void)
{
	return(GLPainter::bgcolor);
}

void
Painter::SetBackgroundColor(const Color &color)
{
	GLPainter::bgcolor = color;
}

Math::Matrix4 &
Painter::Matrix(void)
{
	return(GLPainter::matrix);
}

void
Painter::LoadIdentity(void)
{
	GLPainter::LoadIdentity();
}

void
Painter::LoadProjection(void)
{
	GLPainter::LoadProjection();
}

void
Painter::LoadViewProjection(void)
{
	GLPainter::LoadViewProjection();
}

void
Painter::PushMatrix(void)
{
	GLPainter::PushMatrix();
}

void
Painter::PopMatrix(void)
{
	GLPainter::PopMatrix();
}

void
Painter::Draw(const IMesh &mesh, const Math::Point2 &origin)
{
	GLPainter::Draw(mesh, &origin, 1);
}

void
Painter::Draw(const IMesh &mesh, const Math::Point2 *origins, size_t count)
{
	GLPainter::Draw(mesh, origins, count);
}

unsigned int
Painter::SessionId(void)
{
	return(GLPainter::session_id);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

