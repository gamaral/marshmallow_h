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

#include <game/engine.h>
#include <game/ientity.h>

#include <graphics/viewport.h>

const Core::Type InputComponent::Type("InputComponent");

InputComponent::InputComponent(const Core::Identifier &i, Game::IEntity &e)
    : ComponentBase(i, e)
    , m_linear_impulse(20.f)
    , m_down(false)
    , m_left(false)
    , m_right(false)
    , m_up(false)
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
	return(((direction() == ICDDown)  && m_down)
	    || ((direction() == ICDLeft)  && m_left)
	    || ((direction() == ICDRight) && m_right)
	    || ((direction() == ICDUp)    && m_up));
}

void
InputComponent::update(float)
{
	if (!m_position)
		m_position = entity().getComponentType(Game::PositionComponent::Type()).
		    staticCast<Game::PositionComponent>();

	if (!m_movement) {
		m_movement = entity().getComponentType(Game::MovementComponent::Type()).
		    staticCast<Game::MovementComponent>();
	}

	if (m_position && m_movement) {
		if (inMotion())
		switch (direction()) {
		case  ICDDown:
			if (m_movement->velocity()[1] < -8) m_movement->velocity()[1] = -8;
			m_movement->acceleration()[1] = -m_linear_impulse;
			m_movement->velocity()[0] = 0;
			break;
		case  ICDLeft:
			if (m_movement->velocity()[0] < -8) m_movement->velocity()[0] = -8;
			m_movement->acceleration()[0] = -m_linear_impulse;
			m_movement->velocity()[1] = 0;
			break;
		case ICDRight:
			if (m_movement->velocity()[0] > 8) m_movement->velocity()[0] = 8;
			m_movement->acceleration()[0] = m_linear_impulse;
			m_movement->velocity()[1] = 0;
			break;
		case    ICDUp:
			if (m_movement->velocity()[1] > 8) m_movement->velocity()[1] = 8;
			m_movement->acceleration()[1] = m_linear_impulse;
			m_movement->velocity()[0] = 0;
			break;
		}
		else {
			m_movement->velocity() = Math::Vector2::Zero();
			m_movement->acceleration() = Math::Vector2::Zero();
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

	if (l_kevent.key() == Event::KEY_DOWN) {
		if (m_down = (l_kevent.action() == Event::KeyPressed))
			m_direction_stack.push_front(ICDDown);
		else m_direction_stack.remove(ICDDown);
	}
	else if (l_kevent.key() == Event::KEY_LEFT) {
		if (m_left = (l_kevent.action() == Event::KeyPressed))
			m_direction_stack.push_front(ICDLeft);
		else m_direction_stack.remove(ICDLeft);
	}
	else if (l_kevent.key() == Event::KEY_RIGHT) {
		if (m_right = (l_kevent.action() == Event::KeyPressed))
			m_direction_stack.push_front(ICDRight);
		else m_direction_stack.remove(ICDRight);
	}
	else if (l_kevent.key() == Event::KEY_UP) {
		if (m_up = (l_kevent.action() == Event::KeyPressed))
			m_direction_stack.push_front(ICDUp);
		else m_direction_stack.remove(ICDUp);
	}

	if (m_direction_stack.size() > 0)
		m_direction = m_direction_stack.front();

	return(false);
}

