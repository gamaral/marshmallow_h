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

#include "game/movementcomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/logger.h"
#include "core/type.h"

#include "game/ientity.h"
#include "game/positioncomponent.h"

#include <tinyxml2.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

struct MovementComponent::Private
{
	Private(void)
	    : limit_x(-1.f, -1.f)
	    , limit_y(-1.f, -1.f)
	    , position(0)
	{}

	Math::Vector2 acceleration;
	Math::Vector2 velocity;
	Math::Pair limit_x;
	Math::Pair limit_y;
	PositionComponent *position;
};

MovementComponent::MovementComponent(const Core::Identifier &i, IEntity &e)
    : ComponentBase(i, e)
    , PIMPL_CREATE
{
}

MovementComponent::~MovementComponent(void)
{
	PIMPL_DESTROY;
}

Math::Vector2 &
MovementComponent::acceleration(void)
{
	return(PIMPL->acceleration);
}

Math::Pair &
MovementComponent::limitX(void)
{
	return(PIMPL->limit_x);
}

Math::Pair &
MovementComponent::limitY(void)
{
	return(PIMPL->limit_y);
}

Math::Vector2 &
MovementComponent::velocity(void)
{
	return(PIMPL->velocity);
}

Math::Point2
MovementComponent::simulate(float d) const
{
	if (PIMPL->position)
		return(PIMPL->position->position() + (PIMPL->velocity * d));
	else MMWARNING("MovementComponent::simulate didn't find a position component.");
	return(Math::Point2::Zero());
}

void
MovementComponent::update(float d)
{
	if (!PIMPL->position) {
		PIMPL->position = static_cast<PositionComponent *>
		    (entity().getComponentType("Game::PositionComponent"));
	}

	const Math::Pair &l_limit_x = PIMPL->limit_x;
	const Math::Pair &l_limit_y = PIMPL->limit_y;
	Math::Vector2 &l_velocity = PIMPL->velocity;

	/* update velocity */

	l_velocity += PIMPL->acceleration * d;

	/* check limit */

	if (l_limit_x.first()  > -1 && l_velocity.x < -l_limit_x.first())
		l_velocity.x = -l_limit_x.first();
	if (l_limit_x.second() > -1 && l_velocity.x >  l_limit_x.second())
		l_velocity.x =  l_limit_x.second();

	if (l_limit_y.first()  > -1 && l_velocity.y < -l_limit_y.first())
		l_velocity.y = -l_limit_y.first();
	if (l_limit_y.second() > -1 && l_velocity.y >  l_limit_y.second())
		l_velocity.y =  l_limit_y.second();

	/* update position */

	PIMPL->position->position() += l_velocity * d;
}

bool
MovementComponent::serialize(XMLElement &n) const
{
	if (!ComponentBase::serialize(n))
	    return(false);

	XMLElement *l_acceleration = n.GetDocument()->NewElement("acceleration");
	l_acceleration->SetAttribute("x", PIMPL->acceleration.x);
	l_acceleration->SetAttribute("y", PIMPL->acceleration.y);
	n.InsertEndChild(l_acceleration);

	XMLElement *l_limit = n.GetDocument()->NewElement("limit");
	l_limit->SetAttribute("x1", PIMPL->limit_x.first());
	l_limit->SetAttribute("x2", PIMPL->limit_x.second());
	l_limit->SetAttribute("y1", PIMPL->limit_y.first());
	l_limit->SetAttribute("y2", PIMPL->limit_y.second());
	n.InsertEndChild(l_limit);

	XMLElement *l_velocity = n.GetDocument()->NewElement("velocity");
	l_velocity->SetAttribute("x", PIMPL->velocity.x);
	l_velocity->SetAttribute("y", PIMPL->velocity.y);
	n.InsertEndChild(l_velocity);

	return(true);
}

bool
MovementComponent::deserialize(XMLElement &n)
{
	if (!ComponentBase::deserialize(n))
	    return(false);

	XMLElement *l_acceleration = n.FirstChildElement( "acceleration" );
	if (l_acceleration) {
		l_acceleration->QueryFloatAttribute("x", &PIMPL->acceleration.x);
		l_acceleration->QueryFloatAttribute("y", &PIMPL->acceleration.y);
	}

	XMLElement *l_limit = n.FirstChildElement( "limit" );
	if (l_limit) {
		l_limit->QueryFloatAttribute("x1", &PIMPL->limit_x[0]);
		l_limit->QueryFloatAttribute("x2", &PIMPL->limit_x[1]);
		l_limit->QueryFloatAttribute("y1", &PIMPL->limit_y[0]);
		l_limit->QueryFloatAttribute("y2", &PIMPL->limit_y[1]);
	}

	XMLElement *l_velocity = n.FirstChildElement( "velocity" );
	if (l_velocity) {
		l_velocity->QueryFloatAttribute("x", &PIMPL->velocity.x);
		l_velocity->QueryFloatAttribute("y", &PIMPL->velocity.y);
	}

	return(true);
}

const Core::Type &
MovementComponent::Type(void)
{
	static const Core::Type s_type("Game::MovementComponent");
	return(s_type);
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

