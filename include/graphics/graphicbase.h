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

#ifndef GRAPHICS_GRAPHICBASE_H
#define GRAPHICS_GRAPHICBASE_H 1

#include "graphics/igraphic.h"

#include "core/shared.h"
#include "graphics/color.h"
#include "graphics/textureasset.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Graphics
{

	/*! @brief Graphics Quad Graphic class */
	class GRAPHICS_EXPORT GraphicBase : public IGraphic
	{
		Graphics::Color m_color;
		Graphics::SharedTextureAsset m_texture;
		float m_rotation;

		NO_COPY(GraphicBase);

	public:

		GraphicBase(void);
		virtual ~GraphicBase(void) {};

		void setColor(const Graphics::Color &color);

		void setTexture(SharedTextureAsset texture);

		void setRotation(float angle);

	public: /* virtual */

		VIRTUAL const Graphics::Color & color(void) const
		    { return(m_color); }

		VIRTUAL const SharedTextureAsset & texture(void) const
		    { return(m_texture); }

		VIRTUAL float rotation(void) const
		    { return(m_rotation); }

		VIRTUAL bool serialize(TinyXML::TiXmlElement &node) const;
		VIRTUAL bool deserialize(TinyXML::TiXmlElement &node);
	};
	typedef Core::Shared<GraphicBase> SharedGraphicBase;
	typedef Core::Weak<GraphicBase> WeakGraphicBase;

}

MARSHMALLOW_NAMESPACE_END

#endif
