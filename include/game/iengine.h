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

#ifndef MARSHMALLOW_GAME_IENGINE_H
#define MARSHMALLOW_GAME_IENGINE_H 1

#include <core/fd.h>
#include <core/irenderable.h>
#include <core/iserializable.h>
#include <core/iupdateable.h>

#include <event/ieventlistener.h>

MARSHMALLOW_NAMESPACE_BEGIN

namespace Event
{
	class EventManager;
	typedef Core::Shared<EventManager> SharedEventManager;
}

namespace Game
{
	class SceneManager;
	typedef Core::Shared<SceneManager> SharedSceneManager;

	struct IFactory;
	typedef Core::Shared<IFactory> SharedFactory;

	/*! @brief Game Engine Interface */
	struct IEngine : public Core::IRenderable
	               , public Core::IUpdateable
	               , public Core::ISerializable
	               , public Event::IEventListener
	{
		virtual ~IEngine(void) {};

		/*!
		 * @brief Start Engine
		 */
		virtual int run(void) = 0;

		/*!
		 * @brief Stop Engine
		 * @param exit_code Exit code
		 */
		virtual void stop(int exit_code = 0) = 0;

		/*!
		 * @brief Event Manager
		 */
		virtual Event::SharedEventManager eventManager(void) const = 0;

		/*!
		 * @brief Scene Manager
		 */
		virtual SharedSceneManager sceneManager(void) const = 0;

		/*!
		 * @brief Factory
		 */
		virtual SharedFactory factory(void) const = 0;

		virtual bool initialize(void) = 0;
		virtual void finalize(void) = 0;

		/*!
		 * @brief Returns true if engine is valid
		 */
		virtual bool isValid(void) const = 0;

		virtual void second(void) = 0;
		virtual void tick(void) = 0;
	};
}

MARSHMALLOW_NAMESPACE_END

#endif
