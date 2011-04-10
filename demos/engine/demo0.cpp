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

#include "core/platform.h"
#include "math/vector2.h"
#include "event/debugeventlistener.h"
#include "event/eventbase.h"
#include "event/eventmanager.h"
#include "game/engine.h"
#include "game/scenebase.h"
#include "game/scenemanager.h"
#include "game/entitybase.h"
#include "game/componentbase.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Core;

class DemoMoverComponent : public Game::ComponentBase
{
	Math::Vector2 m_pos;
	Math::Vector2 m_dir;
public:
	DemoMoverComponent(Game::WeakEntity e)
	: Game::ComponentBase("mover", e),
	  m_pos(),
          m_dir(.01, .02) {}

	Math::Vector2 &position(void)
	{
		return(m_pos);
	}

	Math::Vector2 &direction(void)
	{
		return(m_dir);
	}

	VIRTUAL void update(void)
	{
		m_pos.rx() += m_dir.rx() * Game::Engine::Instance()->deltaTime();
		m_pos.ry() += m_dir.ry() * Game::Engine::Instance()->deltaTime();
		INFO("object moved to %f, %f", m_pos.rx(), m_pos.ry());
	}
};

class DemoBounceComponent : public Game::ComponentBase
{
	Game::WeakComponent m_mover;
public:
	DemoBounceComponent(Game::WeakEntity e)
	: Game::ComponentBase("bouncer", e)
	{
	}

	VIRTUAL void update(void)
	{
		if (!m_mover && entity())
			m_mover = entity()->component("mover");

		if (m_mover) {
			/*
			 * TODO: communication will be via messages
			 */
			DemoMoverComponent *mover = static_cast<DemoMoverComponent *>(m_mover.raw());
			Math::Vector2 &pos = mover->position();
			Math::Vector2 &dir = mover->direction();

			if ((pos.rx() < 0 && dir.rx() < 0)
			 || (pos.rx() > 20 && dir.rx() > 0))
				dir.rx() *= -.8;
			if ((pos.ry() < 0 && dir.ry() < 0)
			 || (pos.ry() > 20 && dir.ry() > 0))
				dir.ry() *= -.8;
		}
	}
};

class DemoScene : public Game::SceneBase
{
	bool m_init;
public:
	DemoScene(void)
	: SceneBase("DemoScene"),
	  m_init(false) {}

	VIRTUAL void activate(void)
	{
		if (!m_init) {
			m_init = true;
			Game::SharedEntity l_entity(new Game::EntityBase("player"));
			Game::SharedComponent l_component1(new DemoMoverComponent(l_entity));
			Game::SharedComponent l_component2(new DemoBounceComponent(l_entity));
			l_entity->addComponent(l_component1);
			l_entity->addComponent(l_component2);
			addEntity(l_entity);
		}
	}
};

class Demo : public Game::Engine
{
	int m_stop_timer;
	Event::SharedEventManager  m_event_manager;
	Game::SharedSceneManager   m_scene_manager;
	Event::SharedEventListener m_debugListener;

public:
	Demo(void)
	: Engine(),
	  m_stop_timer(0),
	  m_event_manager(new Event::EventManager("main")),
	  m_scene_manager(new Game::SceneManager(Game::SharedScene())),
	  m_debugListener(new Event::DebugEventListener("log.txt"))
	{
	}

	VIRTUAL void initialize(void)
	{
		Engine::initialize();

		setEventManager(m_event_manager);
		setSceneManager(m_scene_manager);

		eventManager()->connect(m_debugListener, Event::EventBase::Type);

		Game::SharedScene l_scene(new DemoScene);
		m_scene_manager->push(l_scene);
	}

	VIRTUAL void finalize(void)
	{
		eventManager()->disconnect(m_debugListener, Event::EventBase::Type);
		
		Engine::finalize();
	}

	VIRTUAL void second(void)
	{
		Event::SharedEvent event(new Event::EventBase("event"));
		Event::SharedEvent event2(new Event::EventBase("event2"));

		eventManager()->queue(event);
		eventManager()->queue(event2);

		/*
		 * Dequeue all on odd seconds
		 */
		eventManager()->dequeue(event, m_stop_timer % 2);

		if (++m_stop_timer == 10)
			stop();
	}
};

int
main(void)
{
	return(Demo().run());
}

