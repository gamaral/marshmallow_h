/*
 * Copyright (c) 2011-2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
 * All rights reserved.
 *
 * This file is part of Marshmallow Game Engine.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the project as a whole.
 */

#include "extensions.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <set>
#include <string>

#include <cassert>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */
namespace OpenGL { /****************************** Graphics::OpenGL Namespace */
namespace { /************************ Graphics::OpenGL::<anonymous> Namespace */

std::set<std::string> s_supported_extensions;

void
ParseExtensionsString(const char *e)
{
	if (!e) return;

	bool l_next = false;
	size_t l_start = 0;
	for (size_t i = 0; e[i] != '\0'; ++i)
		switch(e[i]) {
		case ' ':
			if (l_next) continue;

			s_supported_extensions.insert
			    (std::string(e, l_start, i - l_start));

			l_next = true;
		break;

		default:
			if (!l_next) continue;

			l_start = i;
			l_next = false;
		}
}

} /********************************** Graphics::OpenGL::<anonymous> Namespace */

#ifndef MMGL_VERSION_2_0
/* required */
PFNGLATTACHSHADERPROC glAttachShader = 0;
PFNGLCOMPILESHADERPROC glCompileShader = 0;
PFNGLCREATEPROGRAMPROC glCreateProgram = 0;
PFNGLCREATESHADERPROC glCreateShader = 0;
PFNGLDELETEPROGRAMPROC glDeleteProgram = 0;
PFNGLDELETESHADERPROC glDeleteShader = 0;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = 0;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = 0;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = 0;
PFNGLGETPROGRAMIVPROC glGetProgramiv = 0;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = 0;
PFNGLGETSHADERIVPROC glGetShaderiv = 0;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = 0;
PFNGLLINKPROGRAMPROC glLinkProgram = 0;
PFNGLSHADERSOURCEPROC glShaderSource = 0;
PFNGLUNIFORM1IPROC glUniform1i = 0;
PFNGLUNIFORM4FPROC glUniform4f = 0;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = 0;
PFNGLUSEPROGRAMPROC glUseProgram = 0;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = 0;
PFNGLACTIVETEXTUREPROC glActiveTexture = 0;
#endif

namespace Extensions { /********************** Graphics::Extensions Namespace */

/* GL_ARB_vertex_buffer_object */
PFNGLBINDBUFFERARBPROC glBindBuffer = 0;
PFNGLBUFFERDATAARBPROC glBufferData = 0;
PFNGLBUFFERSUBDATAARBPROC glBufferSubData = 0;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffers = 0;
PFNGLGENBUFFERSARBPROC glGenBuffers = 0;

/* GL_ARB_framebuffer_object */
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = 0;

/* optional-platform dependent */
#if defined(GLX_MESA_swap_control)
	PFNGLXSWAPINTERVALMESAPROC glXSwapIntervalMESA = 0;
	PFNGLXGETSWAPINTERVALMESAPROC glXGetSwapIntervalMESA = 0;
#endif

#if defined(GLX_EXT_swap_control)
	PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT = 0;
#endif

#if defined(GLX_SGI_swap_control)
	PFNGLXSWAPINTERVALSGIPROC glXSwapIntervalSGI = 0;
#endif

#if defined(WGL_EXT_swap_control)
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = 0;
	PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = 0;
#endif

} /*********************************** Graphics::OpenGL::Extensions Namespace */

void
Extensions::Initialize(const char *extensions)
{
	/* parse extensions strings */
	s_supported_extensions.clear();
	ParseExtensionsString(reinterpret_cast<const char *>
	    (glGetString(GL_EXTENSIONS)));
	ParseExtensionsString(extensions);

	/* wgl extension strings */
#if defined(WGL_EXT_extensions_string)
	PFNWGLGETEXTENSIONSSTRINGEXTPROC wglGetExtensionsStringEXT =
	    reinterpret_cast<PFNWGLGETEXTENSIONSSTRINGEXTPROC>
	        (glGetProcAddress("wglGetExtensionsStringEXT"));
	if (wglGetExtensionsStringEXT) ParseExtensionsString(wglGetExtensionsStringEXT());
#endif

#ifndef MMGL_VERSION_2_0
	glAttachShader = reinterpret_cast<PFNGLATTACHSHADERPROC>
	    (glGetProcAddress("glAttachShader"));
	assert(glAttachShader);

	glCompileShader = reinterpret_cast<PFNGLCOMPILESHADERPROC>
	    (glGetProcAddress("glCompileShader"));
	assert(glCompileShader);

	glCreateProgram = reinterpret_cast<PFNGLCREATEPROGRAMPROC>
	    (glGetProcAddress("glCreateProgram"));
	assert(glCreateProgram);

	glCreateShader = reinterpret_cast<PFNGLCREATESHADERPROC>
	    (glGetProcAddress("glCreateShader"));
	assert(glCreateShader);

	glDeleteProgram = reinterpret_cast<PFNGLDELETEPROGRAMPROC>
	    (glGetProcAddress("glDeleteProgram"));
	assert(glDeleteProgram);

	glDeleteShader = reinterpret_cast<PFNGLDELETESHADERPROC>
	    (glGetProcAddress("glDeleteShader"));
	assert(glDeleteShader);

	glDisableVertexAttribArray = reinterpret_cast<PFNGLDISABLEVERTEXATTRIBARRAYPROC>
	    (glGetProcAddress("glDisableVertexAttribArray"));
	assert(glDisableVertexAttribArray);

	glEnableVertexAttribArray = reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYPROC>
	    (glGetProcAddress("glEnableVertexAttribArray"));
	assert(glEnableVertexAttribArray);

	glGetAttribLocation = reinterpret_cast<PFNGLGETATTRIBLOCATIONPROC>
	    (glGetProcAddress("glGetAttribLocation"));
	assert(glGetAttribLocation);

	glGetProgramiv = reinterpret_cast<PFNGLGETPROGRAMIVPROC>
	    (glGetProcAddress("glGetProgramiv"));
	assert(glGetProgramiv);

	glGetShaderInfoLog = reinterpret_cast<PFNGLGETSHADERINFOLOGPROC>
	    (glGetProcAddress("glGetShaderInfoLog"));
	assert(glGetShaderInfoLog);

	glGetShaderiv = reinterpret_cast<PFNGLGETSHADERIVPROC>
	    (glGetProcAddress("glGetShaderiv"));
	assert(glGetShaderiv);

	glGetUniformLocation = reinterpret_cast<PFNGLGETUNIFORMLOCATIONPROC>
	    (glGetProcAddress("glGetUniformLocation"));
	assert(glGetUniformLocation);

	glLinkProgram = reinterpret_cast<PFNGLLINKPROGRAMPROC>
	    (glGetProcAddress("glLinkProgram"));
	assert(glLinkProgram);

	glShaderSource = reinterpret_cast<PFNGLSHADERSOURCEPROC>
	    (glGetProcAddress("glShaderSource"));
	assert(glShaderSource);

	glUniform1i = reinterpret_cast<PFNGLUNIFORM1IPROC>
	    (glGetProcAddress("glUniform1i"));
	assert(glUniform1i);

	glUniform4f = reinterpret_cast<PFNGLUNIFORM4FPROC>
	    (glGetProcAddress("glUniform4f"));
	assert(glUniform4f);

	glUniformMatrix4fv = reinterpret_cast<PFNGLUNIFORMMATRIX4FVPROC>
	    (glGetProcAddress("glUniformMatrix4fv"));
	assert(glUniformMatrix4fv);

	glUseProgram = reinterpret_cast<PFNGLUSEPROGRAMPROC>
	    (glGetProcAddress("glUseProgram"));
	assert(glUseProgram);

	glVertexAttribPointer = reinterpret_cast<PFNGLVERTEXATTRIBPOINTERPROC>
	    (glGetProcAddress("glVertexAttribPointer"));
	assert(glVertexAttribPointer);

	glActiveTexture = reinterpret_cast<PFNGLACTIVETEXTUREPROC>
	    (glGetProcAddress("glActiveTexture"));
	assert(glActiveTexture);

	if (Supported("GL_ARB_vertex_buffer_object")) {
		glBindBuffer = reinterpret_cast<PFNGLBINDBUFFERARBPROC>
		    (glGetProcAddress("glBindBufferARB"));

		glDeleteBuffers = reinterpret_cast<PFNGLDELETEBUFFERSARBPROC>
		    (glGetProcAddress("glDeleteBuffersARB"));

		glGenBuffers = reinterpret_cast<PFNGLGENBUFFERSARBPROC>
		    (glGetProcAddress("glGenBuffersARB"));

		glBufferData = reinterpret_cast<PFNGLBUFFERDATAARBPROC>
		    (glGetProcAddress("glBufferDataARB"));

		glBufferSubData = reinterpret_cast<PFNGLBUFFERSUBDATAARBPROC>
		    (glGetProcAddress("glBufferSubDataARB"));
	}

	if (Supported("GL_ARB_framebuffer_object")) {
		glGenerateMipmap = reinterpret_cast<PFNGLGENERATEMIPMAPPROC>
		    (glGetProcAddress("glGenerateMipmap"));
	}
#else
	/* GL_ARB_vertex_buffer_object */
	glBindBuffer = reinterpret_cast<PFNGLBINDBUFFERARBPROC>(&::glBindBuffer);
	glDeleteBuffers = reinterpret_cast<PFNGLDELETEBUFFERSARBPROC>(&::glDeleteBuffers);
	glGenBuffers = reinterpret_cast<PFNGLGENBUFFERSARBPROC>(&::glGenBuffers);
	glBufferData = reinterpret_cast<PFNGLBUFFERDATAARBPROC>(&::glBufferData);
	glBufferSubData = reinterpret_cast<PFNGLBUFFERSUBDATAARBPROC>(&::glBufferSubData);

	/* GL_ARB_framebuffer_object */
	glGenerateMipmap = reinterpret_cast<PFNGLGENERATEMIPMAPPROC>(&::glGenerateMipmap);
#endif

#if defined(GLX_MESA_swap_control)
	glXSwapIntervalMESA = reinterpret_cast<PFNGLXSWAPINTERVALMESAPROC>
	    (glGetProcAddress("glXSwapIntervalMESA"));
	glXGetSwapIntervalMESA = reinterpret_cast<PFNGLXGETSWAPINTERVALMESAPROC>
	    (glGetProcAddress("glXGetSwapIntervalMESA"));
#endif

#if defined(GLX_EXT_swap_control)
	glXSwapIntervalEXT = reinterpret_cast<PFNGLXSWAPINTERVALEXTPROC>
	    (glGetProcAddress("glXSwapIntervalEXT"));
#endif

#if defined(GLX_SGI_swap_control)
	glXSwapIntervalSGI = reinterpret_cast<PFNGLXSWAPINTERVALSGIPROC>
	    (glGetProcAddress("glXSwapIntervalSGI"));
#endif

#if defined(WGL_EXT_swap_control)
	wglSwapIntervalEXT = reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>
	    (glGetProcAddress("wglSwapIntervalEXT"));
	wglGetSwapIntervalEXT = reinterpret_cast<PFNWGLGETSWAPINTERVALEXTPROC>
	    (glGetProcAddress("wglGetSwapIntervalEXT"));
#endif
}

void
Extensions::Finalize(void)
{
	/*
	 * We don't keep the pointer values since we want to catch stray GL
	 * calls.
	 */
#ifndef MMGL_VERSION_2_0
	glAttachShader = 0;
	glCompileShader = 0;
	glCreateProgram = 0;
	glCreateShader = 0;
	glDeleteProgram = 0;
	glDeleteShader = 0;
	glDisableVertexAttribArray = 0;
	glEnableVertexAttribArray = 0;
	glGetAttribLocation = 0;
	glGetProgramiv = 0;
	glGetShaderInfoLog = 0;
	glGetShaderiv = 0;
	glGetUniformLocation = 0;
	glLinkProgram = 0;
	glShaderSource = 0;
	glUniform1i = 0;
	glUniform4f = 0;
	glUniformMatrix4fv = 0;
	glUseProgram = 0;
	glVertexAttribPointer = 0;
	glActiveTexture = 0;
#endif

	/* GL_ARB_vertex_buffer_object */
	glBindBuffer = 0;
	glBufferData = 0;
	glBufferSubData = 0;
	glDeleteBuffers = 0;
	glGenBuffers = 0;

	/* GL_ARB_framebuffer_object */
	glGenerateMipmap = 0;

	/* optional-platform dependent */
#if defined(GLX_MESA_swap_control)
	glXSwapIntervalMESA = 0;
	glXGetSwapIntervalMESA = 0;
#endif

#if defined(GLX_EXT_swap_control)
	glXSwapIntervalEXT = 0;
#endif

#if defined(GLX_SGI_swap_control)
	glXSwapIntervalSGI = 0;
#endif

#if defined(WGL_EXT_swap_control)
	wglSwapIntervalEXT = 0;
	wglGetSwapIntervalEXT = 0;
#endif
}

bool
Extensions::Supported(const char *e)
{
	static std::set<std::string>::const_iterator s_end =
		s_supported_extensions.end();

	return(s_supported_extensions.find(e) != s_end);
}

} /*********************************************** Graphics::OpenGL Namespace */
} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

