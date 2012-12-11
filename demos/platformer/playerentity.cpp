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
#include <core/platform.h>

#include <event/eventmanager.h>

#include <graphics/camera.h>
#include <graphics/viewport.h>

#include <game/animationcomponent.h>
#include <game/engine.h>
#include <game/iengine.h>
#include <game/iscene.h>
#include <game/movementcomponent.h>
#include <game/positioncomponent.h>
#include <game/scenemanager.h>
#include <game/sizecomponent.h>
#include <game/tilemapscenelayer.h>

#include "../common/deathevent.h"
#include "../common/doomevent.h"
#include "../common/warpevent.h"
#include "actorcollidercomponent.h"
#include "inputcomponent.h"

PlayerEntity::PlayerEntity(const Core::Identifier &i, Game::EntitySceneLayer &l)
    : Common::ActorEntity(i, l)
    , m_init(false)
{
	m_moving_sky_bg = m_moving_sky = 0;

	Event::EventManager::Instance()->connect(this, Common::DoomEvent::Type());

}

PlayerEntity::~PlayerEntity(void)
{
	Event::EventManager::Instance()->disconnect(this, Common::DoomEvent::Type());
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

		m_animation_component->pushFrame("stand-left",  115, 12);
		m_animation_component->pushFrame("stand-left",  116, 4);
		m_animation_component->pushFrame("stand-right", 111, 12);
		m_animation_component->pushFrame("stand-right", 112, 4);

		m_animation_component->pushFrame("walk-left", 117, 1);
		m_animation_component->pushFrame("walk-left", 118, 1);
		m_animation_component->setFrameRate("walk-left", 16);
		m_animation_component->pushFrame("walk-right", 113, 1);
		m_animation_component->pushFrame("walk-right", 114, 1);
		m_animation_component->setFrameRate("walk-right", 16);

		m_animation_component->pushFrame("jump-left", 118, 1);
		m_animation_component->setFrameRate("jump-left", 1);
		m_animation_component->pushFrame("jump-right", 113, 1);
		m_animation_component->setFrameRate("jump-right", 1);

		m_animation_component->pushFrame("dying", 126, 1);
		m_animation_component->pushFrame("dying", 127, 1);

		pushComponent(m_animation_component.staticCast<Game::IComponent>());

		/* movement component */
		m_movement_component =
		    new Game::MovementComponent("movement", *this);
		pushComponent(m_movement_component.staticCast<Game::IComponent>());

		/* collider component */
		m_collider_component =
		    new ActorColliderComponent("collider", *this);
		pushComponent(m_collider_component.staticCast<Game::IComponent>());

		/* input component */
		m_input_component = new InputComponent("input", *this);
		pushComponent(m_input_component.staticCast<Game::IComponent>());

		m_direction = -1;

		m_init = true;
	}

	/*
	 * XXX(gamaral): Most of this needs to be moved elsewhere.
	 */
	else if (m_input_component->isEnabled()) {
		/* make camera follow player */
		Game::SharedPositionComponent l_pos_component =
		    getComponentType(Game::PositionComponent::Type()).staticCast<Game::PositionComponent>();
		if (l_pos_component) {
			/* camara snap - calculate using map */

			float l_limit;
			Math::Point2 l_pos = l_pos_component->position();
			const Math::Size2f &l_zoom = Graphics::Camera::Zoom();

			Game::SharedTilemapSceneLayer l_platform_layer =
			    Game::Engine::Instance()->sceneManager()->activeScene()->getLayer("platform").staticCast<Game::TilemapSceneLayer>();
			const Math::Size2f &l_hrsize = l_platform_layer->virtualHalfSize();

			l_limit = l_hrsize.width - (Graphics::Viewport::Size().width / (2.f * l_zoom.width));
			if (l_pos.x < -l_limit) l_pos.x = -l_limit;
			else if (l_pos.x > l_limit) l_pos.x = l_limit;

			l_limit = l_hrsize.height - (Graphics::Viewport::Size().height / (2.f * l_zoom.height));
			if (l_pos.y < -l_limit) l_pos.y = -l_limit;
			else if (l_pos.y > l_limit) l_pos.y = l_limit;

			Graphics::Camera::SetPosition(l_pos);

			/* animation playback ratio */

			if (m_input_component->inMotion()) {
				const float l_lratio = m_input_component->linearRatio();
				m_animation_component->setPlaybackRatio(l_lratio < 0 ? -l_lratio : l_lratio);
			}
			else m_animation_component->setPlaybackRatio(1.f);

			/* translate background layers (parallax) */

			Game::SharedTilemapSceneLayer l_clouds =
			    Game::Engine::Instance()->sceneManager()->activeScene()->
			        getLayer("clouds").staticCast<Game::TilemapSceneLayer>();
			if (l_clouds) {
				if ((m_moving_sky += 8.f * d) > l_clouds->virtualSize().area())
					m_moving_sky = 8.f * d;
				l_clouds->setTranslation(Math::Vector2(m_moving_sky + (l_pos.x * .15f), m_moving_sky));
			}

			Game::SharedTilemapSceneLayer l_cloudbg =
			    Game::Engine::Instance()->sceneManager()->activeScene()->
			        getLayer("cloudbg").staticCast<Game::TilemapSceneLayer>();
			if (l_cloudbg) {
				if ((m_moving_sky_bg += 2.f * d) > l_cloudbg->virtualSize().area())
					m_moving_sky_bg = 2.f * d;
				l_cloudbg->setTranslation(Math::Vector2(m_moving_sky_bg + (l_pos.x * .05f), m_moving_sky_bg));
			}

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

bool
PlayerEntity::handleEvent(const Event::IEvent &e)
{
	if (e.type() == Common::DoomEvent::Type()) {
		m_animation_component->play("dying", true);

		m_collider_component->disable();
		m_input_component->disable();

		m_movement_component->velocity()[0] = 0;
		m_movement_component->velocity()[1] = 500;
		m_movement_component->acceleration()[0] = 0;
		m_movement_component->acceleration()[1] = -1000;

		/*
		 * Reload in 1500 milliseconds
		 */
		Event::EventManager::Instance()->queue(new Common::DeathEvent(NOW() + 2000));

		return(true);
	}

	else if (e.type() == Common::WarpEvent::Type()) {
		m_collider_component->disable();
		m_input_component->disable();

		m_movement_component->velocity()[0] = 0;
		m_movement_component->velocity()[1] = 0;
		m_movement_component->acceleration()[0] = 0;
		m_movement_component->acceleration()[1] = 0;

		const Common::WarpEvent &l_warp_event =
		    static_cast<const Common::WarpEvent &>(e);
		Event::EventManager::Instance()->queue(new Common::WarpEvent(l_warp_event.destination()));
		return(true);
	}

	return(false);
}

const Core::Type &
PlayerEntity::Type(void)
{
	static const Core::Type s_type("PlayerEntity");
	return(s_type);
}

