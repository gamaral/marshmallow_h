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

#include "box2dcolliderentity.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <Box2D/Box2D.h>

#include <core/logger.h>

#include <game/box2d/box2dcomponent.h>
#include <game/sizecomponent.h>

const Core::Type Box2DColliderEntity::sType("Box2DColliderEntity");

Box2DColliderEntity::Box2DColliderEntity(const Core::Identifier &i, Game::EntitySceneLayer &l)
    : Game::EntityBase(i, l)
    , m_init(false)
{
}

Box2DColliderEntity::~Box2DColliderEntity(void)
{
}

void
Box2DColliderEntity::update(float d)
{
	if (!m_init) {
		Game::SharedSizeComponent l_size_component =
		    getComponentType(Game::SizeComponent::Type()).staticCast<Game::SizeComponent>();
		if (!l_size_component) {
			MMERROR1("Collider entity requires a size component to be present");
			return;
		}

		Game::SharedBox2DComponent l_box2d_component =
		    new Game::Box2DComponent("box2d", *this);
		l_box2d_component->bodyType() = b2_staticBody;
		l_box2d_component->size() = l_size_component->size();
		pushComponent(l_box2d_component.staticCast<Game::IComponent>());
		m_init = true;
	}

	Game::EntityBase::update(d);
}

