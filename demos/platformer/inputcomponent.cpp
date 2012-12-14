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
#include <event/joystickaxisevent.h>
#include <event/joystickbuttonevent.h>
#include <event/keyboardevent.h>
#include <event/touchevent.h>

#include <graphics/viewport.h>

#include <game/collidercomponent.h>
#include <game/engine.h>
#include <game/iengine.h>
#include <game/ientity.h>
#include <game/movementcomponent.h>
#include <game/positioncomponent.h>

#include "actorcollidercomponent.h"

const Core::Type InputComponent::Type("InputComponent");

#define LINEAR_MAX 300.f
#define JUMP_MAX   200.f
#define BOOST_MAX  .1f
#define TOUCH_AREA 200

InputComponent::InputComponent(const Core::Identifier &i, Game::IEntity &e)
    : ComponentBase(i, e)
    , m_collider()
    , m_linear_impulse(LINEAR_MAX)
    , m_max_speed(LINEAR_MAX / 2.f)
    , m_jump(false)
    , m_boost_fuel(.0f)
    , m_direction(ICDRight)
    , m_left(false)
    , m_right(false)
    , m_enabled(false)
{
	Game::Engine::Instance()->eventManager()->connect(this, Event::JoystickAxisEvent::Type());
	Game::Engine::Instance()->eventManager()->connect(this, Event::JoystickButtonEvent::Type());
	Game::Engine::Instance()->eventManager()->connect(this, Event::KeyboardEvent::Type());
	Game::Engine::Instance()->eventManager()->connect(this, Event::TouchEvent::Type());
	enable();
}

InputComponent::~InputComponent(void)
{
	Game::Engine::Instance()->eventManager()->disconnect(this, Event::TouchEvent::Type());
	Game::Engine::Instance()->eventManager()->disconnect(this, Event::KeyboardEvent::Type());
	Game::Engine::Instance()->eventManager()->disconnect(this, Event::JoystickButtonEvent::Type());
	Game::Engine::Instance()->eventManager()->disconnect(this, Event::JoystickAxisEvent::Type());
}

bool
InputComponent::inMotion(void) const
{
	Direction l_dir = direction();
	return((l_dir == ICDLeft  && m_left) ||
	       (l_dir == ICDRight && m_right));
}

float
InputComponent::linearRatio(void) const
{
	return(m_linear_impulse / LINEAR_MAX);
}

void
InputComponent::update(float d)
{
	if (!m_enabled)
		return;

	if (!m_collider)
		m_collider = entity().getComponentType(Game::ColliderComponent::Type()).
		    staticCast<ActorColliderComponent>();

	if (!m_position)
		m_position = entity().getComponentType(Game::PositionComponent::Type()).
		    staticCast<Game::PositionComponent>();

	if (!m_movement) {
		m_movement = entity().getComponentType(Game::MovementComponent::Type()).
		    staticCast<Game::MovementComponent>();

		if (m_movement) {
			m_movement->acceleration().y = -1000.f;
			m_movement->limitY()[0] = 900.f;
		}
	}

	if (m_collider && m_position && m_movement) {
		/* speed limits */
		if (inMotion()) {
			switch (direction()) {
			case  ICDLeft:
				m_movement->acceleration().x = m_linear_impulse;

				/* sharp u turn */
				if (m_collider->onPlatform() && m_movement->velocity().x > 0)
					m_movement->acceleration().x *= 3.f;
				else if (m_movement->velocity().x < -m_max_speed)
					m_movement->acceleration().x *= -0.5f;
				break;
			case ICDRight:
				m_movement->acceleration().x = m_linear_impulse;

				/* sharp u turn */
				if (m_collider->onPlatform() && m_movement->velocity().x < 0)
					m_movement->acceleration().x *= 3.f;
				else if (m_movement->velocity().x > m_max_speed)
					m_movement->acceleration().x *= -0.5f;
				break;
			default: break;
			}
		}

		/* stop unless falling */
		else if(m_collider->onPlatform()) {
			switch (direction()) {
			case  ICDLeft:
				if (m_movement->velocity().x > -1) {
					m_movement->velocity().x = 0;
					m_movement->acceleration().x = 0;
				} else m_movement->acceleration().x = LINEAR_MAX * 4.f;
				break;
			case ICDRight:
				if (m_movement->velocity().x < 1) {
					m_movement->velocity().x = 0;
					m_movement->acceleration().x = 0;
				} else m_movement->acceleration().x = -LINEAR_MAX * 4.f;
				break;
			default: break;
			}
		}
		else m_movement->acceleration().x = 0;

		/* jumping */
		if (m_jump) {
			if (m_boost_fuel > 0 && m_movement->velocity().y > 0) {
				m_boost_fuel -= d;
				m_movement->velocity().y += JUMP_MAX * (d / BOOST_MAX);
			}
			else if (m_movement->velocity().y < 1)
				m_jump = false;
		}
		else m_boost_fuel = 0;
	}
}

