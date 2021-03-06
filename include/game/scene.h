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

#ifndef MARSHMALLOW_GAME_SCENE_H
#define MARSHMALLOW_GAME_SCENE_H 1

#include <core/global.h>

#include <game/iscene.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

	/*! @brief Game Scene Base Class */
	class MARSHMALLOW_GAME_EXPORT
	Scene : public IScene
	{
		PRIVATE_IMPLEMENTATION
		NO_ASSIGN_COPY(Scene);
	public:

		Scene(const Core::Identifier &identifier);
		virtual ~Scene(void);

		bool isActive(void) const;

		void setBackground(const Graphics::Color &color);

	public: /* reimp */

		VIRTUAL const Core::Identifier & id(void) const;

		VIRTUAL void pushLayer(Game::ISceneLayer *layer);
		VIRTUAL Game::ISceneLayer * popLayer(void);

		VIRTUAL void removeLayer(Game::ISceneLayer *layer);
		VIRTUAL bool removeLayer(const Core::Identifier &identifier);

		VIRTUAL Game::ISceneLayer * getLayer(const Core::Identifier &identifier) const;
		VIRTUAL Game::ISceneLayer * getLayerType(const Core::Type &type) const;

		VIRTUAL const Game::SceneLayerList & getLayers(void) const;

		VIRTUAL const Graphics::Color & background(void) const;

		VIRTUAL void activate(void);
		VIRTUAL void deactivate(void);

		VIRTUAL void render(void);
		VIRTUAL void update(float delta);

		VIRTUAL const Core::Type & type(void) const
		    { return(Type()); }

	public: /* static */

		static const Core::Type & Type(void);
	};

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
