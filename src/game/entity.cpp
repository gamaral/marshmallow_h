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

#include "game/entity.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/logger.h"
#include "core/type.h"

#include "game/factory.h"
#include "game/icomponent.h"

#include <list>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */
namespace { /************************************ Game::<anonymous> Namespace */
	typedef std::list<IComponent *> ComponentList;
} /********************************************** Game::<anonymous> Namespace */

struct Entity::Private
{
	Private(const Core::Identifier &i, Game::EntitySceneLayer *l)
	    : id(i)
	    , layer(l)
	    , killed(false)
	{}

	~Private();

	inline void
	addComponent(Game::IComponent *component);

	inline void
	removeComponent(Game::IComponent *component);

	inline Game::IComponent *
	removeComponent(const Core::Identifier &identifier);

	inline Game::IComponent *
	getComponent(const Core::Identifier &identifier) const;
	
	inline Game::IComponent *
	getComponentType(const Core::Type &type) const;

	inline void
	render(void);

	inline void
	update(float delta);

	Game::ComponentList components;
	Core::Identifier id;
	Game::EntitySceneLayer *layer;
	bool killed;
};

Entity::Private::~Private()
{
	/* free components */
	while (!components.empty()) {
		delete components.back();
		components.pop_back();
	}
}

void
Entity::Private::addComponent(IComponent *c)
{
	components.push_back(c);
}

void
Entity::Private::removeComponent(IComponent *c)
{
	components.remove(c);
}

IComponent *
Entity::Private::removeComponent(const Core::Identifier &i)
{
	IComponent *l_component = 0;

	ComponentList::const_iterator l_i;
	ComponentList::const_iterator l_c = components.end();

	for (l_i = components.begin(); l_i != l_c; ++l_i)
		if ((*l_i)->id() == i) {
			l_component = *l_i;
			components.remove(l_component);
			break;
		}

	return(l_component);
}

IComponent *
Entity::Private::getComponent(const Core::Identifier &i) const
{
	ComponentList::const_iterator l_i;
	ComponentList::const_iterator l_c = components.end();

	for (l_i = components.begin(); l_i != l_c; ++l_i) {
		if ((*l_i)->id() == i)
			return(*l_i);
	}
	return(0);
}

IComponent *
Entity::Private::getComponentType(const Core::Type &t) const
{
	ComponentList::const_iterator l_i;
	ComponentList::const_iterator l_c = components.end();

	for (l_i = components.begin(); l_i != l_c; ++l_i) {
		if ((*l_i)->type() == t)
			return(*l_i);
	}
	return(0);
}

void
Entity::Private::render(void)
{
	if (killed) return;

	ComponentList::const_reverse_iterator l_i;
	ComponentList::const_reverse_iterator l_c = components.rend();

	for (l_i = components.rbegin(); l_i != l_c; ++l_i)
		(*l_i)->render();
}

void
Entity::Private::update(float d)
{
	if (killed) return;

	ComponentList::const_reverse_iterator l_i;
	ComponentList::const_reverse_iterator l_c = components.rend();

	for (l_i = components.rbegin(); l_i != l_c; ++l_i)
		(*l_i)->update(d);
}

Entity::Entity(const Core::Identifier &i, Game::EntitySceneLayer *l)
    : PIMPL_CREATE_X(i, l)
{
}

Entity::~Entity(void)
{
	PIMPL_DESTROY;
}

const Core::Identifier &
Entity::id(void) const
{
	return(PIMPL->id);
}

Game::EntitySceneLayer *
Entity::layer(void) const
{
	return(PIMPL->layer);
}

void
Entity::addComponent(IComponent *c)
{
	PIMPL->addComponent(c);
}

void
Entity::removeComponent(IComponent *c)
{
	PIMPL->removeComponent(c);
}

IComponent *
Entity::removeComponent(const Core::Identifier &i)
{
	return(PIMPL->removeComponent(i));
}

IComponent *
Entity::getComponent(const Core::Identifier &i) const
{
	return(PIMPL->getComponent(i));
}

IComponent *
Entity::getComponentType(const Core::Type &t) const
{
	return(PIMPL->getComponentType(t));
}

void
Entity::render(void)
{
	PIMPL->render();
}

void
Entity::update(float d)
{
	PIMPL->update(d);
}

void
Entity::kill(void)
{
	PIMPL->killed = true;
}

bool
Entity::isZombie(void) const
{
	return(PIMPL->killed);
}

const Core::Type &
Entity::Type(void)
{
	static const Core::Type s_type("Game::Entity");
	return(s_type);
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

