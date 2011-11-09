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

#ifndef GAME_PAUSESCENELAYER_H
#define GAME_PAUSESCENELAYER_H 1

#include "game/scenelayerbase.h"

#include "EASTL/list.h"
using namespace eastl;

#include "core/identifier.h"
#include "core/shared.h"
#include "core/type.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Graphics
{
	struct IMesh;
	typedef Core::Shared<IMesh> SharedMesh;
}

namespace Game
{

	/*! @brief Game Pause Scene Layer Class */
	class GAME_EXPORT PauseSceneLayer : public SceneLayerBase
	{
		Graphics::SharedMesh m_mesh;

		NO_COPY(PauseSceneLayer);

	public:

		PauseSceneLayer(const Core::Identifier &identifier,
		    IScene &scene);
		virtual ~PauseSceneLayer(void);

		Graphics::SharedMesh mesh(void) const;

	public: /* virtual */

		VIRTUAL const Core::Type & type(void) const
		    { return(Type()); }

		VIRTUAL void render(void);
		VIRTUAL void update(float) {};

	public: /* static */

		static const Core::Type & Type(void);

	private: /* static */

		static const Core::Type sType;
	};
	typedef Core::Shared<PauseSceneLayer> SharedPauseSceneLayer;
	typedef Core::Weak<PauseSceneLayer> WeakPauseSceneLayer;

}

MARSHMALLOW_NAMESPACE_END

#endif
