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

#include <core/identifier.h>
#include <core/logger.h>

#include <math/size2.h>
#include <math/vector2.h>

#include <event/debugeventlistener.h>
#include <event/eventbase.h>
#include <event/eventmanager.h>

#include <graphics/itexturedata.h>
#include <graphics/painter.h>
#include <graphics/quadmesh.h>
#include <graphics/viewport.h>

#include <game/collidercomponent.h>
#include <game/collisionscenelayer.h>
#include <game/enginebase.h>
#include <game/entity.h>
#include <game/entityscenelayer.h>
#include <game/icomponent.h>
#include <game/movementcomponent.h>
#include <game/positioncomponent.h>
#include <game/rendercomponent.h>
#include <game/scenebase.h>
#include <game/scenemanager.h>
#include <game/sizecomponent.h>

MARSHMALLOW_NAMESPACE_USE;
using namespace Core;

class DemoBounceComponent : public Game::ComponentBase
{

	Game::WeakPositionComponent m_position;
	Game::WeakMovementComponent m_movement;

public:
	DemoBounceComponent(Game::IEntity &e)
	: Game::ComponentBase("bouncer", e)
	{
	}

	VIRTUAL void update(float d)
	{
		MMUNUSED(d);

		if (!m_position)
			m_position = entity().getComponentType("Game::PositionComponent").
			    cast<Game::PositionComponent>();

		if (!m_movement)
			m_movement = entity().getComponentType("Game::MovementComponent").
			    cast<Game::MovementComponent>();

		if (m_position && m_movement) {
			MMINFO("DELTA: %f", d);
			MMINFO("Current position %f, %f",
			    m_position->position().x(), m_position->position().y());

			Math::Size2f l_vpsize = Graphics::Viewport::Size();

			Math::Point2  &pos = m_position->position();
			Math::Vector2 &dir = m_movement->velocity();
			Math::Vector2 &acc = m_movement->acceleration();

			if ((pos.x() <= -l_vpsize.width() / 2 && dir.x() < 0)
			 || (pos.x() >=  l_vpsize.width() / 2 && dir.x() > 0)) {
				dir[0] *= -1;
				acc[0] *= -1;
			}
			if ((pos.y() <= -l_vpsize.height() / 2 && dir.y() < 0)
			 || (pos.y() >=  l_vpsize.height() / 2 && dir.y() > 0)) {
				dir[1] *= -1;
				acc[1] *= -1;
			}
		}
	}

	VIRTUAL const Core::Type & type(void) const
	{
		static const Core::Type s_type("DemoBounceComponent");
		return(s_type);
	}
};

class DemoScene : public Game::SceneBase
{
	bool m_init;

public:

	DemoScene(void)
	: SceneBase("DemoScene"),
	  m_init(false)
	  {}
	
	virtual ~DemoScene(void) {};

	VIRTUAL void activate(void)
	{
		SceneBase::activate();

		if (!m_init) {
			m_init = true;

			Game::SharedCollisionSceneLayer l_collision_layer(new Game::CollisionSceneLayer("collision", *this));
			pushLayer(l_collision_layer.staticCast<Game::ISceneLayer>());

			Game::SharedEntitySceneLayer l_layer(new Game::EntitySceneLayer("main", *this));
			pushLayer(l_layer.staticCast<Game::ISceneLayer>());

			for (int i = 0; i < 4; i++) {
			Game::SharedEntity l_entity(new Game::Entity("player", *l_layer));

			Game::PositionComponent *l_pcomponent =
			    new Game::PositionComponent("position", *l_entity);
			l_pcomponent->position() = Math::Point2(
			    (rand() % static_cast<int>(Graphics::Viewport::Size().width())) -
			        Graphics::Viewport::Size().width() / 2.f,
			    (rand() % static_cast<int>(Graphics::Viewport::Size().height())) -
			        Graphics::Viewport::Size().height() / 2.f);
			l_entity->pushComponent(l_pcomponent);

			Game::MovementComponent *l_mcomponent =
			    new Game::MovementComponent("movement", *l_entity);
			l_mcomponent->velocity() = Math::Vector2(.1, .1);
			if (rand() % 2)
			    l_mcomponent->velocity()[0] *= -1;
			if (rand() % 2)
			    l_mcomponent->velocity()[1] *= -1;
			l_mcomponent->acceleration() = l_mcomponent->velocity() * 50.f;
			l_entity->pushComponent(l_mcomponent);

			DemoBounceComponent *l_bcomponent = new DemoBounceComponent(*l_entity);
			l_entity->pushComponent(l_bcomponent);

			Game::SizeComponent *l_scomponent =
			    new Game::SizeComponent("size", *l_entity);
			l_scomponent->size() = Math::Size2f(10.f, 6.f);
			l_entity->pushComponent(l_scomponent);

			Math::Rect2 l_rect(Math::Size2f(10.f, 10.f));
			Game::RenderComponent *l_rcomponent =
			    new Game::RenderComponent("render", *l_entity);
			Graphics::SharedMeshBase l_mesh(new Graphics::QuadMesh(l_rect));
			l_mesh->textureData()->load("assets/mallow.png");
			l_rcomponent->mesh() = l_mesh.staticCast<Graphics::IMesh>();
			l_entity->pushComponent(l_rcomponent);

			Game::BounceColliderComponent *l_ccomponent =
			    new Game::BounceColliderComponent("collider", *l_entity);
			l_entity->pushComponent(l_ccomponent);

			l_layer->addEntity(l_entity);
		}
		}
	}

	VIRTUAL const Core::Type & type(void) const
	{
		static const Core::Type s_type("DemoScene");
		return(s_type);
	}
};

class Demo : public Game::EngineBase
{
	int m_stop_timer;
	Event::SharedEventListener m_debugListener;

public:

	Demo(void)
	: EngineBase(60, 60, true),
	  m_stop_timer(0),
	  m_debugListener(new Event::DebugEventListener("log.txt"))
	{
	}

	VIRTUAL bool initialize(void)
	{
		EngineBase::initialize();

		eventManager()->connect(m_debugListener, "Event::KeyboardEvent");

		Game::SharedScene l_scene(new DemoScene);
		sceneManager()->pushScene(l_scene);

		return(true);
	}

	VIRTUAL void second(void)
	{
		EngineBase::second();

		if (++m_stop_timer == 30)
			stop();
	}
};

int
MMain(int argc, char *argv[])
{
	MMUNUSED(argc);
	MMUNUSED(argv);
	if (-1 == CHDIR(DEMO_CWD))
		MMFATAL("Failed to change working directory: ""%s"". ABORT!", DEMO_CWD);
	return(Demo().run());
}

