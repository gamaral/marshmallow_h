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

#include "inputcomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <core/identifier.h>
#include <core/logger.h>

#include <math/size2.h>

#include <event/eventmanager.h>
#include <event/keyboardevent.h>

#include <graphics/viewport.h>

#include <game/engine.h>
#include <game/iengine.h>
#include <game/ientity.h>

#include <Box2D/Box2D.h>

#include <tinyxml2.h>

const Core::Type InputComponent::Type("InputComponent");

InputComponent::InputComponent(const Core::Identifier &i, Game::IEntity &e)
    : ComponentBase(i, e)
    , m_linear_impulse(2.f)
    , m_angular_impulse(0.008f)
    , m_state(ICJumping)
    , m_jump(false)
    , m_left(false)
    , m_right(false)
{
	Game::Engine::Instance()->eventManager()->connect(this, Event::KeyboardEvent::Type());
}

InputComponent::~InputComponent(void)
{
	Game::Engine::Instance()->eventManager()->disconnect(this, Event::KeyboardEvent::Type());
}

void
InputComponent::update(float)
{
	if (!m_position)
		m_position = entity().getComponentType("Game::PositionComponent").
		    cast<Game::PositionComponent>();

	if (!m_body) {
		m_body = entity().getComponentType("Game::Box2DComponent").
		    cast<Game::Box2DComponent>();
	}

	if (m_position && m_body) {
		b2Vec2 l_vel = m_body->body()->GetLinearVelocity();

		switch (m_state) {
		case ICJumping:
			if (l_vel.y <= 0) m_state = ICFalling;
			break;
		case ICFalling:
			if (l_vel.y >= 0) m_state = ICStanding;
			break;
		case ICStanding:
			const float l_mass = m_body->body()->GetMass();
			if (m_jump) {
				m_body->body()->ApplyLinearImpulse(b2Vec2(0, l_mass * m_linear_impulse),
				    m_body->body()->GetWorldCenter(), true);
				m_state = ICJumping;
			}

			if (m_left && l_vel.x > -.5f)
				m_body->body()->ApplyAngularImpulse(l_mass *  m_angular_impulse, true);

			if (m_right && l_vel.x < .5f)
				m_body->body()->ApplyAngularImpulse(l_mass * -m_angular_impulse, true);

			break;
		}
	}
}

bool
InputComponent::handleEvent(const Event::IEvent &e)
{
	using namespace Input;

	if (e.type() != Event::KeyboardEvent::Type())
		return(false);

	const Event::KeyboardEvent &l_kevent =
	    static_cast<const Event::KeyboardEvent &>(e);

	if (l_kevent.key() == Keyboard::KBK_SPACE)
		jump(l_kevent.action() == Keyboard::KeyPressed);
	else if (l_kevent.key() == Keyboard::KBK_LEFT)
		left(l_kevent.action() == Keyboard::KeyPressed);
	else if (l_kevent.key() == Keyboard::KBK_RIGHT)
		right(l_kevent.action() == Keyboard::KeyPressed);

	return(false);
}

bool
InputComponent::serialize(XMLElement &n) const
{
	if (!ComponentBase::serialize(n))
		return(false);

	n.SetAttribute("linear", m_linear_impulse);
	n.SetAttribute("angular", m_angular_impulse);
	return(true);
}

bool
InputComponent::deserialize(XMLElement &n)
{
	if (!ComponentBase::deserialize(n))
		return(false);

	n.QueryFloatAttribute("linear", &m_linear_impulse);
	n.QueryFloatAttribute("angular", &m_angular_impulse);
	return(true);
}

