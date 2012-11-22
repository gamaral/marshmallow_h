/*
 * Copyright 2011-2012 Marshmallow Engine. All rights reserved.
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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef DEMO_INPUTCOMPONENT_H
#define DEMO_INPUTCOMPONENT_H 1

#include <game/componentbase.h>
#include <event/ieventlistener.h>

#include <list>

#include <core/weak.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game
{
	class MovementComponent;
	typedef Core::Weak<MovementComponent> WeakMovementComponent;

	class PositionComponent;
	typedef Core::Weak<PositionComponent> WeakPositionComponent;
}
MARSHMALLOW_NAMESPACE_END

MARSHMALLOW_NAMESPACE_USE

class ActorColliderComponent;
typedef Core::Weak<ActorColliderComponent> WeakActorColliderComponent;

class InputComponent : public Game::ComponentBase
                     , public Event::IEventListener
{
	WeakActorColliderComponent m_collider;
	Game::WeakMovementComponent m_movement;
	Game::WeakPositionComponent m_position;

	std::list<int> m_direction_stack;
	float m_linear_impulse;
	float m_max_speed;
	float m_jump;
	float m_boost_fuel;
	int   m_direction;
	bool  m_left;
	bool  m_right;
	bool  m_enabled;

	NO_ASSIGN_COPY(InputComponent);
public:

	InputComponent(const Core::Identifier &identifier, Game::IEntity &entity);
	virtual ~InputComponent(void);

	enum Direction {
		ICDLeft  = 0,
		ICDRight = 1,
		ICDJump  = 2
	};

	inline void enable()
	    { m_enabled = true; }
	inline void disable()
	    { m_enabled = false; }
	inline bool isEnabled() const
	    { return(m_enabled); }

	inline Direction direction(void) const
	    { return(static_cast<Direction>(m_direction)); }

	bool inMotion(void) const;

	float linearRatio(void) const;

public: /* virtual */

	VIRTUAL const Core::Type & type(void) const
	    { return(Type); }

	VIRTUAL void update(float d);

	VIRTUAL bool handleEvent(const Event::IEvent &event);

public: /* static */

	static const Core::Type Type;
};
typedef Core::Shared<InputComponent> SharedInputComponent;


#endif

