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

#include "game/config.h"
#include "game/ientity.h"
#include "game/positioncomponent.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

struct MovementComponent::Private
{
	Private(void)
	    : limit_x(-1.f, -1.f)
	    , limit_y(-1.f, -1.f)
	    , position(0)
	    , accumulator(.0f)
	{}

	inline void update(float d);

	Math::Vector2 acceleration;
	Math::Vector2 velocity;
	Math::Pair limit_x;
	Math::Pair limit_y;
	PositionComponent *position;
	float accumulator;
};

void
MovementComponent::Private::update(float d)
{
	static const float step(1.f/60.f);

	if (!position) {
		MMDEBUG("Game::PositionComponent not found!");
		return;
	}

	accumulator += d;
	while (accumulator >= step) {
		accumulator -= step;

		/* update velocity */

		velocity += acceleration * step;

		/* check limit */

		if (limit_x.first()  > -1 && velocity.x < -limit_x.first())
			velocity.x = -limit_x.first();
		if (limit_x.second() > -1 && velocity.x >  limit_x.second())
			velocity.x =  limit_x.second();

		if (limit_y.first()  > -1 && velocity.y < -limit_y.first())
			velocity.y = -limit_y.first();
		if (limit_y.second() > -1 && velocity.y >  limit_y.second())
			velocity.y =  limit_y.second();

		/* update position */

		position->translate(velocity * step);
	}
}

MovementComponent::MovementComponent(const Core::Identifier &i, Game::IEntity *e)
    : Component(i, e)
    , PIMPL_CREATE
{
}

MovementComponent::~MovementComponent(void)
{
	PIMPL_DESTROY;
}

const Math::Vector2 &
MovementComponent::acceleration(void) const
{
	return(PIMPL->acceleration);
}

float
MovementComponent::accelerationX(void) const
{
	return(PIMPL->acceleration.x);
}

float
MovementComponent::accelerationY(void) const
{
	return(PIMPL->acceleration.x);
}

void
MovementComponent::setAcceleration(const Math::Vector2 &a)
{
	PIMPL->acceleration = a;
}
		
void
MovementComponent::setAcceleration(float x, float y)
{
	PIMPL->acceleration.set(x, y);
}

void
MovementComponent::setAccelerationX(float x)
{
	PIMPL->acceleration.x = x;
}

void
MovementComponent::setAccelerationY(float y)
{
	PIMPL->acceleration.y = y;
}

const Math::Pair &
MovementComponent::limitX(void) const
{
	return(PIMPL->limit_x);
}

void
MovementComponent::setLimitX(const Math::Pair &l)
{
	PIMPL->limit_x = l;
}

const Math::Pair &
MovementComponent::limitY(void) const
{
	return(PIMPL->limit_y);
}

void
MovementComponent::setLimitY(const Math::Pair &l)
{
	PIMPL->limit_y = l;
}

const Math::Vector2 &
MovementComponent::velocity(void) const
{
	return(PIMPL->velocity);
}

float
MovementComponent::velocityX(void) const
{
	return(PIMPL->velocity.x);
}

float
MovementComponent::velocityY(void) const
{
	return(PIMPL->velocity.y);
}

void
MovementComponent::setVelocity(const Math::Vector2 &v)
{
	PIMPL->velocity = v;
}

void
MovementComponent::setVelocity(float x, float y)
{
	PIMPL->velocity.set(x, y);
}

void
MovementComponent::setVelocityX(float x)
{
	PIMPL->velocity.x = x;
}

void
MovementComponent::setVelocityY(float y)
{
	PIMPL->velocity.y = y;
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
		    (entity()->getComponentType(Game::PositionComponent::Type()));
	}

	PIMPL->update(d);
}

const Core::Type &
MovementComponent::Type(void)
{
	static const Core::Type s_type("Game::MovementComponent");
	return(s_type);
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

