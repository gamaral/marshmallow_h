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

#include <tinyxml.h>

#include "game/ientity.h"
#include "game/positioncomponent.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Game;

const Core::Type MovementComponent::Type("Game::MovementComponent");

MovementComponent::MovementComponent(const Core::Identifier &i, IEntity &e)
    : ComponentBase(i, e),
      m_position(),
      m_direction()
{
}

MovementComponent::~MovementComponent(void)
{
}

void
MovementComponent::update(TIME d)
{
	UNUSED(d);

	if (!m_position)
		m_position = entity().getComponentType("Game::PositionComponent").
		    staticCast<PositionComponent>();

	if (m_position && m_direction)
		m_position->position() += m_direction * static_cast<float>(d);

}

bool
MovementComponent::serialize(TinyXML::TiXmlElement &n) const
{
	n.SetAttribute("id", id().str());
	n.SetAttribute("type", type().str());
	n.SetDoubleAttribute("x", m_direction.rx());
	n.SetDoubleAttribute("y", m_direction.ry());
	return(true);
}

bool
MovementComponent::deserialize(TinyXML::TiXmlElement &n)
{
	n.QueryFloatAttribute("x", &m_direction.rx());
	n.QueryFloatAttribute("y", &m_direction.ry());
	return(true);
}