bool
InputComponent::handleEvent(const Event::IEvent &e)
{
	using namespace Input;

	if (e.type() == Event::KeyboardEvent::Type()) {
		const Event::KeyboardEvent &l_kevent =
		    static_cast<const Event::KeyboardEvent &>(e);

		if (l_kevent.key() == Keyboard::KBK_LEFT ||
		    l_kevent.key() == Keyboard::KBK_A) {
			if ((m_left = (l_kevent.action() == Keyboard::KeyPressed))) {
				m_direction_stack.push_front(ICDLeft);
				m_linear_impulse = -LINEAR_MAX;
			}
			else {
				m_direction_stack.remove(ICDLeft);
				m_linear_impulse = LINEAR_MAX;
			}
		}
		else if (l_kevent.key() == Keyboard::KBK_RIGHT ||
			 l_kevent.key() == Keyboard::KBK_D) {
			if ((m_right = (l_kevent.action() == Keyboard::KeyPressed))) {
				m_direction_stack.push_front(ICDRight);
				m_linear_impulse = LINEAR_MAX;
			}
			else {
				m_direction_stack.remove(ICDRight);
				m_linear_impulse = -LINEAR_MAX;
			}
		}
		else if (l_kevent.key() == Keyboard::KBK_SPACE ||
			 l_kevent.key() == Keyboard::KBK_M) {

			if (!m_jump &&
			    l_kevent.action() == Keyboard::KeyPressed &&
			    m_collider->onPlatform()) {
				m_movement->velocity().y = JUMP_MAX;
				m_boost_fuel = BOOST_MAX;
			}

			m_jump = (l_kevent.action() == Keyboard::KeyPressed ? true : false);
		}
		else if (l_kevent.key() == Keyboard::KBK_SHIFT_L ||
			 l_kevent.key() == Keyboard::KBK_J) {
			m_max_speed += .60f * (l_kevent.action() == Keyboard::KeyPressed ? LINEAR_MAX : -LINEAR_MAX);

			/*
			 * Ugly workaround for input bug
			 */
			if (m_max_speed < (LINEAR_MAX / 2.f))
				m_max_speed = LINEAR_MAX / 2.f;
		}
	}
	else if (e.type() == Event::JoystickButtonEvent::Type()) {
		const Event::JoystickButtonEvent &l_event =
		    static_cast<const Event::JoystickButtonEvent &>(e);

		/* TODO(gamaral) clean this stuff up, move into jump method or event  */
		if (l_event.button() == Joystick::JSB_B) {
			if (!m_jump &&
			    l_event.action() == Joystick::ButtonPressed &&
			    m_collider->onPlatform()) {
				m_movement->velocity().y = JUMP_MAX;
				m_boost_fuel = BOOST_MAX;
			}

			m_jump = (l_event.action() == Joystick::ButtonPressed ? true : false);
		}
		else if (l_event.button() == Joystick::JSB_Y) {
			m_max_speed += .60f * (l_event.action() == Joystick::ButtonPressed ? LINEAR_MAX : -LINEAR_MAX);

			/*
			 * Ugly workaround for input bug
			 */
			if (m_max_speed < (LINEAR_MAX / 2.f))
				m_max_speed = LINEAR_MAX / 2.f;
		}
	}
	else if (e.type() == Event::JoystickAxisEvent::Type()) {
		const Event::JoystickAxisEvent &l_event =
		    static_cast<const Event::JoystickAxisEvent &>(e);

		if (l_event.axis() == Joystick::JSA_HX
		    || l_event.axis() == Joystick::JSA_X) {
			float l_range  = (l_event.maximum() - l_event.minimum()) / 2.f;
			float l_middle = l_event.minimum() + l_range;
			float l_value  = (l_event.value() - l_middle) / l_range;

			m_left = (l_value < -0.15);
			if (m_left) {
				m_direction_stack.push_front(ICDLeft);
				m_linear_impulse = LINEAR_MAX * l_value;
			}
			else m_direction_stack.remove(ICDLeft);

			m_right = (l_value > 0.15);
			if (m_right) {
				m_direction_stack.push_front(ICDRight);
				m_linear_impulse = LINEAR_MAX * l_value;
			}
			else m_direction_stack.remove(ICDRight);
		}
	}
	else if (e.type() == Event::TouchEvent::Type()) {
		const Event::TouchEvent &l_event =
		    static_cast<const Event::TouchEvent &>(e);
#if 0
		if (!m_jump && l_event.action() == Input::Touch::Press) {
			if (m_collider->onPlatform()) {
				m_movement->velocity().y = JUMP_MAX;
				m_boost_fuel = BOOST_MAX;
			}
			m_jump = true;
		}
		else if (m_jump && (l_event.action() == Input::Touch::Release))
			m_jump = false;
#endif

		if (l_event.action() == Input::Touch::Press) {
			if (!m_left && !m_right) {
				if (l_event.x() < TOUCH_AREA) {
					m_left = true;
					m_direction_stack.push_front(ICDLeft);
					m_linear_impulse = -LINEAR_MAX;
				}
				else if (l_event.x() > (Graphics::Viewport::WindowSize().width - TOUCH_AREA)) {
					m_right = true;
					m_direction_stack.push_front(ICDRight);
					m_linear_impulse = LINEAR_MAX;
				}
			}
			else {
				if (!m_jump && m_collider->onPlatform()) {
					m_movement->velocity().y = JUMP_MAX;
					m_boost_fuel = BOOST_MAX;
				}
				m_jump = true;

				/*
				 * Ugly workaround for input bug
				 */
				m_max_speed += .60f * LINEAR_MAX;
				if (m_max_speed < (LINEAR_MAX / 2.f))
					m_max_speed = LINEAR_MAX / 2.f;
			}

		}
		else if (l_event.action() == Input::Touch::Release) {
			if (m_left && (l_event.x() < TOUCH_AREA)) {
				m_left = false;
				m_direction_stack.remove(ICDLeft);
			}
			else if (m_right && (l_event.x() > (Graphics::Viewport::WindowSize().width - TOUCH_AREA))) {
				m_right = false;
				m_direction_stack.remove(ICDRight);
			}
			else {
				if (!m_jump && m_collider->onPlatform()) {
					m_movement->velocity().y = JUMP_MAX;
					m_boost_fuel = BOOST_MAX;
				}
				m_jump = true;

				/*
				 * Ugly workaround for input bug
				 */
				m_max_speed += .60f * -LINEAR_MAX;
				if (m_max_speed < (LINEAR_MAX / 2.f))
					m_max_speed = LINEAR_MAX / 2.f;
			}
		}
	}

	if (m_direction_stack.size() > 0)
		m_direction = m_direction_stack.front();

	return(false);
}

