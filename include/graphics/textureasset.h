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

#ifndef GRAPHICS_TEXTUREASSET_H
#define GRAPHICS_TEXTUREASSET_H 1

#include "core/iasset.h"

#include "core/identifier.h"
#include "math/size2.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Graphics
{

	/*! @brief Graphic Texture Asset Class */
	class GRAPHICS_EXPORT TextureAsset : public Core::IAsset
	{
		Core::Identifier m_id;
		Math::Size2 m_size;
		unsigned int m_texture_id;

		NO_COPY(TextureAsset);

	public:

		TextureAsset(void);
		virtual ~TextureAsset(void);

		void load(const char *filename);
		void unload(void);

		unsigned int tid(void) const
		    { return(m_texture_id); }

		const Math::Size2 &size(void) const
		    { return(m_size); }

	public: /* virtual */

		virtual const Core::Identifier & id(void) const
		    { return(m_id); }

		VIRTUAL const Core::AssetType & type(void) const
		    { return(Type); }

	public: /* operators */

		operator bool(void) const
		    { return(m_texture_id); }

	public: /* static */

		static const Core::AssetType Type;
	};
	typedef Core::Shared<TextureAsset> SharedTextureAsset;
	typedef Core::Weak<TextureAsset> WeakTextureAsset;

}

MARSHMALLOW_NAMESPACE_END

#endif
