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

#include <core/shared.h>

MARSHMALLOW_NAMESPACE_BEGIN

#define MARSHMALLOW_BUSYWAIT   1
#define MARSHMALLOW_LITESLEEP  4
#define MARSHMALLOW_DEEPSLEEP  8

namespace Event
{
	class EventManager;
	typedef Core::Shared<EventManager> SharedEventManager;
	
	struct IEventListener;
	typedef Core::Shared<IEventListener> SharedEventListener;
}

namespace Game
{
	class SceneManager;
	typedef Core::Shared<SceneManager> SharedSceneManager;

	struct IFactory;
	typedef Core::Shared<IFactory> SharedFactory;

	class EngineBaseEventListener;

	/*! @brief Game Engine Base Class */
	class MARSHMALLOW_GAME_EXPORT
	EngineBase : public IEngine
	{
		static EngineBase *s_instance;

		struct Private;
		Private *m_p;

		NO_ASSIGN_COPY(EngineBase);
	public:

		/*!
		 * @param fps Desired frame rate
		 * @param ups Desired update rate
		 * @param suspend_interval Allow engine to suspend during wait
		 */
		EngineBase(int fps, int ups, int suspend_interval);
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
		 * @brief Target updates per second
		 */
		int ups(void) const;

		/*!
		 * @brief Time that has elapsed since last tick
		 */
		MMTIME deltaTime(void) const;

		/*!
		 * @brief Actual frame rate achieved
		 */
		int frameRate(void);

	public: /* virtual */

		VIRTUAL int run(void);
		VIRTUAL void stop(int exit_code = 0);

		VIRTUAL Event::SharedEventManager eventManager(void) const;
		VIRTUAL SharedSceneManager sceneManager(void) const;
		VIRTUAL SharedFactory factory(void) const;

		VIRTUAL bool initialize(void);
		VIRTUAL void finalize(void);

		VIRTUAL bool isValid(void) const;

		VIRTUAL void second(void);
		VIRTUAL void tick(void);

		VIRTUAL void render(void);
		VIRTUAL void update(float delta);

		VIRTUAL bool serialize(XMLElement &node) const;
		VIRTUAL bool deserialize(XMLElement &node);

		VIRTUAL bool handleEvent(const Event::IEvent &event);

	public: /* static */

		static EngineBase * Instance(void);
	};
}

MARSHMALLOW_NAMESPACE_END

#endif
