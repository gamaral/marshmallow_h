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

#ifndef GRAPHICS_TILESETBASE_H
#define GRAPHICS_TILESETBASE_H 1

#include "graphics/itileset.h"

#include <EASTL/map.h>
using namespace eastl;

#include "core/identifier.h"
#include "core/shared.h"

#include "math/size2.h"

#include "graphics/itexturedata.h"
#include "graphics/itexturecoordinatedata.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Graphics
{
	/*! @brief Graphics Tileset Base Class */
	class GRAPHICS_EXPORT TilesetBase : public ITileset
	{
		typedef map<int, SharedTextureCoordinateData> TextureCoordinateMap;
		TextureCoordinateMap m_cache;

		Core::Identifier m_name;
		Math::Size2i m_size;
		SharedTextureData m_texture_data;
		Math::Size2i m_tile_size;

		float m_adjust_col;
		float m_adjust_row;
		float *m_offset_col;
		float *m_offset_row;

	public:
		TilesetBase(void);
		virtual ~TilesetBase(void);

		void setName(const Core::Identifier &name);
		void setSize(const Math::Size2i &size);
		void setTextureData(const SharedTextureData &tileset);

	public: /* virtual */

		VIRTUAL const Core::Identifier & name(void) const
		    { return(m_name); }
		VIRTUAL const SharedTextureData & textureData(void) const
		    { return(m_texture_data); }
		VIRTUAL const Math::Size2i & size(void) const
		    { return(m_size); }
		VIRTUAL SharedTextureCoordinateData getTextureCoordinateData(int index);
		VIRTUAL const Math::Size2i & tileSize(void) const
		    { return(m_tile_size); }
		
		VIRTUAL bool serialize(TinyXML::TiXmlElement &node) const;
		VIRTUAL bool deserialize(TinyXML::TiXmlElement &node);

	protected:

		void reset(void);

	};
	typedef Core::Shared<TilesetBase> SharedTilesetBase;
	typedef Core::Weak<TilesetBase> WeakTilesetBase;

}

MARSHMALLOW_NAMESPACE_END

#endif
