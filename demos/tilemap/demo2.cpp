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

#include <event/eventmanager.h>
#include <event/keyboardevent.h>

#include <graphics/factory.h>
#include <graphics/tileset.h>
#include <graphics/transform.h>
#include <graphics/viewport.h>

#include <game/enginebase.h>
#include <game/entity.h>
#include <game/entityscenelayer.h>
#include <game/positioncomponent.h>
#include <game/scenebase.h>
#include <game/scenemanager.h>
#include <game/textcomponent.h>

#include <cstdio>
#include <cstdlib>

#define TIMEOUT 16
#define MESSAGE "Marshmallow!\n123456790~!@#$%%^\nSecond %d"

MARSHMALLOW_NAMESPACE_USE

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

			Graphics::SharedTextureData l_texture = Graphics::Factory::CreateTextureData();
			if (!l_texture->load("assets/terminus.png"))
				MMFATAL("Failed to load tilemap asset!");

			Graphics::SharedTilesetBase l_tileset = new Graphics::Tileset;
			l_tileset->setTileSize(Math::Size2i(16, 32));
			l_tileset->setTextureData(l_texture);

			Game::SharedEntitySceneLayer l_elayer = new Game::EntitySceneLayer("entity", *this);
			Game::SharedEntity l_entity = new Game::Entity("text", *l_elayer);
			Game::SharedPositionComponent l_pcomp = new Game::PositionComponent("position", *l_entity);
			Game::SharedTextComponent l_tcomp = new Game::TextComponent("txt", *l_entity);
			l_elayer->addEntity(l_entity);
			l_tcomp->tileset() = l_tileset.staticCast<Graphics::ITileset>();
			l_tcomp->setText("Hello World!");
			l_entity->pushComponent(l_pcomp.staticCast<Game::IComponent>());
			l_entity->pushComponent(l_tcomp.staticCast<Game::IComponent>());
			pushLayer(l_elayer.staticCast<Game::ISceneLayer>());
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

	NO_ASSIGN_COPY(Demo);
public:

	Demo(void)
	: EngineBase()
	, m_stop_timer(0)
	{
	}

	VIRTUAL bool initialize(void)
	{
		if (!EngineBase::initialize())
			return(false);

		eventManager()->connect(this, Event::KeyboardEvent::Type());

		Game::SharedScene l_scene(new DemoScene);
		sceneManager()->pushScene(l_scene);

		return(true);
	}

	VIRTUAL void finalize(void)
	{
		eventManager()->disconnect(this, Event::KeyboardEvent::Type());

		EngineBase::finalize();
	}

	VIRTUAL void second(void)
	{
		EngineBase::second();

		if (++m_stop_timer == TIMEOUT)
			stop();

		Game::SharedEntitySceneLayer l_eslayer =
		    sceneManager()->activeScene()->getLayer("entity").staticCast<Game::EntitySceneLayer>();

		Game::SharedEntity l_entity = l_eslayer->getEntity("text");

		Game::SharedTextComponent l_tcomp =
		    l_entity->getComponent("txt").staticCast<Game::TextComponent>();

		char l_message[60];
		sprintf(l_message, MESSAGE, m_stop_timer);
		l_tcomp->setText(l_message);
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

		if (l_kevent.key() == Keyboard::KBK_ESCAPE ||
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

#ifdef DEMO_CWD
	const char *l_cwd = getenv("MM_DEMO_CWD");
	if (!l_cwd) l_cwd = DEMO_CWD;

	if (-1 == MMCHDIR(l_cwd))
		MMFATAL("Failed to change working directory \"" << l_cwd << "\". ABORT!");
#endif

	return(Demo().run());
}

