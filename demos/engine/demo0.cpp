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

#include <core/identifier.h>
#include <core/logger.h>
#include <core/weak.h>

#include <event/debugeventlistener.h>
#include <event/eventmanager.h>
#include <event/keyboardevent.h>

#include <graphics/itexturedata.h>
#include <graphics/quadmesh.h>
#include <graphics/viewport.h>

#include <game/collisionscenelayer.h>
#include <game/enginebase.h>
#include <game/entity.h>
#include <game/entityscenelayer.h>
#include <game/movementcomponent.h>
#include <game/pausescenelayer.h>
#include <game/positioncomponent.h>
#include <game/rendercomponent.h>
#include <game/scenebase.h>
#include <game/scenemanager.h>
#include <game/sizecomponent.h>

#include <cstdlib>

MARSHMALLOW_NAMESPACE_USE
using namespace Core;

class DemoBounceComponent : public Game::ComponentBase
{
	Game::WeakPositionComponent m_position;
	Game::WeakMovementComponent m_movement;

	NO_ASSIGN_COPY(DemoBounceComponent);
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
			MMVERBOSE("Current position (" << m_position->position().x << ", " << m_position->position().y << ").");

			Math::Size2f l_vpsize = Graphics::Viewport::Size();

			Math::Point2  &pos = m_position->position();
			Math::Vector2 &dir = m_movement->velocity();
			Math::Vector2 &acc = m_movement->acceleration();

			if ((pos.x <= -l_vpsize.width / 2 && dir.x < 0)
			 || (pos.x >=  l_vpsize.width / 2 && dir.x > 0)) {
				dir.x *= -1;
				acc.x *= -1;
			}
			if ((pos.y <= -l_vpsize.height / 2 && dir.y < 0)
			 || (pos.y >=  l_vpsize.height / 2 && dir.y > 0)) {
				dir.y *= -1;
				acc.y *= -1;
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

	NO_ASSIGN_COPY(DemoScene);
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

			srand(static_cast<unsigned int>(time(0)));

			for (int i = 0; i < 4; i++) {
			Game::SharedEntity l_entity(new Game::Entity("player", *l_layer));

			const float l_rand = static_cast<float>(rand()) /
			    static_cast<float>(RAND_MAX);

			Game::PositionComponent *l_pcomponent =
			    new Game::PositionComponent("position", *l_entity);
			l_pcomponent->position() = Math::Point2(
			    (l_rand * Graphics::Viewport::Size().width) -
			        (Graphics::Viewport::Size().width / 2.f),
			    (l_rand * Graphics::Viewport::Size().height) -
			        (Graphics::Viewport::Size().height / 2.f));
			l_entity->pushComponent(l_pcomponent);

			Game::MovementComponent *l_mcomponent =
			    new Game::MovementComponent("movement", *l_entity);
			l_mcomponent->velocity() = Math::Vector2(.1f, .1f);
			if (rand() % 2)
			    l_mcomponent->velocity().x *= -1;
			if (rand() % 2)
			    l_mcomponent->velocity().y *= -1;
			l_mcomponent->acceleration() = l_mcomponent->velocity() * 200.f;
			l_entity->pushComponent(l_mcomponent);

			DemoBounceComponent *l_bcomponent = new DemoBounceComponent(*l_entity);
			l_entity->pushComponent(l_bcomponent);

			Game::SizeComponent *l_scomponent =
			    new Game::SizeComponent("size", *l_entity);
			l_scomponent->size() = Math::Size2f(50.f, 50.f);
			l_entity->pushComponent(l_scomponent);

			Game::RenderComponent *l_rcomponent =
			    new Game::RenderComponent("render", *l_entity);
			Graphics::SharedMeshBase l_mesh(new Graphics::QuadMesh(64.f, 64.f));
			l_mesh->setColor(Graphics::Color::White());
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
	Event::SharedEventListener m_debugListener;

	NO_ASSIGN_COPY(Demo);
public:

	Demo(void)
	: EngineBase()
	, m_debugListener(new Event::DebugEventListener("log.txt"))
	{
	}

	virtual ~Demo(void)
	{
	}

	VIRTUAL bool initialize(void)
	{
		if (!EngineBase::initialize())
			return(false);

		eventManager()->connect(this, Event::KeyboardEvent::Type());
		eventManager()->connect(&*m_debugListener, Event::KeyboardEvent::Type());

		Game::SharedScene l_scene(new DemoScene);
		sceneManager()->pushScene(l_scene);

		return(true);
	}

	VIRTUAL void finalize(void)
	{
		eventManager()->disconnect(&*m_debugListener, Event::KeyboardEvent::Type());
		eventManager()->disconnect(this, Event::KeyboardEvent::Type());

		EngineBase::finalize();
	}

	VIRTUAL bool handleEvent(const Event::IEvent &e)
	{
		using namespace Input;

		if (EngineBase::handleEvent(e))
			return(true);

		if (e.type() != Event::KeyboardEvent::Type())
			return(false);

		const Event::KeyboardEvent &l_kevent =
		    static_cast<const Event::KeyboardEvent &>(e);

		if (l_kevent.action() != Keyboard::KeyPressed)
			return(false);

		if (l_kevent.key() == Keyboard::KBK_RETURN ||
                    l_kevent.key() == Keyboard::KBK_Y) {
			Game::SharedScene l_scene = sceneManager()->activeScene();
			if (l_scene->getLayer("pause"))
				l_scene->removeLayer("pause");
			else
				l_scene->pushLayer(new Game::PauseSceneLayer("pause", *l_scene));
		} else if (l_kevent.key() == Keyboard::KBK_ESCAPE ||
                           l_kevent.key() == Keyboard::KBK_P) {
			stop();
		} else return(false);

		return(true);
	}

};

int
MMain(int argc, char *argv[])
{
	MMUNUSED(argc);
	MMUNUSED(argv);

	return(Demo().run());
}

