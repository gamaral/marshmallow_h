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

#ifndef MARSHMALLOW_EXTRA_TMXLOADER_H
#define MARSHMALLOW_EXTRA_TMXLOADER_H 1

#include <core/shared.h>

#include <math/size2.h>

#include <game/iscenelayer.h>

#include <list>

MARSHMALLOW_NAMESPACE_BEGIN

namespace Graphics
{
	struct ITileset;
	typedef Core::Shared<ITileset> SharedTileset;
}

namespace Game
{
	typedef std::list<SharedSceneLayer> SharedSceneLayerList;
}

namespace Extra
{
	/*! @brief Extra TMX Loader Class */
	class MARSHMALLOW_EXTRA_EXPORT
	TMXLoader
	{
		struct Private;
		Private *m_p;

		NO_ASSIGN_COPY(TMXLoader);
	public:

		TMXLoader(Game::IScene &scene);
		virtual ~TMXLoader(void);

		bool load(const char *file);
		bool isLoaded(void) const;

		const Game::SharedSceneLayerList & layers(void) const;

	private:

		bool processLayer(XMLElement &element);
		bool processMap(XMLElement &element);
		bool processObjectGroup(XMLElement &element);
		bool processTileset(XMLElement &element);
	};
	typedef Core::Shared<TMXLoader> SharedTMXLoader;
	typedef Core::Weak<TMXLoader> WeakTMXLoader;
}

MARSHMALLOW_NAMESPACE_END

#endif
