/*
 * Copyright 2011-2013 Marshmallow Engine. All rights reserved.
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

#include "game/movementcomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/logger.h"
#include "core/weak.h"

#include "game/ientity.h"
#include "game/positioncomponent.h"

#include <tinyxml2.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

struct MovementComponent::Private
{
	Private(void)
	    : limit_x(-1.f, -1.f)
	    , limit_y(-1.f, -1.f) {}

	WeakPositionComponent position;
	Math::Vector2 acceleration;
	Math::Pair limit_x;
	Math::Pair limit_y;
	Math::Vector2 velocity;
};

MovementComponent::MovementComponent(const Core::Identifier &i, IEntity &e)
    : ComponentBase(i, e)
    , m_p(new Private)
{
}

MovementComponent::~MovementComponent(void)
{
	delete m_p, m_p = 0;
}

Math::Vector2 &
MovementComponent::acceleration(void)
{
	return(m_p->acceleration);
}

Math::Pair &
MovementComponent::limitX(void)
{
	return(m_p->limit_x);
}

Math::Pair &
MovementComponent::limitY(void)
{
	return(m_p->limit_y);
}

Math::Vector2 &
MovementComponent::velocity(void)
{
	return(m_p->velocity);
}

Math::Point2
MovementComponent::simulate(float d) const
{
	if (m_p->position)
		return(m_p->position->position() + (m_p->velocity * d));
	else MMWARNING("MovementComponent::simulate didn't find a position component.");
	return(Math::Point2::Zero());
}

void
MovementComponent::update(float d)
{
	if (!m_p->position) {
		m_p->position = entity().getComponentType("Game::PositionComponent").
		    staticCast<PositionComponent>();
	}

	const Math::Pair &l_limit_x = m_p->limit_x;
	const Math::Pair &l_limit_y = m_p->limit_y;
	Math::Vector2 &l_velocity = m_p->velocity;

	/* update velocity */

	l_velocity += m_p->acceleration * d;

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

	m_p->position->position() += l_velocity * d;
}

bool
MovementComponent::serialize(XMLElement &n) const
{
	if (!ComponentBase::serialize(n))
	    return(false);

	XMLElement *l_acceleration = n.GetDocument()->NewElement("acceleration");
	l_acceleration->SetAttribute("x", m_p->acceleration.x);
	l_acceleration->SetAttribute("y", m_p->acceleration.y);
	n.InsertEndChild(l_acceleration);

	XMLElement *l_limit = n.GetDocument()->NewElement("limit");
	l_limit->SetAttribute("x1", m_p->limit_x.first());
	l_limit->SetAttribute("x2", m_p->limit_x.second());
	l_limit->SetAttribute("y1", m_p->limit_y.first());
	l_limit->SetAttribute("y2", m_p->limit_y.second());
	n.InsertEndChild(l_limit);

	XMLElement *l_velocity = n.GetDocument()->NewElement("velocity");
	l_velocity->SetAttribute("x", m_p->velocity.x);
	l_velocity->SetAttribute("y", m_p->velocity.y);
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
		l_acceleration->QueryFloatAttribute("x", &m_p->acceleration.x);
		l_acceleration->QueryFloatAttribute("y", &m_p->acceleration.y);
	}

	XMLElement *l_limit = n.FirstChildElement( "limit" );
	if (l_limit) {
		l_limit->QueryFloatAttribute("x1", &m_p->limit_x[0]);
		l_limit->QueryFloatAttribute("x2", &m_p->limit_x[1]);
		l_limit->QueryFloatAttribute("y1", &m_p->limit_y[0]);
		l_limit->QueryFloatAttribute("y2", &m_p->limit_y[1]);
	}

	XMLElement *l_velocity = n.FirstChildElement( "velocity" );
	if (l_velocity) {
		l_velocity->QueryFloatAttribute("x", &m_p->velocity.x);
		l_velocity->QueryFloatAttribute("y", &m_p->velocity.y);
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

