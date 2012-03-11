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

#include "vbo.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <windows.h>

#include "common.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace
{
}

namespace Graphics
{

namespace OpenGL
{
	namespace VBO
	{
		void
		BindBuffer(GLenum t, GLuint b)
		{
			static PFNGLBINDBUFFERARBPROC proc(0);
			if (!proc)
				proc = reinterpret_cast<PFNGLBINDBUFFERARBPROC>
				    (wglGetProcAddress(reinterpret_cast<LPCSTR>("glBindBufferARB")));
			if (proc) proc(t, b);
		}

		void
		BufferData(GLenum t, GLsizeiptr s, const GLvoid *d, GLenum u)
		{
			static PFNGLBUFFERDATAARBPROC proc(0);
			if (!proc)
				proc = reinterpret_cast<PFNGLBUFFERDATAARBPROC>
				    (wglGetProcAddress(reinterpret_cast<LPCSTR>("glBufferDataARB")));
			if (proc) proc(t, s, d, u);
		}

		void
		BufferSubData(GLenum t, GLintptr o, GLsizeiptr s, const GLvoid *d)
		{
			static PFNGLBUFFERSUBDATAARBPROC proc(0);
			if (!proc)
				proc = reinterpret_cast<PFNGLBUFFERSUBDATAARBPROC>
				    (wglGetProcAddress(reinterpret_cast<LPCSTR>("glBufferSubDataARB")));
			if (proc) proc(t, o, s, d);
		}

		void
		DeleteBuffers(GLsizei n, const GLuint *b)
		{
			static PFNGLDELETEBUFFERSARBPROC proc(0);
			if (!proc)
				proc = reinterpret_cast<PFNGLDELETEBUFFERSARBPROC>
				    (wglGetProcAddress(reinterpret_cast<LPCSTR>("glDeleteBuffersARB")));
			if (proc) proc(n, b);
		}

		void
		GenBuffers(GLsizei n, GLuint *b)
		{
			static PFNGLGENBUFFERSARBPROC proc(0);
			if (!proc)
				proc = reinterpret_cast<PFNGLGENBUFFERSARBPROC>
				    (wglGetProcAddress(reinterpret_cast<LPCSTR>("glGenBuffersARB")));
			if (proc) proc(n, b);
		}

		bool
		Supported(void)
		{
			static char s_supported(0);
			if (!s_supported)
			  s_supported = IsExtensionSupported("GL_ARB_vertex_buffer_object") ? 1 : -1;
			return(s_supported == 1);
		}
	}
}

}

MARSHMALLOW_NAMESPACE_END

