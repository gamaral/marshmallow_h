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

#include "game/movementcomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include "game/ientity.h"
#include "game/positioncomponent.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Game;

const Core::Type MovementComponent::sType("Game::MovementComponent");

MovementComponent::MovementComponent(const Core::Identifier &i, IEntity &e)
    : ComponentBase(i, e)
    , m_position()
    , m_acceleration()
    , m_velocity()
{
}

MovementComponent::~MovementComponent(void)
{
}

Math::Point2
MovementComponent::simulate(float d) const
{
	if (m_position)
		return(m_position->position() + (m_velocity * d));
	else MMWARNING1("MovementComponent::simulate didn't find a position component.");
	return(Math::Point2::Zero());
}

void
MovementComponent::update(float d)
{
	if (!m_position) {
		m_position = entity().getComponentType("Game::PositionComponent").
		    staticCast<PositionComponent>();
	}

	m_position->position() += m_velocity * d;
	m_velocity += m_acceleration * d;
}

bool
MovementComponent::serialize(TinyXML::TiXmlElement &n) const
{
	if (!ComponentBase::serialize(n))
	    return(false);

	TinyXML::TiXmlElement l_velocity("velocity");
	l_velocity.SetDoubleAttribute("x", m_velocity.x());
	l_velocity.SetDoubleAttribute("y", m_velocity.y());
	n.InsertEndChild(l_velocity);

	TinyXML::TiXmlElement l_acceleration("acceleration");
	l_acceleration.SetDoubleAttribute("x", m_acceleration.x());
	l_acceleration.SetDoubleAttribute("y", m_acceleration.y());
	n.InsertEndChild(l_acceleration);

	return(true);
}

bool
MovementComponent::deserialize(TinyXML::TiXmlElement &n)
{
	if (!ComponentBase::deserialize(n))
	    return(false);

	TinyXML::TiXmlElement *l_velocity = n.FirstChildElement( "velocity" );
	if (l_velocity) {
		l_velocity->QueryFloatAttribute("x", &m_velocity[0]);
		l_velocity->QueryFloatAttribute("y", &m_velocity[1]);
	}

	TinyXML::TiXmlElement *l_acceleration = n.FirstChildElement( "acceleration" );
	if (l_acceleration) {
		l_acceleration->QueryFloatAttribute("x", &m_acceleration[0]);
		l_acceleration->QueryFloatAttribute("y", &m_acceleration[1]);
	}

	return(true);
}

const Core::Type &
MovementComponent::Type(void)
{
	return(sType);
}

