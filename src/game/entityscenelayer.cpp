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

#include "game/entityscenelayer.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <cmath>

#include "core/logger.h"

#include "math/vector2.h"

#include "graphics/transform.h"
#include "graphics/viewport.h"

#include "game/factorybase.h"
#include "game/ientity.h"
#include "game/positioncomponent.h"
#include "game/sizecomponent.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Game;

const Core::Type EntitySceneLayer::sType("Game::EntitySceneLayer");

EntitySceneLayer::EntitySceneLayer(const Core::Identifier &i, IScene &s, int f)
    : SceneLayerBase(i, s, f)
    , m_entities()
    , m_visiblility_testing(true)
{
}

EntitySceneLayer::~EntitySceneLayer(void)
{
	m_entities.clear();
}

void
EntitySceneLayer::addEntity(const SharedEntity &e)
{
	m_entities.push_back(e);
}

void
EntitySceneLayer::removeEntity(const Core::Identifier &i)
{
	EntityList::const_iterator l_i;
	EntityList::const_iterator l_c = m_entities.end();

	/* maybe replace later with a map if required */
	for (l_i = m_entities.begin(); l_i != l_c; ++l_i) {
		if ((*l_i)->id() == i) {
			m_entities.remove(*l_i);
			return;
		}
	}
}

void
EntitySceneLayer::removeEntity(const SharedEntity &e)
{
	m_entities.remove(e);
}

SharedEntity
EntitySceneLayer::getEntity(const Core::Identifier &i) const
{
	EntityList::const_iterator l_i;
	EntityList::const_iterator l_c = m_entities.end();

	/* maybe replace later with a map if required */
	for (l_i = m_entities.begin(); l_i != l_c; ++l_i) {
		if ((*l_i)->id() == i)
			return(*l_i);
	}

	return(SharedEntity());
}

void
EntitySceneLayer::setVisibilityTesting(bool value)
{
	m_visiblility_testing = value;
}

void
EntitySceneLayer::render(void)
{
	EntityList::const_iterator l_i;

	if (m_visiblility_testing) {
		const Math::Point2 &l_camera_pos = Graphics::Viewport::Camera().translation();
		const float l_visiblility_radius2 = Graphics::Viewport::Radius2();

		for (l_i = m_entities.begin(); l_i != m_entities.end();l_i++) {
			SharedEntity l_entity = (*l_i);
			float l_size2 = 0;

			if (l_entity->isZombie())
				continue;

			SharedPositionComponent l_positionComponent =
			    l_entity->getComponentType("Game::PositionComponent")
			        .staticCast<Game::PositionComponent>();
			if (!l_positionComponent) {
				l_entity->render();
				continue;
			}

			SharedSizeComponent l_sizeComponent =
			    l_entity->getComponentType("Game::SizeComponent")
			        .staticCast<Game::SizeComponent>();
			if (l_sizeComponent) {
				const Math::Size2f &l_size =
				    l_sizeComponent->size();
				l_size2 = powf(l_size.width(),  2) +
				          powf(l_size.height(), 2);
			}

			const Math::Point2 &l_position = l_positionComponent->position();
			const float l_distance2 = l_camera_pos.difference(l_position).magnitude2();

			if (l_distance2 < l_visiblility_radius2 + l_size2)
				l_entity->render();
		}
	}
	else for (l_i = m_entities.begin(); l_i != m_entities.end();l_i++)
		if (!(*l_i)->isZombie()) (*l_i)->render();
}

void
EntitySceneLayer::update(float d)
{
	EntityList::const_iterator l_i;

	for (l_i = m_entities.begin(); l_i != m_entities.end();) {
		SharedEntity l_entity = (*l_i++);

		if (l_entity->isZombie())
			removeEntity(l_entity);
		else
			l_entity->update(d);
	}
}

bool
EntitySceneLayer::serialize(TinyXML::TiXmlElement &n) const
{
	if (!SceneLayerBase::serialize(n))
		return(false);

	EntityList::const_iterator l_i;
	for (l_i = m_entities.begin(); l_i != m_entities.end();) {
		SharedEntity l_entity = (*l_i++);
		TinyXML::TiXmlElement l_element("entity");
		if (l_entity->serialize(l_element))
			n.InsertEndChild(l_element);
	}
	
	return(true);
}

bool
EntitySceneLayer::deserialize(TinyXML::TiXmlElement &n)
{
	if (!SceneLayerBase::deserialize(n))
		return(false);

	TinyXML::TiXmlElement *l_child;
	for (l_child = n.FirstChildElement("entity") ;
	     l_child;
	     l_child = l_child->NextSiblingElement("entity")) {

		const char *l_id   = l_child->Attribute("id");
		const char *l_type = l_child->Attribute("type");

		SharedEntity l_entity =
		    FactoryBase::Instance()->createEntity(l_type, l_id, *this);

		if (!l_entity) {
			MMWARNING("Entity '%s' of type '%s' creation failed", l_id, l_type);
			continue;
		}

		if (!l_entity->deserialize(*l_child)) {
			MMWARNING("Entity '%s' of type '%s' failed deserialization", l_id, l_type);
			continue;
		}

		addEntity(l_entity);
	}
	
	return(true);
}

const Core::Type &
EntitySceneLayer::Type(void)
{
	return(sType);
}

