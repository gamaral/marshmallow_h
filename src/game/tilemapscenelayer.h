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

#ifndef GAME_TILEMAPSCENELAYER_H
#define GAME_TILEMAPSCENELAYER_H 1

#include "game/scenelayerbase.h"

#include "EASTL/map.h"
using namespace eastl;

#include "core/identifier.h"
#include "core/shared.h"
#include "core/type.h"
#include "graphics/ivertexdata.h"
#include "graphics/tileset.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Game
{

	/*! @brief Game Tilemap Scene Layer Class */
	class GAME_EXPORT TilemapSceneLayer : public SceneLayerBase
	{
		NO_COPY(TilemapSceneLayer);

		typedef map<int, Graphics::SharedTileset> TilesetCollection;
		TilesetCollection m_tilesets;

		typedef map<int, Graphics::SharedVertexData> VertexDataCache;
		VertexDataCache m_vertexes;

		UINT32 *m_data;

		Math::Size2f m_hrtile_size;
		Math::Size2f m_rtile_size;

		Math::Size2i m_tile_size;
		Math::Size2i m_size;

		float m_opacity;
		bool m_visible;

	public:

		TilemapSceneLayer(const Core::Identifier &identifier,
		    IScene &scene);
		virtual ~TilemapSceneLayer(void);

		Graphics::SharedTileset tileset(int index, int *offset = 0);
		void attachTileset(int offset, Graphics::SharedTileset tileset);
		void dettachTileset(int offset);

		const UINT32 * data(void) const
		    { return(m_data); }
		void setData(UINT32 *data);

		const Math::Size2i & tileSize(void) const
		    { return(m_tile_size); }
		void setTileSize(const Math::Size2i &size);

		const Math::Size2i & size(void) const
		    { return(m_size); }
		void setSize(const Math::Size2i &size);

		float opacity(void) const
		    { return (m_opacity); }
		void setOpacity(float alpha);

		bool visible(void) const
		    { return (m_visible); }
		void setVisibility(bool value);

	public: /* virtual */

		VIRTUAL const Core::Type & type(void) const
		    { return(Type()); }

		VIRTUAL void render(void);
		VIRTUAL void update(TIME) {};

	public: /* static */

		static const Core::Type & Type(void);

	protected:

		void recalculateAllVertexData();
		void recalculateRelativeTileSize();
		void recalculateVertexData(int offset);

	private: /* static */

		static const Core::Type sType;
	};
	typedef Core::Shared<TilemapSceneLayer> SharedTilemapSceneLayer;
	typedef Core::Weak<TilemapSceneLayer> WeakTilemapSceneLayer;

}

MARSHMALLOW_NAMESPACE_END

#endif
