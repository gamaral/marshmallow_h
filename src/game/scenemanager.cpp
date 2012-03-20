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

#include "game/scenemanager.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"
#include "core/weak.h"

#include "event/eventmanager.h"
#include "event/renderevent.h"
#include "event/updateevent.h"

#include "game/factorybase.h"
#include "game/iscene.h"

#include <tinyxml2.h>

#include <list>

MARSHMALLOW_NAMESPACE_USE
using namespace Core;
using namespace Game;

/******************************************************************************/

namespace {
	typedef std::list<SharedScene> SceneStack;
} // namespace

/******************************************************************************/

struct SceneManager::Private
{
	SceneStack  stack;
	SharedScene active;
};

SceneManager::SceneManager(void)
    : m_p(new Private)
{
	Event::EventManager::Instance()->connect(this, Event::RenderEvent::Type());
	Event::EventManager::Instance()->connect(this, Event::UpdateEvent::Type());
}

SceneManager::~SceneManager(void)
{
	Event::EventManager::Instance()->disconnect(this, Event::UpdateEvent::Type());
	Event::EventManager::Instance()->disconnect(this, Event::RenderEvent::Type());

	m_p->stack.clear();

	delete m_p;
	m_p = 0;
}

void
SceneManager::pushScene(const SharedScene &scene)
{
	if (!scene) return;

	if (m_p->active) {
		m_p->active->deactivate();
		m_p->stack.push_front(m_p->active);
	}

	m_p->active = scene;
	m_p->active->activate();
}

void
SceneManager::popScene(void)
{
	if (m_p->active)
		m_p->active->deactivate();

	if (!m_p->stack.empty()) {
		m_p->active = m_p->stack.front();
		m_p->stack.pop_front();
	} else MMWARNING("Scene stack is empty!");
}

SharedScene
SceneManager::activeScene(void) const
{
	return(m_p->active);
}

void
SceneManager::render(void)
{
	SceneStack::const_iterator l_i;
	SceneStack::const_iterator l_c = m_p->stack.end();

	for (l_i = m_p->stack.begin(); l_i != l_c; ++l_i)
		(*l_i)->render();

	if (m_p->active)
		m_p->active->render();
}

void
SceneManager::update(float d)
{
	if (m_p->active) m_p->active->update(d);
}

bool
SceneManager::serialize(XMLElement &n) const
{
	SceneStack::const_reverse_iterator l_i;
	SceneStack::const_reverse_iterator l_c = m_p->stack.rend();
	for (l_i = m_p->stack.rbegin(); l_i != l_c; ++l_i) {
		XMLElement *l_element = n.GetDocument()->NewElement("scene");
		if ((*l_i)->serialize(*l_element))
			n.InsertEndChild(l_element);
	}

	if (m_p->active) {
		XMLElement *l_element = n.GetDocument()->NewElement("scene");
		if (m_p->active->serialize(*l_element))
			n.InsertEndChild(l_element);
	}
	
	return(true);
}

bool
SceneManager::deserialize(XMLElement &n)
{
	XMLElement *l_child;
	for (l_child = n.FirstChildElement("scene") ;
	     l_child;
	     l_child = l_child->NextSiblingElement("scene")) {

		const char *l_id   = l_child->Attribute("id");
		const char *l_type = l_child->Attribute("type");

		SharedScene l_scene =
		    FactoryBase::Instance()->createScene(l_type, l_id);

		if (!l_scene) {
			MMWARNING("Scene '" << l_id << "' of type '" << l_type << "' creation failed");
			continue;
		}

		if (!l_scene->deserialize(*l_child)) {
			MMWARNING("Scene '" << l_id << "' of type '" << l_type << "' failed deserialization");
			continue;
		}

		pushScene(l_scene);
	}
	
	return(true);
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

