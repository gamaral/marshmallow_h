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

#ifndef MARSHMALLOW_GRAPHICS_ITILESET_H
#define MARSHMALLOW_GRAPHICS_ITILESET_H 1

#include <core/iserializable.h>

#include <graphics/config.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Math { /******************************************** Math Namespace */
	template <typename T> struct Size2;
	typedef Size2<int> Size2i;
} /*********************************************************** Math Namespace */

namespace Graphics { /************************************ Graphics Namespace */

	struct ITextureCoordinateData;
	struct ITextureData;

	/*! @brief Graphics Tileset Interface */
	struct MARSHMALLOW_GRAPHICS_EXPORT
	ITileset : public Core::ISerializable
	{
		virtual ~ITileset(void);

		virtual const Core::Identifier & name(void) const = 0;
		virtual const Math::Size2i & size(void) const = 0;
		virtual ITextureData * textureData(void) const = 0;
		virtual const Math::Size2i & tileSize(void) const = 0;
		virtual int spacing(void) const = 0;
		virtual int margin(void) const = 0;

		/*! @brief Get tile texture coordinate data
		 *  @param index Tile index, top-to-bottom wrapping from left-to-right.
		 */
		virtual ITextureCoordinateData * getTextureCoordinateData(uint16_t index) = 0;
	};

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
