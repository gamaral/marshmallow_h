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

#include "game/entitybase.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"
#include "game/factorybase.h"
#include "game/icomponent.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Game;

EntityBase::EntityBase(const Core::Identifier &i, EntitySceneLayer &l)
    : m_components(),
      m_id(i),
      m_layer(l),
      m_killed(false)
{
}

EntityBase::~EntityBase(void)
{
	m_components.clear();
}

void
EntityBase::pushComponent(const SharedComponent &c)
{
	m_components.push_front(c);
}

void
EntityBase::popComponent(void)
{
	m_components.pop_front();
}

void
EntityBase::removeComponent(const Core::Identifier &i)
{
	ComponentList::const_iterator l_i;
	ComponentList::const_iterator l_c = m_components.end();

	for (l_i = m_components.begin(); l_i != l_c; ++l_i)
		if ((*l_i)->id() == i) {
			m_components.remove(*l_i);
			return;
		}
}

void
EntityBase::removeComponent(const SharedComponent &c)
{
	m_components.remove(c);
}

SharedComponent
EntityBase::getComponent(const Core::Identifier &i) const
{
	ComponentList::const_iterator l_i;
	ComponentList::const_iterator l_c = m_components.end();

	for (l_i = m_components.begin(); l_i != l_c; ++l_i) {
		if ((*l_i)->id() == i)
			return(*l_i);
	}
	return(SharedComponent());
}

SharedComponent
EntityBase::getComponentType(const Core::Type &t) const
{
	ComponentList::const_iterator l_i;
	ComponentList::const_iterator l_c = m_components.end();

	for (l_i = m_components.begin(); l_i != l_c; ++l_i) {
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
	ComponentList::const_reverse_iterator l_c = m_components.rend();

	for (l_i = m_components.rbegin(); l_i != l_c; ++l_i)
		(*l_i)->render();
}

void
EntityBase::update(TIME d)
{
	if (isZombie()) return;

	ComponentList::const_reverse_iterator l_i;
	ComponentList::const_reverse_iterator l_c = m_components.rend();

	for (l_i = m_components.rbegin(); l_i != l_c; ++l_i)
		(*l_i)->update(d);
}

bool
EntityBase::serialize(TinyXML::TiXmlElement &n) const
{
	n.SetAttribute("id", id().str().c_str());
	n.SetAttribute("type", type().str().c_str());

	ComponentList::const_reverse_iterator l_i;
	ComponentList::const_reverse_iterator l_c = m_components.rend();

	for (l_i = m_components.rbegin(); l_i != l_c; l_i++) {
		TinyXML::TiXmlElement l_element("component");
		if ((*l_i)->serialize(l_element))
			n.InsertEndChild(l_element);
	}
	
	return(true);
}

bool
EntityBase::deserialize(TinyXML::TiXmlElement &n)
{
	TinyXML::TiXmlElement *l_child;
	for (l_child = n.FirstChildElement("component") ;
	     l_child;
	     l_child = l_child->NextSiblingElement("component")) {

		const char *l_id   = l_child->Attribute("id");
		const char *l_type = l_child->Attribute("type");

		SharedComponent l_component =
		    FactoryBase::Instance()->createComponent(l_type, l_id, *this);

		if (!l_component) {
			WARNING("Component '%s' of type '%s' creation failed", l_id, l_type);
			continue;
		}

		if (!l_component->deserialize(*l_child)) {
			WARNING("Component '%s' of type '%s' failed deserialization", l_id, l_type);
			continue;
		}

		pushComponent(l_component);
	}

	return(true);
}

