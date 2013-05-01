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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef MARSHMALLOW_GRAPHICS_OPENGL_TEXTUREDATA_H
#define MARSHMALLOW_GRAPHICS_OPENGL_TEXTUREDATA_H 1

#include "graphics/itexturedata.h"

#include "core/identifier.h"

#include "math/size2.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */
namespace OpenGL { /****************************** Graphics::OpenGL Namespace */

	/*! @brief Graphic OpenGL Texture Data Class */
	class TextureData : public ITextureData
	{
		Core::Identifier m_id;
		Math::Size2i m_size;
		unsigned int m_session_id;
		unsigned int m_texture_id;
		ScaleMode m_min;
		ScaleMode m_mag;

		NO_ASSIGN_COPY(TextureData);
	public:

		TextureData(void);
		virtual ~TextureData(void);

		unsigned int sessionId(void) const
		    { return(m_session_id); };

		unsigned int textureId(void) const
		    { return(m_texture_id); }

		bool reload(void);

	public: /* virtual */

		VIRTUAL const Core::Identifier & id(void) const
		    { return(m_id); }

		VIRTUAL const Core::Type & type(void) const
		    { return(Type()); }

		VIRTUAL bool load(const Core::Identifier &id,
		                  ScaleMode min, ScaleMode mag);
		VIRTUAL void unload(void);

		VIRTUAL bool isLoaded(void) const
		    { return(m_texture_id != 0); }

		VIRTUAL ScaleMode minificationMode(void) const
		    { return(m_min); }
		VIRTUAL ScaleMode magnificationMode(void) const
		    { return(m_mag); }

		VIRTUAL const Math::Size2i & size(void) const
		    { return(m_size); }

	public: /* static */

		static const Core::Type & Type(void);
	};

} /*********************************************** Graphics::OpenGL Namespace */
} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
