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

#include "game/scenemanager.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/logger.h"
#include "core/type.h"

#include "event/eventmanager.h"
#include "event/renderevent.h"
#include "event/updateevent.h"

#include "graphics/backend_p.h"

#include "game/engine.h"
#include "game/factory.h"
#include "game/iscene.h"

#include <list>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */
namespace { /************************************ Game::<anonymous> Namespace */

	typedef std::list<IScene *> SceneStack;

} /********************************************** Game::<anonymous> Namespace */

struct SceneManager::Private
{
	Private()
	    : active(0)
	{}

	SceneStack stack;
	IScene    *active;
};

SceneManager::SceneManager(void)
    : PIMPL_CREATE
{
	Event::EventManager::Instance()->connect(this, Event::RenderEvent::Type());
	Event::EventManager::Instance()->connect(this, Event::UpdateEvent::Type());
}

SceneManager::~SceneManager(void)
{
	Event::EventManager::Instance()->disconnect(this, Event::UpdateEvent::Type());
	Event::EventManager::Instance()->disconnect(this, Event::RenderEvent::Type());

	PIMPL->stack.clear();

	PIMPL_DESTROY;
}

void
SceneManager::pushScene(Game::IScene *scene)
{
	if (!scene) return;

	if (PIMPL->active) {
		PIMPL->active->deactivate();
		PIMPL->stack.push_front(PIMPL->active);
	}

	PIMPL->active = scene;
	PIMPL->active->activate();
}

Game::IScene *
SceneManager::popScene(void)
{
	IScene *l_scene = PIMPL->active;

	if (PIMPL->active)
		PIMPL->active->deactivate();

	if (!PIMPL->stack.empty()) {
		PIMPL->active = PIMPL->stack.front();
		PIMPL->stack.pop_front();
	} else {
		PIMPL->active = 0;
		MMWARNING("Scene stack is empty!");
	}

	return(l_scene);
}

Game::IScene *
SceneManager::activeScene(void) const
{
	return(PIMPL->active);
}

void
SceneManager::render(void)
{
	SceneStack::const_iterator l_i;
	SceneStack::const_iterator l_c = PIMPL->stack.end();

	for (l_i = PIMPL->stack.begin(); l_i != l_c; ++l_i)
		(*l_i)->render();

	if (PIMPL->active)
		PIMPL->active->render();
}

void
SceneManager::update(float d)
{
	if (PIMPL->active &&
	    !Game::Engine::Instance()->isSuspended())
		PIMPL->active->update(d);
}

bool
SceneManager::handleEvent(const Event::IEvent &e)
{
	if (e.type() == Event::RenderEvent::Type())
		render();
	else if (e.type() == Event::UpdateEvent::Type())
		update(static_cast<const Event::UpdateEvent &>(e).delta());

	return(false);
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

