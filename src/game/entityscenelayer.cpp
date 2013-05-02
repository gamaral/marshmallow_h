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

#include "game/entityscenelayer.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/logger.h"
#include "core/type.h"

#include "graphics/camera.h"

#include "game/factory.h"
#include "game/ientity.h"
#include "game/positioncomponent.h"
#include "game/sizecomponent.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

struct EntitySceneLayer::Private
{
	Private()
	    : visiblility_testing(true)
	{}

	~Private();

	inline void
	removeEntity(const Core::Identifier &identifier);

	inline Game::IEntity *
	getEntity(const Core::Identifier &identifier) const;

	inline void
	render(void);

	inline void
	update(float delta);

	EntityList entities;
	bool visiblility_testing;
};

EntitySceneLayer::Private::~Private()
{
	/* free entities */
	while (entities.size() > 0) {
		delete entities.back();
		entities.pop_back();
	}
}

void
EntitySceneLayer::Private::removeEntity(const Core::Identifier &i)
{
	EntityList::const_iterator l_i;
	EntityList::const_iterator l_c = entities.end();

	/* maybe replace later with a map if required */
	for (l_i = entities.begin(); l_i != l_c; ++l_i) {
		if ((*l_i)->id() == i) {
			entities.remove(*l_i);
			return;
		}
	}
}

Game::IEntity *
EntitySceneLayer::Private::getEntity(const Core::Identifier &i) const
{
	EntityList::const_iterator l_i;
	EntityList::const_iterator l_c = entities.end();

	/* maybe replace later with a map if required */
	for (l_i = entities.begin(); l_i != l_c; ++l_i) {
		if ((*l_i)->id() == i)
			return(*l_i);
	}

	return(0);
}

void
EntitySceneLayer::Private::render(void)
{
	EntityList::const_iterator l_i;

	if (visiblility_testing) {
		const Math::Point2 &l_camera_pos = Graphics::Camera::Position();
		const float l_visiblility_radius2 = Graphics::Camera::VisibleMagnitude2();

		for (l_i = entities.begin(); l_i != entities.end();l_i++) {
			IEntity *l_entity = (*l_i);
			float l_size2 = 0;

			if (l_entity->isZombie())
				continue;

			PositionComponent *l_positionComponent = static_cast<PositionComponent *>
			    (l_entity->getComponentType("Game::PositionComponent"));
			if (!l_positionComponent) {
				l_entity->render();
				continue;
			}

			SizeComponent *l_sizeComponent = static_cast<SizeComponent *>
			    (l_entity->getComponentType("Game::SizeComponent"));
			if (l_sizeComponent) {
				const Math::Size2f &l_size =
				    l_sizeComponent->size();
				l_size2 = powf(l_size.width,  2) +
				          powf(l_size.height, 2);
			}

			const Math::Point2 &l_position = l_positionComponent->position();
			const float l_distance2 = l_camera_pos.difference(l_position).magnitude2();

			if (l_distance2 < l_visiblility_radius2 + l_size2)
				l_entity->render();
		}
	}
	else for (l_i = entities.begin(); l_i != entities.end();l_i++)
		if (!(*l_i)->isZombie()) (*l_i)->render();
}

void
EntitySceneLayer::Private::update(float d)
{
	EntityList::const_iterator l_i;

	for (l_i = entities.begin(); l_i != entities.end();) {
		IEntity *l_entity = (*l_i++);

		if (l_entity->isZombie()) {
			entities.remove(l_entity);
			delete l_entity;
		}
		else
			l_entity->update(d);
	}
}
EntitySceneLayer::EntitySceneLayer(const Core::Identifier &i, IScene &s, int f)
    : SceneLayer(i, s, f)
    , PIMPL_CREATE
{
}

EntitySceneLayer::~EntitySceneLayer(void)
{
	PIMPL->entities.clear();

	PIMPL_DESTROY;
}

void
EntitySceneLayer::addEntity(Game::IEntity *e)
{
	PIMPL->entities.push_back(e);
}

void
EntitySceneLayer::removeEntity(const Core::Identifier &i)
{
	PIMPL->removeEntity(i);
}

void
EntitySceneLayer::removeEntity(Game::IEntity *e)
{
	PIMPL->entities.remove(e);
}

Game::IEntity *
EntitySceneLayer::getEntity(const Core::Identifier &i) const
{
	return(PIMPL->getEntity(i));
}

const Game::EntityList &
EntitySceneLayer::getEntities(void) const
{
	return(PIMPL->entities);
}

bool
EntitySceneLayer::visiblityTesting(void) const
{
	return(PIMPL->visiblility_testing);
}

void
EntitySceneLayer::setVisibilityTesting(bool value)
{
	PIMPL->visiblility_testing = value;
}

void
EntitySceneLayer::render(void)
{
	PIMPL->render();
}

void
EntitySceneLayer::update(float d)
{
	PIMPL->update(d);
}

const Core::Type &
EntitySceneLayer::Type(void)
{
	static const Core::Type s_type("Game::EntitySceneLayer");
	return(s_type);
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

