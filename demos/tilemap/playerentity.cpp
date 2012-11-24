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

#include "playerentity.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <core/identifier.h>
#include <core/logger.h>

#include <graphics/camera.h>

#include <game/animationcomponent.h>
#include <game/collidercomponent.h>
#include <game/positioncomponent.h>
#include <game/sizecomponent.h>

#include "inputcomponent.h"

PlayerEntity::PlayerEntity(const Core::Identifier &i, Game::EntitySceneLayer &l)
    : Game::Entity(i, l)
    , m_init(false)
{
}

PlayerEntity::~PlayerEntity(void)
{
}

void
PlayerEntity::update(float d)
{
	if (!m_init) {
		Game::SharedSizeComponent l_size_component =
		    getComponentType(Game::SizeComponent::Type()).staticCast<Game::SizeComponent>();
		if (!l_size_component) {
			MMERROR("Player entity requires a size component to be present");
			return;
		}

		m_animation_component =
		    new Game::AnimationComponent("animation", *this);
		m_animation_component->pushFrame("stand-down", 72, 12);
		m_animation_component->pushFrame("stand-down", 73, 4);
		m_animation_component->pushFrame("walk-down", 74, 4);
		m_animation_component->pushFrame("walk-down", 75, 4);
		m_animation_component->setFrameRate("walk-down", 16);
		m_animation_component->pushFrame("stand-left",  76, 12);
		m_animation_component->pushFrame("stand-left",  77, 4);
		m_animation_component->pushFrame("walk-left", 78, 4);
		m_animation_component->pushFrame("walk-left", 79, 4);
		m_animation_component->setFrameRate("walk-left", 16);
		m_animation_component->pushFrame("stand-right", 72, 12);
		m_animation_component->pushFrame("stand-right", 73, 4);
		m_animation_component->pushFrame("walk-right", 74, 4);
		m_animation_component->pushFrame("walk-right", 75, 4);
		m_animation_component->setFrameRate("walk-right", 16);
		m_animation_component->pushFrame("stand-up",  76, 12);
		m_animation_component->pushFrame("stand-up",  77, 4);
		m_animation_component->pushFrame("walk-up", 80, 4);
		m_animation_component->pushFrame("walk-up", 81, 4);
		m_animation_component->setFrameRate("walk-up", 16);
		pushComponent(m_animation_component.staticCast<Game::IComponent>());

		/* input component */
		m_input_component = new InputComponent("input", *this);
		pushComponent(m_input_component.staticCast<Game::IComponent>());

		/* collider component */
		Game::SharedBounceColliderComponent l_ccomponent =
		    new Game::BounceColliderComponent("player", *this);
		pushComponent(l_ccomponent.staticCast<Game::IComponent>());

		/* movement component */
		Game::SharedMovementComponent l_mcomponent = new Game::MovementComponent("movement", *this);
		pushComponent(l_mcomponent.staticCast<Game::IComponent>());

		m_direction = -1;

		m_init = true;
	} else {
		/* make camera follow player */
		Game::SharedPositionComponent l_pos_component =
		    getComponentType(Game::PositionComponent::Type()).staticCast<Game::PositionComponent>();
		if (l_pos_component)
			Graphics::Camera::SetPosition(l_pos_component->position());

		/* update animation */
		switch(m_input_component->direction()) {
		case InputComponent::ICDDown:
			if (m_direction == InputComponent::ICDDown
			 && m_in_motion == m_input_component->inMotion())
				break;

			m_in_motion = m_input_component->inMotion();
			m_direction = InputComponent::ICDDown;
			if (m_in_motion)
				m_animation_component->play("walk-down", true);
			else
				m_animation_component->play("stand-down", true);
			break;
		case InputComponent::ICDLeft:
			if (m_direction == InputComponent::ICDLeft
			 && m_in_motion == m_input_component->inMotion())
				break;

			m_in_motion = m_input_component->inMotion();
			m_direction = InputComponent::ICDLeft;
			if (m_in_motion)
				m_animation_component->play("walk-left", true);
			else
				m_animation_component->play("stand-left", true);
			break;
		case InputComponent::ICDRight:
			if (m_direction == InputComponent::ICDRight
			 && m_in_motion == m_input_component->inMotion())
				break;

			m_in_motion = m_input_component->inMotion();
			m_direction = InputComponent::ICDRight;
			if (m_in_motion)
				m_animation_component->play("walk-right", true);
			else
				m_animation_component->play("stand-right", true);
			break;
		case InputComponent::ICDUp:
			if (m_direction == InputComponent::ICDUp
			 && m_in_motion == m_input_component->inMotion())
				break;

			m_in_motion = m_input_component->inMotion();
			m_direction = InputComponent::ICDUp;
			if (m_in_motion)
				m_animation_component->play("walk-up", true);
			else
				m_animation_component->play("stand-up", true);
			break;
		}
	}

	Game::EntityBase::update(d);
}

const Core::Type &
PlayerEntity::Type(void)
{
	static const Core::Type s_type("PlayerEntity");
	return(s_type);
}

