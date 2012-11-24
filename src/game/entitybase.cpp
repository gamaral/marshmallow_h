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

#include "game/entitybase.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/logger.h"
#include "core/shared.h"

#include "game/factorybase.h"
#include "game/icomponent.h"

#include <tinyxml2.h>

#include <list>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

typedef std::list<SharedComponent> ComponentList;

struct EntityBase::Private
{
	Private(const Core::Identifier &i, EntitySceneLayer &l)
	    : id(i)
	    , layer(l)
	    , killed(false) {}

	ComponentList components;
	Core::Identifier id;
	EntitySceneLayer &layer;
	bool killed;
};

EntityBase::EntityBase(const Core::Identifier &i, EntitySceneLayer &l)
    : m_p(new Private(i, l))
{
}

EntityBase::~EntityBase(void)
{
	m_p->components.clear();

	delete m_p, m_p = 0;
}

const Core::Identifier &
EntityBase::id(void) const
{
	return(m_p->id);
}

EntitySceneLayer &
EntityBase::layer(void)
{
	return(m_p->layer);
}

void
EntityBase::pushComponent(const SharedComponent &c)
{
	m_p->components.push_front(c);
}

void
EntityBase::popComponent(void)
{
	m_p->components.pop_front();
}

void
EntityBase::removeComponent(const Core::Identifier &i)
{
	ComponentList::const_iterator l_i;
	ComponentList::const_iterator l_c = m_p->components.end();

	for (l_i = m_p->components.begin(); l_i != l_c; ++l_i)
		if ((*l_i)->id() == i) {
			m_p->components.remove(*l_i);
			return;
		}
}

void
EntityBase::removeComponent(const SharedComponent &c)
{
	m_p->components.remove(c);
}

SharedComponent
EntityBase::getComponent(const Core::Identifier &i) const
{
	ComponentList::const_iterator l_i;
	ComponentList::const_iterator l_c = m_p->components.end();

	for (l_i = m_p->components.begin(); l_i != l_c; ++l_i) {
		if ((*l_i)->id() == i)
			return(*l_i);
	}
	return(SharedComponent());
}

SharedComponent
EntityBase::getComponentType(const Core::Type &t) const
{
	ComponentList::const_iterator l_i;
	ComponentList::const_iterator l_c = m_p->components.end();

	for (l_i = m_p->components.begin(); l_i != l_c; ++l_i) {
		if ((*l_i)->type() == t)
			return(*l_i);
	}
	return(SharedComponent());
}

void
EntityBase::render(void)
{
	if (isZombie()) return;

	ComponentList::const_reverse_iterator l_i;
	ComponentList::const_reverse_iterator l_c = m_p->components.rend();

	for (l_i = m_p->components.rbegin(); l_i != l_c; ++l_i)
		(*l_i)->render();
}

void
EntityBase::update(float d)
{
	if (isZombie()) return;

	ComponentList::const_reverse_iterator l_i;
	ComponentList::const_reverse_iterator l_c = m_p->components.rend();

	for (l_i = m_p->components.rbegin(); l_i != l_c; ++l_i)
		(*l_i)->update(d);
}

void
EntityBase::kill(void)
{
	m_p->killed = true;
}

bool
EntityBase::isZombie(void) const
{
	return(m_p->killed);
}

bool
EntityBase::serialize(XMLElement &n) const
{
	n.SetAttribute("id", id().str().c_str());
	n.SetAttribute("type", type().str().c_str());

	ComponentList::const_reverse_iterator l_i;
	ComponentList::const_reverse_iterator l_c = m_p->components.rend();

	for (l_i = m_p->components.rbegin(); l_i != l_c; l_i++) {
		XMLElement *l_element = n.GetDocument()->NewElement("component");
		if ((*l_i)->serialize(*l_element))
			n.InsertEndChild(l_element);
	}
	
	return(true);
}

bool
EntityBase::deserialize(XMLElement &n)
{
	XMLElement *l_child;
	for (l_child = n.FirstChildElement("component") ;
	     l_child;
	     l_child = l_child->NextSiblingElement("component")) {

		const char *l_id   = l_child->Attribute("id");
		const char *l_type = l_child->Attribute("type");

		SharedComponent l_component =
		    FactoryBase::Instance()->createComponent(l_type, l_id, *this);

		if (!l_component) {
			MMWARNING("Failed to create component '" << l_id << "' (" << l_type << ")");
			continue;
		}

		if (!l_component->deserialize(*l_child)) {
			MMWARNING("Failed to deserialize component '" << l_id << "' (" << l_type << ")");
			continue;
		}

		pushComponent(l_component);
	}

	return(true);
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

