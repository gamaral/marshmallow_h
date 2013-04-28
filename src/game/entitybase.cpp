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

#include "game/entitybase.h"

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

struct EntityBase::Private
{
	Private(const Core::Identifier &i, EntitySceneLayer &l)
	    : id(i)
	    , layer(l)
	    , killed(false)
	{}

	~Private();

	inline void
	pushComponent(Game::IComponent *component);

	inline Game::IComponent *
	popComponent(void);

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

	ComponentList components;
	Core::Identifier id;
	EntitySceneLayer &layer;
	bool killed;
};

EntityBase::Private::~Private()
{
	/* free components */
	while (components.size() > 0) {
		delete components.back();
		components.pop_back();
	}
}

void
EntityBase::Private::pushComponent(IComponent *c)
{
	components.push_front(c);
}

IComponent *
EntityBase::Private::popComponent(void)
{
	IComponent *l_component = components.front();
	components.pop_front();
	return(l_component);
}

void
EntityBase::Private::removeComponent(IComponent *c)
{
	components.remove(c);
}

IComponent *
EntityBase::Private::removeComponent(const Core::Identifier &i)
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
EntityBase::Private::getComponent(const Core::Identifier &i) const
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
EntityBase::Private::getComponentType(const Core::Type &t) const
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
EntityBase::Private::render(void)
{
	if (killed) return;

	ComponentList::const_reverse_iterator l_i;
	ComponentList::const_reverse_iterator l_c = components.rend();

	for (l_i = components.rbegin(); l_i != l_c; ++l_i)
		(*l_i)->render();
}

void
EntityBase::Private::update(float d)
{
	if (killed) return;

	ComponentList::const_reverse_iterator l_i;
	ComponentList::const_reverse_iterator l_c = components.rend();

	for (l_i = components.rbegin(); l_i != l_c; ++l_i)
		(*l_i)->update(d);
}

EntityBase::EntityBase(const Core::Identifier &i, EntitySceneLayer &l)
    : PIMPL_CREATE_X(i, l)
{
}

EntityBase::~EntityBase(void)
{
	PIMPL->components.clear();
	PIMPL_DESTROY;
}

const Core::Identifier &
EntityBase::id(void) const
{
	return(PIMPL->id);
}

EntitySceneLayer &
EntityBase::layer(void)
{
	return(PIMPL->layer);
}

void
EntityBase::pushComponent(IComponent *c)
{
	PIMPL->pushComponent(c);
}

IComponent *
EntityBase::popComponent(void)
{
	return(PIMPL->popComponent());
}

void
EntityBase::removeComponent(IComponent *c)
{
	PIMPL->removeComponent(c);
}

IComponent *
EntityBase::removeComponent(const Core::Identifier &i)
{
	return(PIMPL->removeComponent(i));
}

IComponent *
EntityBase::getComponent(const Core::Identifier &i) const
{
	return(PIMPL->getComponent(i));
}

IComponent *
EntityBase::getComponentType(const Core::Type &t) const
{
	return(PIMPL->getComponentType(t));
}

void
EntityBase::render(void)
{
	PIMPL->render();
}

void
EntityBase::update(float d)
{
	PIMPL->update(d);
}

void
EntityBase::kill(void)
{
	PIMPL->killed = true;
}

bool
EntityBase::isZombie(void) const
{
	return(PIMPL->killed);
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

