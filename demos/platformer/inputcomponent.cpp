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

#include <core/logger.h>

#include <math/size2.h>

#include <event/eventmanager.h>
#include <event/keyboardevent.h>

#include <graphics/viewport.h>

#include <game/collidercomponent.h>
#include <game/engine.h>
#include <game/ientity.h>
#include <game/movementcomponent.h>
#include <game/positioncomponent.h>

#include "playercollidercomponent.h"

const Core::Type InputComponent::Type("InputComponent");

#define JUMP_MAX  200.f
#define BOOST_MAX 0.1f

InputComponent::InputComponent(const Core::Identifier &i, Game::IEntity &e)
    : ComponentBase(i, e)
    , m_collider()
    , m_linear_impulse(300.f)
    , m_max_speed(200.f)
    , m_jump(false)
    , m_left(false)
    , m_right(false)
    , m_direction(ICDRight)
{
	Game::Engine::Instance()->eventManager()->connect(this, Event::KeyboardEvent::Type());
}

InputComponent::~InputComponent(void)
{
	Game::Engine::Instance()->eventManager()->disconnect(this, Event::KeyboardEvent::Type());
}

bool
InputComponent::inMotion(void) const
{
	Direction l_dir = direction();
	return((l_dir == ICDLeft  && m_left) ||
	       (l_dir == ICDRight && m_right));
}

void
InputComponent::update(float d)
{
	if (!m_collider)
		m_collider = entity().getComponentType(Game::ColliderComponent::Type()).
		    staticCast<PlayerColliderComponent>();

	if (!m_position)
		m_position = entity().getComponentType(Game::PositionComponent::Type()).
		    staticCast<Game::PositionComponent>();

	if (!m_movement) {
		m_movement = entity().getComponentType(Game::MovementComponent::Type()).
		    staticCast<Game::MovementComponent>();

		if (m_movement) {
			m_movement->acceleration()[1] = -1000.f;
			m_movement->limitY()[0] = 900.f;
		}
	}

	if (m_collider && m_position && m_movement) {
		/* speed limits */
		if (inMotion()) {
			switch (direction()) {
			case  ICDLeft:
				m_movement->acceleration()[0] = -m_linear_impulse;

				/* sharp u turn */
				if (m_collider->onPlatform() && m_movement->velocity().x() > 0)
					m_movement->acceleration()[0] *= 3.f;
				else if (m_movement->velocity().x() < -m_max_speed)
					m_movement->acceleration()[0] *= -0.5f;
				break;
			case ICDRight:
				m_movement->acceleration()[0] = m_linear_impulse;

				/* sharp u turn */
				if (m_collider->onPlatform() && m_movement->velocity().x() < 0)
					m_movement->acceleration()[0] *= 3.f;
				else if (m_movement->velocity().x() > m_max_speed)
					m_movement->acceleration()[0] *= -0.5f;
				break;
			default: break;
			}
		}
		/* stop unless falling */
		else if(m_collider->onPlatform()) {
			switch (direction()) {
			case  ICDLeft:
				if (m_movement->velocity().x() > -1) {
					m_movement->velocity()[0] = 0;
					m_movement->acceleration()[0] = 0;
				} else m_movement->acceleration()[0] = m_linear_impulse * 4.f;
				break;
			case ICDRight:
				if (m_movement->velocity().x() < 1) {
					m_movement->velocity()[0] = 0;
					m_movement->acceleration()[0] = 0;
				} else m_movement->acceleration()[0] = -m_linear_impulse * 4.f;
				break;
			default: break;
			}
		}
		else m_movement->acceleration()[0] = 0;

		/* jumping */
		if (m_jump) {
			if (m_boost_fuel > 0) {
				m_boost_fuel -= d;
				m_movement->velocity()[1] += JUMP_MAX * (d / BOOST_MAX);
			}
			else if (m_collider->onPlatform() && m_movement->velocity()[1] < 1) {
				m_movement->velocity()[1] = JUMP_MAX;
				m_boost_fuel = BOOST_MAX;
			}
		}
	}
}

bool
InputComponent::handleEvent(const Event::IEvent &e)
{
	if (e.type() != Event::KeyboardEvent::Type())
		return(false);

	const Event::KeyboardEvent &l_kevent =
	    static_cast<const Event::KeyboardEvent &>(e);

	if (l_kevent.key() == Event::KEY_LEFT ||
            l_kevent.key() == Event::KEY_A) {
		if ((m_left = (l_kevent.action() == Event::KeyPressed)))
			m_direction_stack.push_front(ICDLeft);
		else m_direction_stack.remove(ICDLeft);
	}
	else if (l_kevent.key() == Event::KEY_RIGHT ||
                 l_kevent.key() == Event::KEY_D) {
		if ((m_right = (l_kevent.action() == Event::KeyPressed)))
			m_direction_stack.push_front(ICDRight);
		else m_direction_stack.remove(ICDRight);
	}
	else if (l_kevent.key() == Event::KEY_SPACE ||
                 l_kevent.key() == Event::KEY_M) {
		m_jump = (l_kevent.action() == Event::KeyPressed ? true : false);
	}
	else if (l_kevent.key() == Event::KEY_SHIFT_L ||
                 l_kevent.key() == Event::KEY_J) {
		m_max_speed += l_kevent.action() == Event::KeyPressed ? 200 : -200;
	}

	if (m_direction_stack.size() > 0)
		m_direction = m_direction_stack.front();

	return(false);
}

