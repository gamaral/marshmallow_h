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

#include <Box2D/Box2D.h>

#include <core/logger.h>

#include <math/size2.h>

#include <event/eventmanager.h>
#include <event/keyboardevent.h>
#include <event/proxyeventlistener.h>

#include <game/enginebase.h>
#include <game/ientity.h>

#include <graphics/viewport.h>

const Core::Type InputComponent::Type("InputComponent");

InputComponent::InputComponent(const Core::Identifier &i, Game::IEntity &e)
    : ComponentBase(i, e)
    , m_linear_impulse(100.f)
    , m_jump(false)
    , m_left(false)
    , m_right(false)
    , m_direction(ICDRight)
{
	m_event_proxy = new Event::ProxyEventListener(*this);
	Game::EngineBase::Instance()->eventManager()->connect(m_event_proxy, Event::KeyboardEvent::Type());
}

InputComponent::~InputComponent(void)
{
}

bool
InputComponent::inMotion(void) const
{
	return(((direction() == ICDLeft)  && m_left)
	    || ((direction() == ICDRight) && m_right));
}

void
InputComponent::update(float)
{
	if (!m_position)
		m_position = entity().getComponentType(Game::PositionComponent::Type()).
		    staticCast<Game::PositionComponent>();

	if (!m_body) {
		m_body = entity().getComponentType(Game::Box2DComponent::Type()).
		    staticCast<Game::Box2DComponent>();
	}

	if (m_position && m_body) {
		b2Vec2 l_vel = m_body->body()->GetLinearVelocity();
		const float l_mass = m_body->body()->GetMass();
		b2Vec2 l_impulse(0, 0);

		if (inMotion())
		switch (direction()) {
		case  ICDLeft:
			if (l_vel.x > -10) l_impulse(0) = -m_linear_impulse;
			l_impulse(1) = l_vel.y * -1;
			break;
		case ICDRight:
			if (l_vel.x <  10) l_impulse(0) = m_linear_impulse;
			l_impulse(1) = l_vel.y * -1;
			break;
		}
		else l_impulse(0) = l_vel.x * -1, l_impulse(1) = l_vel.y * -1;

		if (m_jump) {
			l_impulse(1) = m_linear_impulse * 20;
			m_jump = false;
		}

		m_body->body()->ApplyLinearImpulse(l_impulse, m_body->body()->GetWorldCenter());
	}
}

bool
InputComponent::handleEvent(const Event::IEvent &e)
{
	if (e.type() != Event::KeyboardEvent::Type())
		return(false);

	const Event::KeyboardEvent &l_kevent =
	    static_cast<const Event::KeyboardEvent &>(e);

	if (l_kevent.key() == Event::KEY_LEFT) {
		if (m_left = (l_kevent.action() == Event::KeyPressed))
			m_direction_stack.push_front(ICDLeft);
		else m_direction_stack.remove(ICDLeft);
	}
	else if (l_kevent.key() == Event::KEY_RIGHT) {
		if (m_right = (l_kevent.action() == Event::KeyPressed))
			m_direction_stack.push_front(ICDRight);
		else m_direction_stack.remove(ICDRight);
	}
	else if (l_kevent.key() == Event::KEY_SPACE) {
		m_jump = (l_kevent.action() == Event::KeyPressed);
	}

	if (m_direction_stack.size() > 0)
		m_direction = m_direction_stack.front();

	return(false);
}

