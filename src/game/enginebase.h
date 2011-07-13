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

#ifndef GAME_ENGINEBASE_H
#define GAME_ENGINEBASE_H 1

#include "game/iengine.h"

#include "core/shared.h"

MARSHMALLOW_NAMESPACE_BEGIN

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
	class GAME_EXPORT EngineBase : public IEngine
	{
		static EngineBase *s_instance;
		Event::SharedEventManager  m_event_manager;
		Game::SharedSceneManager   m_scene_manager;
		Event::SharedEventListener m_event_proxy;
		Game::SharedFactory        m_factory;
		float  m_fps;
		float  m_ups;
		TIME   m_delta_time;
		int    m_exit_code;
		int    m_frame_rate;
		bool   m_suspendable;
		bool   m_running;

		NO_COPY(EngineBase);

	public:

		/*!
		 * @param fps Desired frame rate
		 * @param ups Desired update rate
		 * @param suspendable Allow engine to sleep
		 */
		EngineBase(float fps = 60.0, float ups = 60.0,
		    bool suspendable = false);
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
		float fps(void) const
		    { return(m_fps); }

		/*!
		 * @brief Target updates per second
		 */
		float ups(void) const
		    { return(m_ups); }

		/*!
		 * @brief Time that has elapsed since last tick
		 */
		TIME deltaTime(void) const
		    { return(m_delta_time); }

		/*!
		 * @brief Actual frame rate achieved
		 */
		int frameRate(void)
		    { return(m_frame_rate); }

	public: /* virtual */

		VIRTUAL int run(void);
		VIRTUAL void stop(int exit_code = 0);

		VIRTUAL Event::SharedEventManager eventManager(void) const;
		VIRTUAL SharedSceneManager sceneManager(void) const;
		VIRTUAL SharedFactory factory(void) const;

		VIRTUAL bool initialize(void);
		VIRTUAL void finalize(void);

		VIRTUAL void second(void);
		VIRTUAL void tick(TIME timeout);

		VIRTUAL void render(void);
		VIRTUAL void update(TIME delta);

		VIRTUAL bool serialize(TinyXML::TiXmlElement &node) const;
		VIRTUAL bool deserialize(TinyXML::TiXmlElement &node);

		VIRTUAL bool handleEvent(const Event::IEvent &event);

	public: /* static */

		static EngineBase * Instance(void);
	};

}

MARSHMALLOW_NAMESPACE_END

#endif
