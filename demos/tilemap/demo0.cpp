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
#include <core/shared.h>

#include <event/eventmanager.h>
#include <event/keyboardevent.h>

#include <graphics/camera.h>
#include <graphics/factory.h>

#include <game/enginebase.h>
#include <game/scenebase.h>
#include <game/scenemanager.h>
#include <game/tilemapscenelayer.h>

#include <cmath>
#include <cstdlib>

#define TIMEOUT 30

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
			Graphics::SharedTextureData l_texture = Graphics::Factory::CreateTextureData();
			if (!l_texture->load("assets/sewer_tileset.png"))
				MMFATAL("Failed to load tilemap asset!");

			m_init = true;

			Graphics::SharedTilesetBase l_tileset = new Graphics::Tileset;
			l_tileset->setTileSize(Math::Size2i(32, 32));
			l_tileset->setTextureData(l_texture);

			Game::SharedTilemapSceneLayer l_tslayer;
			uint32_t *l_data;

			/* layer 1 */
			l_tslayer = new Game::TilemapSceneLayer("bottom", *this);
			l_tslayer->setSize(Math::Size2i(80, 60));
			l_tslayer->setTileSize(Math::Size2i(32, 32));
			l_tslayer->attachTileset(1, l_tileset.staticCast<Graphics::ITileset>());

			/* generate random tilemap */
			l_data = new uint32_t[l_tslayer->size().area()];
			for (int i = 0; i < l_tslayer->size().area(); ++i)
				l_data[i] = static_cast<uint32_t>(1 /*offset*/ + rand() % 72) /* 8x9 sample tileset */;
			l_tslayer->setData(l_data);

			pushLayer(l_tslayer.staticCast<Game::ISceneLayer>());

			/* layer 2 */
			l_tslayer = new Game::TilemapSceneLayer("overlay", *this);
			l_tslayer->setOpacity(0.15f);
			l_tslayer->setSize(Math::Size2i(80, 60));
			l_tslayer->setTileSize(Math::Size2i(32, 32));
			l_tslayer->attachTileset(1, l_tileset.staticCast<Graphics::ITileset>());

			/* generate random tilemap */
			l_data = new uint32_t[l_tslayer->size().area()];
			for (int i = 0; i < l_tslayer->size().area(); ++i)
				l_data[i] = static_cast<uint32_t>(rand() % 2) /* 0 = nothing, 1 = some tile */;
			l_tslayer->setData(l_data);

			pushLayer(l_tslayer.staticCast<Game::ISceneLayer>());
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

		second();

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

		Graphics::Camera::SetRotation(static_cast<float>((25 * m_stop_timer) % 360));
		Graphics::Camera::SetZoom(1.5f + cosf(static_cast<float>(m_stop_timer)));
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

