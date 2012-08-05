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

#ifndef GRAPHICS_OPENGL_EXTENSIONS_H
#define GRAPHICS_OPENGL_EXTENSIONS_H 1

#include "core/namespace.h"

#include "headers.h"

/* used to skip functions we know we should support already. */
#if defined(GL_VERSION_2_0) || defined(GL_ES_VERSION_2_0)
#  define MMGL_VERSION_2_0 1
#endif

#ifdef MARSHMALLOW_OPENGL_ES2
#  include <GLES2/gl2ext.h>
#else
#  if defined(MARSHMALLOW_OPENGL_X11)
#    include <GL/glext.h>
#    include <GL/glx.h>
#    include <GL/glxext.h>
#  elif defined(MARSHMALLOW_OPENGL_WGL)
#    include "GL/glext.h"
#    include "GL/wglext.h"
#  elif defined(__APPLE__)
#    include <OpenGL/glext.h>
#  else
#    include <GL/glext.h>
#  endif
#endif

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */

	/*!
	 * Must be implemented by viewport.
	 */
	typedef void (*PFNPROC)(void);
	PFNPROC glGetProcAddress(const char *func);

namespace OpenGL { /****************************** Graphics::OpenGL Namespace */

	/*
	 * Required, no checks needed.
	 */
#ifndef MMGL_VERSION_2_0
	extern PFNGLATTACHSHADERPROC glAttachShader;
	extern PFNGLCOMPILESHADERPROC glCompileShader;
	extern PFNGLCREATEPROGRAMPROC glCreateProgram;
	extern PFNGLCREATESHADERPROC glCreateShader;
	extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
	extern PFNGLDELETESHADERPROC glDeleteShader;
	extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
	extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
	extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
	extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
	extern PFNGLGETSHADERIVPROC glGetShaderiv;
	extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
	extern PFNGLLINKPROGRAMPROC glLinkProgram;
	extern PFNGLSHADERSOURCEPROC glShaderSource;
	extern PFNGLUNIFORM1IPROC glUniform1i;
	extern PFNGLUNIFORM4FPROC glUniform4f;
	extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
	extern PFNGLUSEPROGRAMPROC glUseProgram;
	extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
	extern PFNGLACTIVETEXTUREPROC glActiveTexture;
#endif

namespace Extensions { /************** Graphics::OpenGL::Extensions Namespace */

	/*
	 * The following are optional.
	 */

	/* GL_ARB_vertex_buffer_object */

	typedef void (*PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
	typedef void (*PFNGLBUFFERDATAARBPROC) (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
	typedef void (*PFNGLBUFFERSUBDATAARBPROC) (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
	typedef void (*PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint *buffers);
	typedef void (*PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint *buffers);

	extern PFNGLBINDBUFFERARBPROC glBindBuffer;
	extern PFNGLBUFFERDATAARBPROC glBufferData;
	extern PFNGLBUFFERSUBDATAARBPROC glBufferSubData;
	extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffers;
	extern PFNGLGENBUFFERSARBPROC glGenBuffers;

	/* GL_ARB_framebuffer_object */

	typedef void (*PFNGLGENERATEMIPMAPPROC) (GLenum target);

	extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;

	/*
	 * The following are optional-platform dependent
	 */

#if defined(GLX_MESA_swap_control)
	extern PFNGLXSWAPINTERVALMESAPROC glXSwapIntervalMESA;
	extern PFNGLXGETSWAPINTERVALMESAPROC glXGetSwapIntervalMESA;
#endif

#if defined(GLX_SGI_swap_control)
	extern PFNGLXSWAPINTERVALSGIPROC glxSwapInterval;
#endif

#if defined(WGL_EXT_swap_control)
	extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
	extern PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;
#endif

	/*!
	 * Must be called by the viewport after context creation.
	 * @param extensions Viewport extensions string.
	 */
	void Initialize(const char *extensions = 0);

	/*!
	 * Must be called by the viewport after context destruction.
	 */
	void Finalize(void);

	/*!
	 * Returns true if extension is supported.
	 */
	bool Supported(const char *extension);

} /*********************************** Graphics::OpenGL::Extensions Namespace */
} /*********************************************** Graphics::OpenGL Namespace */
} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
