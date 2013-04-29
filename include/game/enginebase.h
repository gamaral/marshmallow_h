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

#ifndef MARSHMALLOW_GAME_ENGINEBASE_H
#define MARSHMALLOW_GAME_ENGINEBASE_H 1

#include <core/global.h>

#include <game/config.h>
#include <game/iengine.h>

MARSHMALLOW_NAMESPACE_BEGIN

/*
 * Sleep Intervals
 */
#define MMSLEEP_DISABLED  0
#define MMSLEEP_INSOMNIAC 2
#define MMSLEEP_LITESLEEP 4
#define MMSLEEP_DEEPSLEEP 6

namespace Event { /****************************************** Event Namespace */
	class EventManager;
	struct IEventListener;
} /********************************************************** Event Namespace */

namespace Game { /******************************************** Game Namespace */

	class EngineBaseEventListener;
	class SceneManager;
	struct IFactory;

	/*! @brief Game Engine Base Class */
	class MARSHMALLOW_GAME_EXPORT
	EngineBase : public IEngine
	{
		PRIVATE_IMPLEMENTATION
		NO_ASSIGN_COPY(EngineBase);
	public:

		/*!
		 * @param fps Desired frame rate
		 * @param sleep Sleep interval
		 */
		EngineBase(int fps = MARSHMALLOW_ENGINE_FRAMERATE,
		           int sleep = MMSLEEP_DISABLED);
		virtual ~EngineBase(void);

		/*!
		 * @brief Set Event Manager
		 */
		void setEventManager(Event::EventManager *manager);

		/*!
		 * @brief Set Scene Manager
		 */
		void setSceneManager(Game::SceneManager *manager);

		/*!
		 * @brief Set Factory
		 */
		void setFactory(Game::IFactory *factory);

		/*!
		 * @brief Target frames per second
		 */
		int fps(void) const;

		/*!
		 * @brief Time that has elapsed since last tick
		 */
		MMTIME deltaTime(void) const;

		/*!
		 * @brief Actual frame rate achieved
		 */
		int frameRate(void);

	public: /* virtual */

		virtual void second(void);
		virtual void tick(float delta);

		VIRTUAL int run(void);
		VIRTUAL void stop(int exit_code = 0);

		VIRTUAL void suspend(void);
		VIRTUAL void resume(void);

		VIRTUAL Event::EventManager * eventManager(void) const;
		VIRTUAL Game::SceneManager * sceneManager(void) const;
		VIRTUAL Game::IFactory * factory(void) const;

		VIRTUAL bool initialize(void);
		VIRTUAL void finalize(void);

		VIRTUAL bool isSuspended(void) const;
		VIRTUAL bool isValid(void) const;

		VIRTUAL void render(void);
		VIRTUAL void update(float delta);

		VIRTUAL bool handleEvent(const Event::IEvent &event);
	};

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
