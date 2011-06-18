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

#ifndef GRAPHICS_OPENGL_VERTEXDATA_H
#define GRAPHICS_OPENGL_VERTEXDATA_H 1

#include <GL/gl.h>

#include "graphics/ivertexdata.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Graphics
{

namespace OpenGL
{

	/*! @brief Graphics OpenGL Vertex Data Class */
	class GRAPHICS_EXPORT VertexData : public IVertexData
	{
		GLfloat *m_data;
		int m_count;
		GLuint m_bufferId;

	public:
		VertexData(int count);
		virtual ~VertexData(void);

		const GLfloat * data(void) const
		    { return(isBuffered() ? 0 : m_data); }

		/* VBO */

		void buffer(void);
		void unbuffer(void);

		bool isBuffered(void) const
		    { return(m_bufferId != 0); }

		GLuint bufferId(void) const
		    { return(m_bufferId); }

	public: /* virtual */
		VIRTUAL bool get(int index, float &x, float &y) const;
		VIRTUAL bool set(int index, float x, float y);

		VIRTUAL int count(void) const
		    { return(m_count); }
	};
	typedef Core::Shared<VertexData> SharedVertexData;
	typedef Core::Weak<VertexData> WeakVertexData;

}

}

MARSHMALLOW_NAMESPACE_END

#endif
