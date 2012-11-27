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
#include <core/shared.h>
#include <core/type.h>

#include <event/eventmanager.h>
#include <event/joystickbuttonevent.h>
#include <event/keyboardevent.h>

#include <graphics/camera.h>
#include <graphics/display.h>
#include <graphics/viewport.h>

#include <game/collisionscenelayer.h>
#include <game/enginebase.h>
#include <game/pausescenelayer.h>
#include <game/scene.h>
#include <game/scenemanager.h>

#include <extra/tmxloader.h>

#include "../common/deathevent.h"
#include "../common/doomevent.h"
#include "../common/warpevent.h"

#include "customfactory.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

MARSHMALLOW_NAMESPACE_USE
using namespace Core;

class Demo : public Game::EngineBase
{
	char m_current_level[FILENAME_MAX + 1];

	NO_ASSIGN_COPY(Demo);
public:

	Demo(void)
	    : EngineBase()
	{
		memset(m_current_level, 0, sizeof(m_current_level));
		
	}

	VIRTUAL bool initialize(void)
	{
		setFactory(new CustomFactory);

		if (!EngineBase::initialize())
			return(false);

		Graphics::Camera::SetZoom(2.f);

		eventManager()->connect(this, Common::DeathEvent::Type());
		eventManager()->connect(this, Common::WarpEvent::Type());
		eventManager()->connect(this, Event::JoystickButtonEvent::Type());
		eventManager()->connect(this, Event::KeyboardEvent::Type());

		loadLevel("start");

		return(true);
	}

	VIRTUAL void finalize(void)
	{
		eventManager()->disconnect(this, Common::DeathEvent::Type());
		eventManager()->disconnect(this, Common::WarpEvent::Type());
		eventManager()->disconnect(this, Event::JoystickButtonEvent::Type());
		eventManager()->disconnect(this, Event::KeyboardEvent::Type());

		EngineBase::finalize();
	}

	VIRTUAL bool handleEvent(const Event::IEvent &e)
	{
		using namespace Input;

		if (EngineBase::handleEvent(e))
			return(true);

		/*
		 * What happens when we get keyboard events
		 *
		 * TODO(gamaral) Bad, we need to move all this into an input
		 * translation layer.
		 */
		if (e.type() == Event::KeyboardEvent::Type()) {
			const Event::KeyboardEvent &l_kevent =
			    static_cast<const Event::KeyboardEvent &>(e);

			if (l_kevent.action() != Keyboard::KeyPressed)
				return(false);

			if (l_kevent.key() == Keyboard::KBK_PAUSE ||
			    l_kevent.key() == Keyboard::KBK_RETURN ||
			    l_kevent.key() == Keyboard::KBK_Y) {
				Game::SharedScene l_scene = sceneManager()->activeScene();
				if (l_scene->getLayer("pause"))
					l_scene->removeLayer("pause");
				else
					l_scene->pushLayer(new Game::PauseSceneLayer("pause", *l_scene));
			} else if (l_kevent.key() == Keyboard::KBK_ESCAPE ||
				   l_kevent.key() == Keyboard::KBK_P) {
				stop();
			} else if (l_kevent.key() == Keyboard::KBK_F11) {
				Graphics::Display l_display = Graphics::Viewport::Display();
				l_display.fullscreen = !l_display.fullscreen;
				Graphics::Viewport::Setup(l_display);

			/* suicide */
			} else if (l_kevent.key() == Keyboard::KBK_F1 ||
				   l_kevent.key() == Keyboard::KBK_R) {
				eventManager()->queue(new Common::DoomEvent());
				return(true);

			/* restart */
			} else if (l_kevent.key() == Keyboard::KBK_F2) {
				loadLevel("start");
				return(true);
			} else return(false);
		}

		/*
		 * What happens when we get joystick events
		 *
		 * TODO(gamaral) Bad, we need to move all this into an input
		 * translation layer.
		 */
		else if (e.type() == Event::JoystickButtonEvent::Type()) {
			const Event::JoystickButtonEvent &l_event =
			    static_cast<const Event::JoystickButtonEvent &>(e);
			if (l_event.button() == Joystick::JSB_MENU
			    && l_event.action() == Joystick::ButtonPressed) {
				Graphics::Display l_display = Graphics::Viewport::Display();
				l_display.fullscreen = !l_display.fullscreen;
				Graphics::Viewport::Setup(l_display);
			}

			else if (l_event.button() == Joystick::JSB_START
			    && l_event.action() == Joystick::ButtonPressed) {
				Game::SharedScene l_scene = sceneManager()->activeScene();
				if (l_scene->getLayer("pause"))
					l_scene->removeLayer("pause");
				else
					l_scene->pushLayer(new Game::PauseSceneLayer("pause", *l_scene));
			}

			/* suicide */
			else if (l_event.pressed(Joystick::JSB_START|Joystick::JSB_SELECT)
			    && l_event.action() == Joystick::ButtonPressed) {
				eventManager()->queue(new Common::DoomEvent());
				return(true);
			}
		}

		/*
		 * What happens when we get a death event
		 */
		else if (e.type() == Common::DeathEvent::Type()) {
			loadLevel();
			return(true);
		}

		/*
		 * What happens when we get a warp event
		 */
		else if (e.type() == Common::WarpEvent::Type()) {
			const Common::WarpEvent &l_warp_event =
			    static_cast<const Common::WarpEvent &>(e);
			loadLevel(l_warp_event.destination());
			return(true);
		}

		return(false);
	}

	void loadLevel(const std::string &level = std::string())
	{
		if (!level.empty()) {
			MMDEBUG("LOADING LEVEL: " << level);
			sprintf(m_current_level, "assets/platformer_%s.tmx", level.c_str());
		}
		else MMDEBUG("RELOAD LEVEL");

		sceneManager()->popScene();

		Game::SharedScene l_scene(new Game::Scene("main"));

		/* collision layer */
		Game::SharedCollisionSceneLayer l_collision_layer
		    (new Game::CollisionSceneLayer("collision", *l_scene));
		l_scene->pushLayer(l_collision_layer.staticCast<Game::ISceneLayer>());

		/* load tmx tilemap */
		Extra::TMXLoader m_tmxloader(*l_scene);

		if (!m_tmxloader.load(m_current_level))
			MMFATAL("Failed to load tilemap asset!");
		sceneManager()->pushScene(l_scene);
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

