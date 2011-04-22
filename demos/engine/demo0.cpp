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
#include "event/debugeventlistener.h"
#include "event/eventbase.h"
#include "event/eventmanager.h"
#include "game/componentbase.h"
#include "game/engine.h"
#include "game/entitybase.h"
#include "game/scenebase.h"
#include "game/scenemanager.h"
#include "math/vector2.h"
#include "graphics/painter.h"
#include "graphics/quadgraphic.h"
#include "graphics/textureasset.h"
#include "graphics/viewport.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Core;

class DemoMoverComponent : public Game::ComponentBase
{
	Math::Vector2 m_pos;
	Math::Vector2 m_dir;

public:
	DemoMoverComponent(Game::WeakEntity e)
	: Game::ComponentBase("mover", e),
	  m_pos(0, 0),
	  m_dir(100.2, 100.2)
	{
		Math::Size2 l_vpsize = Graphics::Viewport::Size();
		m_dir.rx() += (rand() % 10) / 100.f;
		m_dir.ry() += (rand() % 10) / 100.f;
		if (rand() % 2) m_dir.rx() *= -1;
		if (rand() % 2) m_dir.ry() *= -1;
	}

	Math::Vector2 &position(void)
	{
		return(m_pos);
	}

	Math::Vector2 &direction(void)
	{
		return(m_dir);
	}

	VIRTUAL void update(TIME d)
	{
		INFO("DELTA: %f", d);
		m_pos.rx() += m_dir.rx() * d;
		m_pos.ry() += m_dir.ry() * d;
		INFO("object moved to %f, %f", m_pos.rx(), m_pos.ry());
	}

};

class DemoBounceComponent : public Game::ComponentBase
{

	Core::Weak<DemoMoverComponent> m_mover;

public:
	DemoBounceComponent(Game::WeakEntity e)
	: Game::ComponentBase("bouncer", e)
	{
	}

	VIRTUAL void update(TIME d)
	{
		UNUSED(d);

		if (!m_mover && entity())
			m_mover = entity()->component("mover").cast<DemoMoverComponent>();

		if (m_mover) {
			Math::Size2 l_vpsize = Graphics::Viewport::Size();

			Math::Vector2 &pos = m_mover->position();
			Math::Vector2 &dir = m_mover->direction();

			if ((pos.rx() <= -l_vpsize.width() / 2 && dir.rx() < 0)
			 || (pos.rx() >=  l_vpsize.width() / 2 && dir.rx() > 0))
				dir.rx() *= -0.95f;
			if ((pos.ry() <= -l_vpsize.height() / 2 && dir.ry() < 0)
			 || (pos.ry() >=  l_vpsize.height() / 2 && dir.ry() > 0))
				dir.ry() *= -0.95f;
		}
	}
};

class DemoDrawComponent : public Game::ComponentBase
{
	Core::Weak<DemoMoverComponent> m_mover;
	Graphics::SharedTextureAsset m_asset;
	Graphics::SharedGraphic m_quad;
	float m_angle;

public:

	DemoDrawComponent(Game::WeakEntity e)
	: Game::ComponentBase("draw", e),
	  m_asset(new Graphics::TextureAsset),
	  m_quad(),
	  m_angle(0)
	{
	}

	VIRTUAL void render(void)
	{
		if (!m_mover && entity())
			m_mover = entity()->component("mover").cast<DemoMoverComponent>();

		if (m_asset && !(*m_asset))
			m_asset->load("demos/engine/assets/mallow.png");

		if (m_mover && m_asset) {
			Math::Rect2 l_rect(Math::Vector2(-32, -32), Math::Size2(64, 64));
			m_quad = new Graphics::QuadGraphic(l_rect, m_mover->position());
			m_quad->setTexture(m_asset);
		}

		if (m_quad) {
			m_quad->setRotation(m_angle = fmod(m_angle + 1, 360.f));
			Graphics::Painter::Draw(*m_quad);
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
		SceneBase::activate();

		if (!m_init) {
			m_init = true;
			Game::SharedEntity l_entity(new Game::EntityBase("player"));
			Game::SharedComponent l_component1(new DemoMoverComponent(l_entity));
			Game::SharedComponent l_component2(new DemoBounceComponent(l_entity));
			Game::SharedComponent l_component3(new DemoDrawComponent(l_entity));
			l_entity->addComponent(l_component1);
			l_entity->addComponent(l_component2);
			l_entity->addComponent(l_component3);
			addEntity(l_entity);
		}
	}
};

class Demo : public Game::Engine
{
	int m_stop_timer;
	Event::SharedEventListener m_debugListener;

public:

	Demo(void)
	: Engine(),
	  m_stop_timer(0),
	  m_debugListener(new Event::DebugEventListener("log.txt"))
	{
	}

	VIRTUAL void initialize(void)
	{
		Engine::initialize();

		Graphics::Viewport::Redisplay(640, 480);

#if 0
		eventManager()->connect(m_debugListener, "Event::RenderEvent");
		eventManager()->connect(m_debugListener, "Event::UpdateEvent");
#endif
		eventManager()->connect(m_debugListener, "Event::KeyboardEvent");

		Game::SharedScene l_scene(new DemoScene);
		sceneManager()->push(l_scene);
	}

	VIRTUAL void finalize(void)
	{
		eventManager()->disconnect(m_debugListener, Event::EventBase::Type);
		
		Engine::finalize();
	}

	VIRTUAL void second(void)
	{
		Engine::second();

		if (++m_stop_timer == 30)
			stop();
	}
};

int
main(void)
{
	return(Demo().run());
}

