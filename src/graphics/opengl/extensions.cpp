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

#include "extensions.h"

#include <cassert>

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

MARSHMALLOW_NAMESPACE_BEGIN

namespace Graphics
{

namespace OpenGL
{
	PFNGLGETPROCADDRESSPROC glGetProcAddress = 0;

#ifndef MARSHMALLOW_OPENGL_GLES2
	PFNGLATTACHSHADERPROC glAttachShader = 0;
	PFNGLBINDBUFFERARBPROC glBindBuffer = 0;
	PFNGLBUFFERDATAARBPROC glBufferData = 0;
	PFNGLBUFFERSUBDATAARBPROC glBufferSubData = 0;
	PFNGLCOMPILESHADERARBPROC glCompileShader = 0;
	PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgram = 0;
	PFNGLCREATESHADEROBJECTARBPROC glCreateShader = 0;
	PFNGLDELETEBUFFERSARBPROC glDeleteBuffers = 0;
	PFNGLDELETEPROGRAMPROC glDeleteProgram = 0;
	PFNGLDELETESHADERPROC glDeleteShader = 0;
	PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArray = 0;
	PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArray = 0;
	PFNGLGENBUFFERSARBPROC glGenBuffers = 0;
	PFNGLGETATTRIBLOCATIONARBPROC glGetAttribLocation = 0;
	PFNGLGETPROGRAMIVARBPROC glGetProgramiv = 0;
	PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = 0;
	PFNGLGETSHADERIVPROC glGetShaderiv = 0;
	PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocation = 0;
	PFNGLLINKPROGRAMARBPROC glLinkProgram = 0;
	PFNGLSHADERSOURCEARBPROC glShaderSource = 0;
	PFNGLUNIFORM1IARBPROC glUniform1i = 0;
	PFNGLUNIFORM4FARBPROC glUniform4f = 0;
	PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fv = 0;
	PFNGLUSEPROGRAMOBJECTARBPROC glUseProgram = 0;
	PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointer = 0;
#endif

#if defined(MARSHMALLOW_OPENGL_GLX)
	PFNGLXSWAPINTERVALSGIPROC glSwapInterval = 0;
#elif defined(MARSHMALLOW_OPENGL_WGL)
	PFNGLACTIVETEXTUREARBPROC glActiveTexture = 0;
	PFNWGLSWAPINTERVALEXTPROC glSwapInterval = 0;
#endif

