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

#ifndef MARSHMALLOW_GRAPHICS_ITILESET_H
#define MARSHMALLOW_GRAPHICS_ITILESET_H 1

#include <core/iserializable.h>

#include <core/fd.h>
#include <graphics/config.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Math { /******************************************** Math Namespace */
	template <typename T> class Size2;
	typedef Size2<int> Size2i;
} /*********************************************************** Math Namespace */

namespace Graphics { /************************************ Graphics Namespace */

	struct ITextureCoordinateData;
	typedef Core::Shared<ITextureCoordinateData> SharedTextureCoordinateData;

	struct ITextureData;
	typedef Core::Shared<ITextureData> SharedTextureData;

	/*! @brief Graphics Tileset Interface */
	struct ITileset : public Core::ISerializable
	{
		virtual ~ITileset(void) {};

		virtual const Core::Identifier & name(void) const = 0;
		virtual const Math::Size2i & size(void) const = 0;
		virtual const SharedTextureData & textureData(void) const = 0;
		virtual const Math::Size2i & tileSize(void) const = 0;
		virtual int spacing(void) const = 0;
		virtual int margin(void) const = 0;

		/*! @brief Get tile texture coordinate data
		 *  @param index Tile index, top-to-bottom wrapping from left-to-right.
		 */
		virtual SharedTextureCoordinateData getTextureCoordinateData(uint16_t index) = 0;
	};
	typedef Core::Shared<ITileset> SharedTileset;
	typedef Core::Weak<ITileset> WeakTileset;

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
