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

#ifndef MARSHMALLOW_GRAPHICS_TILESETBASE_H
#define MARSHMALLOW_GRAPHICS_TILESETBASE_H 1

#include <math/size2.h>

#include <graphics/itexturecoordinatedata.h>
#include <graphics/itexturedata.h>
#include <graphics/itileset.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */

	/*! @brief Graphics Tileset Base Class */
	class MARSHMALLOW_GRAPHICS_EXPORT
	TilesetBase : public ITileset
	{
		PRIVATE_IMPLEMENTATION;
		NO_ASSIGN_COPY(TilesetBase);
	public:
		TilesetBase(void);
		virtual ~TilesetBase(void);

		void setName(const Core::Identifier &name);
		void setTextureData(Graphics::ITextureData *tileset);
		void setTileSize(const Math::Size2i &size);
		void setMargin(int margin);
		void setSpacing(int spacing);

	public: /* virtual */

		VIRTUAL const Core::Identifier & name(void) const;
		VIRTUAL const Math::Size2i & size(void) const;
		VIRTUAL Graphics::ITextureData * textureData(void) const;
		VIRTUAL const Math::Size2i & tileSize(void) const;
		VIRTUAL int spacing(void) const;
		VIRTUAL int margin(void) const;
		VIRTUAL Graphics::ITextureCoordinateData * getTextureCoordinateData(uint16_t index);
		
	protected:

		void reset(void);
	};

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
