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

#include <core/logger.h>

#include <event/eventmanager.h>
#include <event/keyboardevent.h>

#include <graphics/transform.h>
#include <graphics/viewport.h>

#include <game/collisionscenelayer.h>
#include <game/enginebase.h>
#include <game/pausescenelayer.h>
#include <game/scene.h>
#include <game/scenemanager.h>

#include <extra/tmxloader.h>

#include "customfactory.h"

#include <cstdlib>

MARSHMALLOW_NAMESPACE_USE
using namespace Core;

class Demo : public Game::EngineBase
{
	NO_ASSIGN_COPY(Demo);
public:

	Demo(void)
	: EngineBase(60, 60, MARSHMALLOW_BUSYWAIT)
	{
	}

	VIRTUAL bool initialize(void)
	{
		setFactory(new CustomFactory);

		if (!EngineBase::initialize())
			return(false);

		eventManager()->connect(this, Event::KeyboardEvent::Type());

		loadLevel();

		return(true);
	}

	VIRTUAL void finalize(void)
	{
		if (isValid())
			eventManager()->disconnect(this, Event::KeyboardEvent::Type());

		EngineBase::finalize();
	}

	VIRTUAL bool handleEvent(const Event::IEvent &e)
	{
		if (EngineBase::handleEvent(e))
			return(true);

		if (e.type() != Event::KeyboardEvent::Type())
			return(false);

		const Event::KeyboardEvent &l_kevent =
		    static_cast<const Event::KeyboardEvent &>(e);

		if (l_kevent.action() != Event::KeyPressed)
			return(false);

		if (l_kevent.key() == Event::KEY_RETURN) {
			Game::SharedScene l_scene = sceneManager()->activeScene();
			if (l_scene->getLayer("pause"))
				l_scene->removeLayer("pause");
			else
				l_scene->pushLayer(new Game::PauseSceneLayer("pause", *l_scene));
		} else if (l_kevent.key() == Event::KEY_ESCAPE) {
			stop();
		} else if (l_kevent.key() == Event::KEY_F1) {
			loadLevel();
		} else return(false);

		return(true);
	}

	void loadLevel()
	{
		sceneManager()->popScene();

		Game::SharedScene l_scene(new Game::Scene("main"));

		/* collision layer */
		Game::SharedCollisionSceneLayer l_collision_layer
		    (new Game::CollisionSceneLayer("collision", *l_scene));
		l_scene->pushLayer(l_collision_layer.staticCast<Game::ISceneLayer>());

		/* load tmx tilemap */
		Extra::TMXLoader m_tmxloader(*l_scene);
		if (!m_tmxloader.load("assets/platformer.tmx"))
			MMFATAL("Failed to load tilemap asset!");
		sceneManager()->pushScene(l_scene);
	}
};

int
MMain(int argc, char *argv[])
{
	MMUNUSED(argc);
	MMUNUSED(argv);

	if (-1 == CHDIR(DEMO_CWD))
		MMFATAL("Failed to change working directory \"" << DEMO_CWD << "\". ABORT!");

	return(Demo().run());
}

