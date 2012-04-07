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

#include <core/logger.h>

#include <graphics/transform.h>
#include <graphics/viewport.h>

#include <game/animationcomponent.h>
#include <game/engine.h>
#include <game/iscene.h>
#include <game/movementcomponent.h>
#include <game/positioncomponent.h>
#include <game/scenemanager.h>
#include <game/sizecomponent.h>
#include <game/tilemapscenelayer.h>

#include "inputcomponent.h"
#include "playercollidercomponent.h"

PlayerEntity::PlayerEntity(const Core::Identifier &i, Game::EntitySceneLayer &l)
    : Game::Entity(i, l)
    , m_init(false)
{
	m_moving_sky = 0;
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
		m_animation_component->pushFrame("stand-left",  106, 12);
		m_animation_component->pushFrame("stand-left",  107, 4);
		m_animation_component->pushFrame("walk-left", 108, 1);
		m_animation_component->pushFrame("walk-left", 109, 1);
		m_animation_component->setFrameRate("walk-left", 16);
		m_animation_component->pushFrame("jump-left", 108, 1);
		m_animation_component->setFrameRate("jump-left", 1);
		m_animation_component->pushFrame("stand-right", 102, 12);
		m_animation_component->pushFrame("stand-right", 103, 4);
		m_animation_component->pushFrame("walk-right", 104, 1);
		m_animation_component->pushFrame("walk-right", 105, 1);
		m_animation_component->setFrameRate("walk-right", 16);
		m_animation_component->pushFrame("jump-right", 105, 1);
		m_animation_component->setFrameRate("jump-right", 1);
		pushComponent(m_animation_component.staticCast<Game::IComponent>());

		/* movement component */
		Game::SharedMovementComponent l_movement_component =
		    new Game::MovementComponent("movement", *this);
		pushComponent(l_movement_component.staticCast<Game::IComponent>());

		/* collider component */
		m_collider_component =
		    new PlayerColliderComponent("player", *this);
		pushComponent(m_collider_component.staticCast<Game::IComponent>());

		/* input component */
		m_input_component = new InputComponent("input", *this);
		pushComponent(m_input_component.staticCast<Game::IComponent>());

		m_direction = -1;

		m_init = true;
	} else {
		/* make camera follow player */
		Game::SharedPositionComponent l_pos_component =
		    getComponentType(Game::PositionComponent::Type()).staticCast<Game::PositionComponent>();
		if (l_pos_component) {
			Graphics::Transform l_camera = Graphics::Viewport::Camera();
			Math::Point2 l_pos = l_pos_component->position();

			/* camara snap - calculate using map */

			float l_limit;
			Game::SharedTilemapSceneLayer l_platform_layer =
			    Game::Engine::Instance()->sceneManager()->activeScene()->getLayer("platform").staticCast<Game::TilemapSceneLayer>();
			const Math::Size2f &l_hrsize = l_platform_layer->virtualHalfSize();

			l_limit = l_hrsize.width() - (Graphics::Viewport::Size().width() / 2.f);
			if (l_pos.x() < -l_limit) l_pos[0] = -l_limit;
			else if (l_pos.x() > l_limit) l_pos[0] = l_limit;
			l_limit = l_hrsize.height() - (Graphics::Viewport::Size().height() / 2.f);
			if (l_pos.y() < -l_limit) l_pos[1] = -l_limit;
			else if (l_pos.y() > l_limit) l_pos[1] = l_limit;

			l_camera.setTranslation(l_pos);
			Graphics::Viewport::SetCamera(l_camera);

			/* translate background layers (parallax) */

			Game::SharedTilemapSceneLayer l_clouds =
			    Game::Engine::Instance()->sceneManager()->activeScene()->
			        getLayer("clouds").staticCast<Game::TilemapSceneLayer>();
			if ((m_moving_sky += 8 * d) > l_clouds->virtualSize().area())
				m_moving_sky = 8 * d;

			if (l_clouds)
				l_clouds->setTranslation(Math::Vector2(m_moving_sky + (l_pos.x() * 0.15f), m_moving_sky));
		}

		/* update animation */
		switch(m_input_component->direction()) {
		case InputComponent::ICDLeft:
			if (m_direction == InputComponent::ICDLeft
			 && m_in_motion == m_input_component->inMotion()
			 && m_on_platform == m_collider_component->onPlatform())
				break;

			m_direction = InputComponent::ICDLeft;
			m_in_motion = m_input_component->inMotion();
			m_on_platform = m_collider_component->onPlatform();

			if (m_on_platform) {
				if (m_in_motion)
					m_animation_component->play("walk-left", true);
				else
					m_animation_component->play("stand-left", true);
			} else m_animation_component->play("jump-left", true);
			break;
		case InputComponent::ICDRight:
			if (m_direction == InputComponent::ICDRight
			 && m_in_motion == m_input_component->inMotion()
			 && m_on_platform == m_collider_component->onPlatform())
				break;

			m_direction = InputComponent::ICDRight;
			m_in_motion = m_input_component->inMotion();
			m_on_platform = m_collider_component->onPlatform();

			if (m_on_platform) {
				if (m_in_motion)
					m_animation_component->play("walk-right", true);
				else
					m_animation_component->play("stand-right", true);
			} else m_animation_component->play("jump-right", true);
			break;
		default: break;
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

