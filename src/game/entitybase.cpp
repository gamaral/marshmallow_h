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

#include "game/icomponent.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Game;

const Core::Type EntityBase::Type("Game::EntityBase");

EntityBase::EntityBase(const Core::Identifier i)
    : m_id(i)
{
}

EntityBase::~EntityBase(void)
{
}

void
EntityBase::addComponent(SharedComponent &c)
{
	m_components.push_back(c);
}

void
EntityBase::removeComponent(const SharedComponent &c)
{
	m_components.remove(c);
}

SharedComponent
EntityBase::component(const Core::Identifier &i) const
{
	ComponentList::const_iterator l_i;
	ComponentList::const_iterator l_c = m_components.end();

	/* maybe replace later with a map if required */
	for (l_i = m_components.begin(); l_i != l_c; ++l_i) {
		if ((*l_i)->id() == i)
			return(*l_i);
	}
	return(SharedComponent());
}

void
EntityBase::update(void)
{
	ComponentList::const_iterator l_i;
	ComponentList::const_iterator l_c = m_components.end();

	for (l_i = m_components.begin(); l_i != l_c; ++l_i)
		(*l_i)->update();
}

