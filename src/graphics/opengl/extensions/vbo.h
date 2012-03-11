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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef GRAPHICS_OPENGL_VBOEXT_H
#define GRAPHICS_OPENGL_VBOEXT_H 1

#include "core/environment.h"
#include "core/namespace.h"

#include "../headers.h"

#if MARSHMALLOW_VIEWPORT_GLES
#   include <GLES/glext.h>
#else
#   if defined(__linux__)
#      include <GL/glext.h>
#   elif defined(_WIN32)
#      include "glext.h"
#   elif defined(__APPLE__)
#      include <OpenGL/glext.h>
#   endif
#endif



MARSHMALLOW_NAMESPACE_BEGIN

namespace Graphics
{

namespace OpenGL
{

	namespace VBO
	{
		GRAPHICS_EXPORT
		void BindBuffer(GLenum target, GLuint buffer);
		
		GRAPHICS_EXPORT
		void BufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
		
		GRAPHICS_EXPORT
		void BufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
		
		GRAPHICS_EXPORT
		void DeleteBuffers(GLsizei n, const GLuint *buffers);
		
		GRAPHICS_EXPORT
		void GenBuffers(GLsizei n, GLuint *buffers);

		GRAPHICS_EXPORT
		bool Supported(void);
	}

}

}

MARSHMALLOW_NAMESPACE_END

#endif
