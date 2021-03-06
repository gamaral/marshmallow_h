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

#include "game/positioncomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/type.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

struct PositionComponent::Private
{
	Math::Point2 position;
};

PositionComponent::PositionComponent(const Core::Identifier &i, Game::IEntity *e)
    : Component(i, e)
    , PIMPL_CREATE
{
}

PositionComponent::~PositionComponent(void)
{
	PIMPL_DESTROY;
}

const Math::Point2 &
PositionComponent::position(void) const
{
	return(PIMPL->position);
}

void
PositionComponent::setPosition(const Math::Point2 &p)
{
	PIMPL->position = p;
}

void
PositionComponent::setPosition(float x, float y)
{
	PIMPL->position.x = x;
	PIMPL->position.y = y;
}

float
PositionComponent::positionX(void) const
{
	return(PIMPL->position.x);
}

void
PositionComponent::setPositionX(float x)
{
	PIMPL->position.x = x;
}

float
PositionComponent::positionY(void) const
{
	return(PIMPL->position.y);
}

void
PositionComponent::setPositionY(float y)
{
	PIMPL->position.y = y;
}

void
PositionComponent::translate(const Math::Vector2 &r)
{
	PIMPL->position += r;
}

void
PositionComponent::translate(float x, float y)
{
	PIMPL->position.x += x;
	PIMPL->position.y += y;
}

void
PositionComponent::translateX(float x)
{
	PIMPL->position.x += x;
}

void
PositionComponent::translateY(float y)
{
	PIMPL->position.y += y;
}

const Core::Type &
PositionComponent::Type(void)
{
	static const Core::Type s_type("Game::PositionComponent");
	return(s_type);
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

