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

#ifndef MARSHMALLOW_GAME_ENGINEBASE_H
#define MARSHMALLOW_GAME_ENGINEBASE_H 1

#include <game/iengine.h>

#include <core/global.h>

#include <game/config.h>

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
	typedef Core::Shared<EventManager> SharedEventManager;
	
	struct IEventListener;
	typedef Core::Shared<IEventListener> SharedEventListener;
} /********************************************************** Event Namespace */

namespace Game { /******************************************** Game Namespace */

	class SceneManager;
	typedef Core::Shared<SceneManager> SharedSceneManager;

	struct IFactory;
	typedef Core::Shared<IFactory> SharedFactory;

	class EngineBaseEventListener;

	/*! @brief Game Engine Base Class */
	class MARSHMALLOW_GAME_EXPORT
	EngineBase : public IEngine
	{
		struct Private;
		Private *m_p;

		NO_ASSIGN_COPY(EngineBase);
	public:

		/*!
		 * @param fps Desired frame rate
		 * @param sleep Sleep interval
		 */
		EngineBase(int fps = MARSHMALLOW_ENGINE_FRAMERATE,
		           int sleep = MMSLEEP_INSOMNIAC);
		virtual ~EngineBase(void);

		/*!
		 * @brief Set Event Manager
		 */
		void setEventManager(const Event::SharedEventManager &manager);

		/*!
		 * @brief Set Scene Manager
		 */
		void setSceneManager(const SharedSceneManager &manager);

		/*!
		 * @brief Set Factory
		 */
		void setFactory(const SharedFactory &factory);

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

		VIRTUAL Event::SharedEventManager eventManager(void) const;
		VIRTUAL SharedSceneManager sceneManager(void) const;
		VIRTUAL SharedFactory factory(void) const;

		VIRTUAL bool initialize(void);
		VIRTUAL void finalize(void);

		VIRTUAL bool isSuspended(void) const;
		VIRTUAL bool isValid(void) const;

		VIRTUAL void render(void);
		VIRTUAL void update(float delta);

		VIRTUAL bool serialize(XMLElement &node) const;
		VIRTUAL bool deserialize(XMLElement &node);

		VIRTUAL bool handleEvent(const Event::IEvent &event);
	};

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
