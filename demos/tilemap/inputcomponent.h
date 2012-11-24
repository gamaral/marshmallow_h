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

#ifndef TILEMAP_INPUTCOMPONENT_H
#define TILEMAP_INPUTCOMPONENT_H 1

#include <game/componentbase.h>
#include <event/ieventlistener.h>

#include <core/weak.h>

#include <game/movementcomponent.h>
#include <game/positioncomponent.h>

#include <list>

MARSHMALLOW_NAMESPACE_USE

class InputComponent : public Game::ComponentBase
                     , public Event::IEventListener
{
	Game::WeakPositionComponent m_position;
	Game::WeakMovementComponent m_movement;

	std::list<int> m_direction_stack;
	float m_linear_impulse;
	int   m_direction;
	bool  m_down;
	bool  m_left;
	bool  m_right;
	bool  m_up;

	NO_ASSIGN_COPY(InputComponent);
public:

	InputComponent(const Core::Identifier &identifier, Game::IEntity &entity);
	virtual ~InputComponent(void);

	enum Direction {
		ICDDown  = 0,
		ICDLeft  = 1,
		ICDRight = 2,
		ICDUp    = 3
	};

	Direction direction(void) const
	    { return(static_cast<Direction>(m_direction)); }

	bool inMotion(void) const;

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

