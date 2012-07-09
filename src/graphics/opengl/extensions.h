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

#ifdef MARSHMALLOW_OPENGL_GLES2
#  include "extensions/gl2ext.h"
#else
#  include "extensions/glext.h"
#endif

#if defined(MARSHMALLOW_OPENGL_GLX)
#  include <GL/glx.h>
#  include <GL/glxext.h>
#elif defined(MARSHMALLOW_OPENGL_WGL)
#  include "extensions/wglext.h"
#endif

MARSHMALLOW_NAMESPACE_BEGIN

namespace Graphics
{

namespace OpenGL
{
	typedef void (*PFNPROC)(void);
	typedef PFNPROC (*PFNGLGETPROCADDRESSPROC)(const char*);
	extern PFNGLGETPROCADDRESSPROC glGetProcAddress;

	/*
	 * Must be called by the viewport.
	 */
	void InitializeExtensions(PFNGLGETPROCADDRESSPROC addr);

#ifndef MARSHMALLOW_OPENGL_GLES2
	extern PFNGLATTACHSHADERPROC glAttachShader;
	extern PFNGLBINDBUFFERARBPROC glBindBuffer;
	extern PFNGLBUFFERDATAARBPROC glBufferData;
	extern PFNGLBUFFERSUBDATAARBPROC glBufferSubData;
	extern PFNGLCOMPILESHADERARBPROC glCompileShader;
	extern PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgram;
	extern PFNGLCREATESHADEROBJECTARBPROC glCreateShader;
	extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffers;
	extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
	extern PFNGLDELETESHADERPROC glDeleteShader;
	extern PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArray;
	extern PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArray;
	extern PFNGLGENBUFFERSARBPROC glGenBuffers;
	extern PFNGLGETATTRIBLOCATIONARBPROC glGetAttribLocation;
	extern PFNGLGETPROGRAMIVARBPROC glGetProgramiv;
	extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
	extern PFNGLGETSHADERIVPROC glGetShaderiv;
	extern PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocation;
	extern PFNGLLINKPROGRAMARBPROC glLinkProgram;
	extern PFNGLSHADERSOURCEARBPROC glShaderSource;
	extern PFNGLUNIFORM1IARBPROC glUniform1i;
	extern PFNGLUNIFORM4FARBPROC glUniform4f;
	extern PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fv;
	extern PFNGLUSEPROGRAMOBJECTARBPROC glUseProgram;
	extern PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointer;
#endif

#if defined(MARSHMALLOW_OPENGL_GLX)
	extern PFNGLXSWAPINTERVALSGIPROC glSwapInterval;
#elif defined(MARSHMALLOW_OPENGL_WGL)
	extern PFNGLACTIVETEXTUREARBPROC glActiveTexture;
	extern PFNWGLSWAPINTERVALEXTPROC glSwapInterval;
#endif

}

}

MARSHMALLOW_NAMESPACE_END

#endif
