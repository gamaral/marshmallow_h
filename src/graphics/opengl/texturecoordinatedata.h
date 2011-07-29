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

#ifndef GRAPHICS_OPENGL_TEXTURECOORDINATEDATA_H
#define GRAPHICS_OPENGL_TEXTURECOORDINATEDATA_H 1

#include "graphics/itexturecoordinatedata.h"

#include <GL/gl.h>

#include "core/identifier.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Graphics
{

namespace OpenGL
{

	/*! @brief Graphics OpenGL Texture Coordinate Data Class */
	class GRAPHICS_EXPORT TextureCoordinateData : public ITextureCoordinateData
	{
		Core::Identifier m_id;
		GLfloat *m_data;
		int m_count;
		GLuint m_bufferId;

		NO_COPY(TextureCoordinateData);
		NO_ASSIGN(TextureCoordinateData);
	public:
		TextureCoordinateData(int count);
		virtual ~TextureCoordinateData(void);

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

		VIRTUAL const Core::Identifier & id(void) const
		    { return(m_id); }

		VIRTUAL const Core::Type & type(void) const
		    { return(Type()); }

		VIRTUAL bool get(int index, float &u, float &v) const;
		VIRTUAL bool set(int index, float u, float v);

		VIRTUAL int count(void) const
		    { return(m_count); }

	public: /* static */

		static const Core::Type & Type(void);

	private: /* static */

		static const Core::Type sType;
	};
	typedef Core::Shared<TextureCoordinateData> SharedTextureCoordinateData;
	typedef Core::Weak<TextureCoordinateData> WeakTextureCoordinateData;

}

}

MARSHMALLOW_NAMESPACE_END

#endif
