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

#include "snatcherlayer.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"

#include <event/eventmanager.h>
#include <event/ievent.h>
#include <event/keyboardevent.h>

#include <game/engine.h>
#include <game/entityscenelayer.h>
#include <game/iengine.h>
#include <game/iscene.h>

#include "inputcomponent.h"

SnatcherLayer::SnatcherLayer(const Core::Identifier &i, Game::IScene &s)
    : Game::SceneLayerBase(i, s)
    , m_state(ssIdle)
    , m_timeout(0)
    , m_active(0)
{
	Game::Engine::Instance()->eventManager()->connect(this, Event::KeyboardEvent::Type());
}

SnatcherLayer::~SnatcherLayer(void)
{
	Game::Engine::Instance()->eventManager()->disconnect(this, Event::KeyboardEvent::Type());
}

void
SnatcherLayer::registerEntity(Game::IEntity &e)
{
	m_entities.push_back(e.id());
}

void
SnatcherLayer::deregisterEntity(Game::IEntity &e)
{
	register MMUID l_id = e.id();
	UIDList::reverse_iterator l_i;

	for (l_i = m_entities.rbegin(); l_i != m_entities.rend();) {
		if (*l_i == l_id) {
			UIDList::iterator l_tmp = m_entities.erase(--l_i.base());
			l_i = UIDList::reverse_iterator(l_tmp);
		}
		else ++l_i;
	}
}

void
SnatcherLayer::update(float t)
{
	/* XXX: LATE NIGHT HACK */
	switch (m_state) {
	case ssNext: {
		const int l_ecount = m_entities.size();
		if (l_ecount == 0) break;

		/*
		 * locate entity layer
		 * TODO: make member variables
		 */
		Game::SharedSceneLayer l_layer = scene().getLayerType(Game::EntitySceneLayer::Type());
		Game::SharedEntitySceneLayer l_entityLayer = l_layer.staticCast<Game::EntitySceneLayer>();
		Game::WeakEntity l_entity;

		/* we must have an entity layer */
		assert(l_entityLayer && "Entity scene layer was not found!");

		/* remove previous input component */
		if (m_active < l_ecount) {
			l_entity = l_entityLayer->getEntity(m_entities[m_active]);
			if (l_entity) l_entity->removeComponent("input");
		}

		/* update active index */
		m_active = (m_active + 1) % l_ecount;

		/* add input component */
		l_entity = l_entityLayer->getEntity(m_entities[m_active]);
		if (l_entity) l_entity->pushComponent(new InputComponent("input", *l_entity));

		setState(ssTimeout);
		} /* NOTICE: FALLTHROUGH */
	case ssTimeout:
		m_timeout += t;
		if (m_timeout >= 0.5f)
			setState(ssIdle);

		/* NOTICE: FALLTHROUGH */
	case ssIdle:
		break;
	}
}

bool
SnatcherLayer::handleEvent(const Event::IEvent &e)
{
	using namespace Input;

	if (e.type() != Event::KeyboardEvent::Type())
		return(false);

	const Event::KeyboardEvent &l_kevent =
	    static_cast<const Event::KeyboardEvent &>(e);

	if (l_kevent.key() == Keyboard::KBK_TAB && m_state == ssIdle)
		m_state = ssNext;

	return(false);
}

void
SnatcherLayer::setState(SnatchState s)
{
	if (s == m_state)
		return;

	switch (s) {
	case ssTimeout:
		m_timeout = 0;

		/* NOTICE: FALLTHROUGH */
	case ssNext:
	case ssIdle:
	break;
	}
	m_state = s;
}

const Core::Type &
SnatcherLayer::Type(void)
{
	static const Core::Type s_type("SnatcherLayer");
	return(s_type);
}