	void
	InitializeExtensions(PFNGLGETPROCADDRESSPROC addr)
	{
		glGetProcAddress = addr;
		assert(glGetProcAddress && "Invalid glGetProcAddress!");

#ifndef MARSHMALLOW_OPENGL_GLES2
		glAttachShader = reinterpret_cast<PFNGLATTACHSHADERPROC>
		    (glGetProcAddress("glAttachShader"));
		assert(glAttachShader);

		glBindBuffer = reinterpret_cast<PFNGLBINDBUFFERARBPROC>
		    (glGetProcAddress("glBindBufferARB"));
		assert(glBindBuffer);

		glBufferData = reinterpret_cast<PFNGLBUFFERDATAARBPROC>
		    (glGetProcAddress("glBufferDataARB"));
		assert(glBufferData);

		glBufferSubData = reinterpret_cast<PFNGLBUFFERSUBDATAARBPROC>
		    (glGetProcAddress("glBufferSubDataARB"));
		assert(glBufferSubData);

		glCompileShader = reinterpret_cast<PFNGLCOMPILESHADERARBPROC>
		    (glGetProcAddress("glCompileShaderARB"));
		assert(glCompileShader);

		glCreateProgram = reinterpret_cast<PFNGLCREATEPROGRAMOBJECTARBPROC>
		    (glGetProcAddress("glCreateProgramObjectARB"));
		assert(glCreateProgram);

		glCreateShader = reinterpret_cast<PFNGLCREATESHADEROBJECTARBPROC>
		    (glGetProcAddress("glCreateShaderObjectARB"));
		assert(glCreateShader);

		glDeleteBuffers = reinterpret_cast<PFNGLDELETEBUFFERSARBPROC>
		    (glGetProcAddress("glDeleteBuffersARB"));
		assert(glDeleteBuffers);

		glDeleteProgram = reinterpret_cast<PFNGLDELETEPROGRAMPROC>
		    (glGetProcAddress("glDeleteProgram"));
		assert(glDeleteProgram);

		glDeleteShader = reinterpret_cast<PFNGLDELETESHADERPROC>
		    (glGetProcAddress("glDeleteShader"));
		assert(glDeleteShader);

		glDisableVertexAttribArray = reinterpret_cast<PFNGLDISABLEVERTEXATTRIBARRAYARBPROC>
		    (glGetProcAddress("glDisableVertexAttribArrayARB"));
		assert(glDisableVertexAttribArray);

		glEnableVertexAttribArray = reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYARBPROC>
		    (glGetProcAddress("glEnableVertexAttribArrayARB"));
		assert(glEnableVertexAttribArray);

		glGenBuffers = reinterpret_cast<PFNGLGENBUFFERSARBPROC>
		    (glGetProcAddress("glGenBuffersARB"));
		assert(glGenBuffers);

		glGetAttribLocation = reinterpret_cast<PFNGLGETATTRIBLOCATIONARBPROC>
		    (glGetProcAddress("glGetAttribLocationARB"));
		assert(glGetAttribLocation);

		glGetProgramiv = reinterpret_cast<PFNGLGETPROGRAMIVARBPROC>
		    (glGetProcAddress("glGetProgramivARB"));
		assert(glGetProgramiv);

		glGetShaderInfoLog = reinterpret_cast<PFNGLGETSHADERINFOLOGPROC>
		    (glGetProcAddress("glGetShaderInfoLog"));
		assert(glGetShaderInfoLog);

		glGetShaderiv = reinterpret_cast<PFNGLGETSHADERIVPROC>
		    (glGetProcAddress("glGetShaderiv"));
		assert(glGetShaderiv);

		glGetUniformLocation = reinterpret_cast<PFNGLGETUNIFORMLOCATIONARBPROC>
		    (glGetProcAddress("glGetUniformLocationARB"));
		assert(glGetUniformLocation);

		glLinkProgram = reinterpret_cast<PFNGLLINKPROGRAMARBPROC>
		    (glGetProcAddress("glLinkProgramARB"));
		assert(glLinkProgram);

		glShaderSource = reinterpret_cast<PFNGLSHADERSOURCEARBPROC>
		    (glGetProcAddress("glShaderSourceARB"));
		assert(glShaderSource);

		glUniform1i = reinterpret_cast<PFNGLUNIFORM1IARBPROC>
		    (glGetProcAddress("glUniform1iARB"));
		assert(glUniform1i);

		glUniform4f = reinterpret_cast<PFNGLUNIFORM4FARBPROC>
		    (glGetProcAddress("glUniform4fARB"));
		assert(glUniform4f);

		glUniformMatrix4fv = reinterpret_cast<PFNGLUNIFORMMATRIX4FVARBPROC>
		    (glGetProcAddress("glUniformMatrix4fvARB"));
		assert(glUniformMatrix4fv);

		glUseProgram = reinterpret_cast<PFNGLUSEPROGRAMOBJECTARBPROC>
		    (glGetProcAddress("glUseProgramObjectARB"));
		assert(glUseProgram);

		glVertexAttribPointer = reinterpret_cast<PFNGLVERTEXATTRIBPOINTERARBPROC>
		    (glGetProcAddress("glVertexAttribPointerARB"));
		assert(glVertexAttribPointer);
#endif

#if defined(MARSHMALLOW_OPENGL_GLX)
		glSwapInterval = reinterpret_cast<PFNGLXSWAPINTERVALSGIPROC>
		    (glGetProcAddress("glXSwapIntervalSGI"));
#elif defined(MARSHMALLOW_OPENGL_WGL)
		glActiveTexture = reinterpret_cast<PFNGLACTIVETEXTUREARBPROC>
		    (glGetProcAddress("glActiveTextureARB"));
		assert(glActiveTexture);

		glSwapInterval = reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>
		    (glGetProcAddress("wglSwapIntervalEXT"));
#endif

	}
}

}

MARSHMALLOW_NAMESPACE_END

