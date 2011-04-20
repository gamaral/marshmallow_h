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

#ifndef GRAPHICS_IGRAPHIC_H
#define GRAPHICS_IGRAPHIC_H 1

#include "core/global.h"
#include "graphics/config.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Math
{
	class Vector2;
}

namespace Core
{
	template <class T> class Shared;
	template <class T> class Weak;
}

namespace Graphics
{

	class TextureAsset;
	typedef Core::Weak<TextureAsset> WeakTextureAsset;

	enum GraphicType
	{
		UnknownGraphicType = 0,
		PointGraphicType,
		LineGraphicType,
		TriangleGraphicType,
		QuadGraphicType,
		PolygonGraphicType,
		GraphicTypeMax
	};

	/*! @brief Graphics Graphic Interface */
	struct GRAPHICS_EXPORT IGraphic
	{
		virtual ~IGraphic(void) {};

		virtual const GraphicType & type(void) const = 0;

		virtual const Math::Vector2 & position(void) const = 0;
		virtual void setPosition(const Math::Vector2 &position) = 0;

		virtual const WeakTextureAsset & texture(void) const = 0;
		virtual void setTexture(WeakTextureAsset texture) = 0;

		virtual float rotation(void) const = 0;
		virtual void setRotation(float angle) = 0;

		/* TODO: Create brush and/or pen shared classes */
	};
	typedef Core::Shared<IGraphic> SharedGraphic;
	typedef Core::Weak<IGraphic> WeakGraphic;

}

MARSHMALLOW_NAMESPACE_END

#endif
