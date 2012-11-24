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

#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H 1

#include <game/componentbase.h>
#include <event/ieventlistener.h>

#include <core/weak.h>

#include <game/box2d/box2dcomponent.h>
#include <game/positioncomponent.h>

MARSHMALLOW_NAMESPACE_USE

class InputComponent : public Game::ComponentBase
                     , public Event::IEventListener
{
	enum State {
		ICFalling,
		ICStanding,
		ICJumping
	};

	Game::WeakPositionComponent m_position;
	Game::WeakBox2DComponent m_body;

	float m_linear_impulse;
	float m_angular_impulse;

	State m_state;

	bool m_jump;
	bool m_left;
	bool m_right;

	NO_ASSIGN_COPY(InputComponent);
public:

	InputComponent(const Core::Identifier &identifier, Game::IEntity &entity);
	virtual ~InputComponent(void);

	void jump(bool status)
	    { m_jump = status; }

	void left(bool status)
	    { m_left = status; }

	void right(bool status)
	    { m_right = status; }

public: /* virtual */

	VIRTUAL const Core::Type & type(void) const
	    { return(Type); }

	VIRTUAL void update(float d);

	VIRTUAL bool handleEvent(const Event::IEvent &event);

	VIRTUAL bool serialize(XMLElement &node) const;
	VIRTUAL bool deserialize(XMLElement &node);

public: /* static */

	static const Core::Type Type;
};

#endif

