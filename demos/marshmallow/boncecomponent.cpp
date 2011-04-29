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

#include "bouncecomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <core/logger.h>
#include <math/size2.h>
#include <graphics/viewport.h>
#include <game/ientity.h>

const Core::Type BounceComponent::Type("BounceComponent");

BounceComponent::BounceComponent(const Core::Identifier &i, Game::IEntity &e)
    : ComponentBase(i, e)
{
	
}

BounceComponent::~BounceComponent(void)
{
}

void
BounceComponent::update(TIME d)
{
	if (!m_position)
		m_position = entity().getComponentType("Game::PositionComponent").
		    cast<Game::PositionComponent>();

	if (!m_movement)
		m_movement = entity().getComponentType("Game::MovementComponent").
		    cast<Game::MovementComponent>();

	if (m_position && m_movement) {
		INFO("DELTA: %f", d);
		INFO("Current position %f, %f",
		    m_position->position().rx(), m_position->position().ry());

		Math::Size2 l_vpsize = Graphics::Viewport::Size();

		Math::Point2  &pos = m_position->position();
		Math::Vector2 &dir = m_movement->direction();

		if ((pos.rx() <= -l_vpsize.width() / 2 && dir.rx() < 0)
		 || (pos.rx() >=  l_vpsize.width() / 2 && dir.rx() > 0))
			dir.rx() *= -0.95f;
		if ((pos.ry() <= -l_vpsize.height() / 2 && dir.ry() < 0)
		 || (pos.ry() >=  l_vpsize.height() / 2 && dir.ry() > 0))
			dir.ry() *= -0.95f;
	}
}

