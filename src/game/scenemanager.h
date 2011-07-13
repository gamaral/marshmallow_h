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

#ifndef GAME_SCENEMANAGER_H
#define GAME_SCENEMANAGER_H 1

#include "EASTL/list.h"
using namespace eastl;

#include "core/irenderable.h"
#include "core/iserializable.h"
#include "core/iupdateable.h"
#include "event/ieventlistener.h"

#include "core/shared.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Event
{
	struct IEventListener;
	typedef Core::Shared<IEventListener> SharedEventListener;
}

namespace Game
{
	struct IScene;

	/*! @brief Game Scene Manager */
	class GAME_EXPORT SceneManager : public Core::IRenderable,
                                         public Core::IUpdateable,
	                                 public Core::ISerializable,
	                                 public Event::IEventListener
	{
		typedef Core::Shared<IScene> SharedScene;
		typedef list<SharedScene> SceneStack;

		SceneStack  m_stack;
		SharedScene m_active;
		Event::SharedEventListener m_event_proxy;

		NO_COPY(SceneManager);

	public:

		SceneManager(void);
		virtual ~SceneManager(void);

		void pushScene(SharedScene &scene);
		void popScene(void);

		SharedScene activeScene(void) const;

	public: /* virtual */

		VIRTUAL void render(void);
		VIRTUAL void update(TIME delta);

		VIRTUAL bool serialize(TinyXML::TiXmlElement &node) const;
		VIRTUAL bool deserialize(TinyXML::TiXmlElement &node);

		VIRTUAL bool handleEvent(const Event::IEvent &event);
	};
	typedef Core::Shared<SceneManager> SharedSceneManager;

}

MARSHMALLOW_NAMESPACE_END

#endif
