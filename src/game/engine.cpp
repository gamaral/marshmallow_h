/*
 * Copyright (c) 2011-2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
 * All rights reserved.
 *
 * This file is part of Marshmallow Game Engine.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the project as a whole.
 */

#include "game/engine.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/logger.h"
#include "core/platform.h"
#include "core/type.h"

#include "event/eventmanager.h"
#include "event/quitevent.h"
#include "event/renderevent.h"
#include "event/updateevent.h"

#include "audio/backend_p.h"

#include "graphics/backend_p.h"
#include "graphics/display.h"
#include "graphics/painter_p.h"

#include "input/joystick_p.h"
#include "input/keyboard_p.h"

#include "game/backend_p.h"
#include "game/factory.h"
#include "game/scenemanager.h"

#include <cstdio>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */
namespace { /************************************ Game::<anonymous> Namespace */

void
GetBackendOverrides(Graphics::Display &display)
{
	const char *l_env;
	if ((l_env = getenv("MM_WIDTH")))
		sscanf(l_env, "%hu", &display.width);

	if ((l_env = getenv("MM_HEIGHT")))
		sscanf(l_env, "%hu", &display.height);

	if ((l_env = getenv("MM_SIZE")))
		sscanf(l_env, "%hux%hu", &display.width, &display.height);

	if ((l_env = getenv("MM_DEPTH"))) {
		uint16_t l_tmp = 0;
		sscanf(l_env, "%hu", &l_tmp);
		display.depth = static_cast<uint8_t>(l_tmp);
	}

	if ((l_env = getenv("MM_FULLSCREEN")))
		display.fullscreen = (l_env[0] == '1');

	if ((l_env = getenv("MM_VSYNC"))) {
		uint16_t l_tmp = 0;
		sscanf(l_env, "%hu", &l_tmp);
		display.vsync = static_cast<uint8_t>(l_tmp);
	}
}

} /********************************************** Game::<anonymous> Namespace */

struct Engine::Private
{
	Private(int fps_, int sleep_)
	    : event_manager(0)
	    , factory(0)
	    , scene_manager(0)
	    , delta_time(0)
	    , exit_code(0)
	    , fps(fps_)
	    , frame_rate(0)
	    , sleep(sleep_)
	    , running(false)
	    , suspended(false)
	    , valid(false)
	{}

	Event::EventManager *event_manager;
	Game::IFactory      *factory;
	Game::SceneManager  *scene_manager;
	MMTIME delta_time;
	int    exit_code;
	int    fps;
	int    frame_rate;
	int    sleep;
	bool   running;
	bool   suspended;
	bool   valid;

};

Engine::Engine(int fps_, int sleep)
    : PIMPL_CREATE_X(fps_, sleep)
{
	MMINFO("Marshmallow Engine Version "
	    << MARSHMALLOW_VERSION_MAJOR << "."
	    << MARSHMALLOW_VERSION_MINOR << "."
	    << MARSHMALLOW_VERSION_BUILD << "."
	    << MARSHMALLOW_VERSION_REVISION);

	Backend::SetInstance(this);
}

Engine::~Engine(void)
{
	Backend::SetInstance(0);

	PIMPL_DESTROY;
}

bool
Engine::initialize(void)
{
	using namespace Core;
	using namespace Graphics;
	using namespace Input;

	/*
	 * Initialize Backends
	 */

	Platform::Initialize();

	if (!PIMPL->event_manager)
		PIMPL->event_manager = new Event::EventManager("Engine.EventManager");
	eventManager()->connect(this, Event::QuitEvent::Type());

	Audio::Backend::Initialize();
	Graphics::Backend::Initialize();
	Input::Joystick::Initialize();
	Input::Keyboard::Initialize();

	if (!PIMPL->scene_manager)
		PIMPL->scene_manager = new SceneManager();

	if (!PIMPL->factory)
		PIMPL->factory = new Factory;

	/*
	 * Environment Overrides
	 */
	
	Graphics::Display l_display = Graphics::Backend::Display();
	GetBackendOverrides(l_display);

	/*
	 * Setup
	 */
	if (!Graphics::Backend::Setup(l_display)) {
		MMERROR("Failed to initialize engine!");
		return(false);
	}

	/* validate */
	PIMPL->valid = true;

	return(true);
}

void
Engine::finalize(void)
{
	using namespace Core;
	using namespace Graphics;
	using namespace Input;

	if (isValid())
		eventManager()->disconnect(this, Event::QuitEvent::Type());

	delete PIMPL->factory, PIMPL->factory = 0;
	delete PIMPL->scene_manager, PIMPL->scene_manager = 0;

	Joystick::Finalize();
	Keyboard::Finalize();
	Graphics::Backend::Finalize();
	Audio::Backend::Finalize();

	delete PIMPL->event_manager, PIMPL->event_manager = 0;

	Platform::Finalize();

	/* invalidate */
	PIMPL->valid = false;
}

bool
Engine::isSuspended(void) const
{
	return(PIMPL->suspended);
}

bool
Engine::isValid(void) const
{
	return(PIMPL->valid);
}

void
Engine::setEventManager(Event::EventManager *m)
{
	PIMPL->event_manager = m;
}

void
Engine::setSceneManager(Game::SceneManager *m)
{
	
	PIMPL->scene_manager = m;
}

void
Engine::setFactory(Game::IFactory *f)
{
	PIMPL->factory = f;
}

int
Engine::fps(void) const
{
	return(PIMPL->fps);
}

MMTIME
Engine::deltaTime(void) const
{
	return(PIMPL->delta_time);
}

int
Engine::frameRate(void)
{
	return(PIMPL->frame_rate);
}

int
Engine::run(void)
{
	using namespace Core;

	if (!initialize()) {
		MMERROR("Engine initialization failed");
		finalize();
		return(-1);
	}

#define MILLISECONDS_PER_SECOND 1000
	const MMTIME l_tick_target = MILLISECONDS_PER_SECOND / PIMPL->fps;
	const MMTIME l_tick_fast_target = (l_tick_target * 2) / 3;
	MMTIME l_second = 0;
	MMTIME l_tock = 0;
	MMTIME l_tick;

	/* start */
	bool l_wait  = false;
	PIMPL->valid   = true;
	PIMPL->running = true;

	tick(.0f);
	update(.0f);
	l_tick = NOW() - l_tick_target;

	/*
	 * Game Loop
	 */
	while (PIMPL->running) {
		l_tick = NOW();

#if MARSHMALLOW_DEBUG
		/* detect breakpoint */
		if (PIMPL->delta_time > MILLISECONDS_PER_SECOND) {
			MMWARNING("Abnormally long time between ticks, debugger breakpoint?");
			PIMPL->delta_time = l_tick_target;
		}
#endif

		/* update dt counters */
		l_tock   += PIMPL->delta_time;
		l_second += PIMPL->delta_time;

		/* wait if no vsync or cpu/gpu too fast */
		l_wait |= (PIMPL->delta_time <= l_tick_fast_target);

		/*
		 * Second
		 */
		if (l_second >= MILLISECONDS_PER_SECOND) {
			second();

			l_wait = (PIMPL->delta_time <= (l_tick_fast_target));
			PIMPL->frame_rate = 0;

			/* reset second */
			l_second = 0;
		}

		/*
		 * Tock
		 */
		if (l_tock >= l_tick_target || !l_wait) {
			/*
			 * Update
			 */
			update(static_cast<float>(l_tick_target) / MILLISECONDS_PER_SECOND);

			/*
			 * Render
			 */
			render();
			PIMPL->frame_rate++;

			/* reset tock */
			l_tock= 0;
		}

		/*
		 * Sleep
		 *
		 * Higher suspend interval values might cause minor choppiness
		 * but it might be worth it for sub 20% CPU usage (very battery
		 * friendly).
		 */
		if (l_wait) Platform::Sleep(Graphics::Backend::Active() ? PIMPL->sleep : l_tick_fast_target);

		/*
		 * Tick
		 */
		tick(static_cast<float>(PIMPL->delta_time) / MILLISECONDS_PER_SECOND);

		PIMPL->delta_time = NOW() - l_tick;
	}

	/*
	 * Exit
	 */

	finalize();
	return(PIMPL->exit_code);
}

void
Engine::stop(int ec)
{
	MMINFO("Engine stopped.");
	PIMPL->exit_code = ec;
	PIMPL->running = false;
}

void
Engine::suspend(void)
{
	MMINFO("Engine suspended.");
	PIMPL->suspended = true;
}

void
Engine::resume(void)
{
	MMINFO("Engine resumed.");
	PIMPL->suspended = false;
}

Event::EventManager *
Engine::eventManager(void) const
{
	return(PIMPL->event_manager);
}

Game::SceneManager *
Engine::sceneManager(void) const
{
	return(PIMPL->scene_manager);
}

Game::IFactory *
Engine::factory(void) const
{
	return(PIMPL->factory);
}

void
Engine::tick(float delta)
{
	using namespace Input;

	Audio::Backend::Tick(delta);
	Graphics::Backend::Tick(delta);
	Keyboard::Tick(delta);
	Joystick::Tick(delta);

	if (PIMPL->event_manager) PIMPL->event_manager->execute();
	else MMWARNING("No event manager!");
}

void
Engine::second(void)
{
	MMDEBUG("FPS=" << PIMPL->frame_rate);
}

void
Engine::render(void)
{
	using namespace Event;

	if (!Graphics::Backend::Active() || PIMPL->suspended)
		return;

	Graphics::Painter::Render();

	RenderEvent event;
	eventManager()->dispatch(event);

	Graphics::Backend::Finish();
}

void
Engine::update(float d)
{
	if (!Graphics::Backend::Active() || PIMPL->suspended)
		return;

	Event::UpdateEvent event(d);
	eventManager()->dispatch(event);
}

bool
Engine::handleEvent(const Event::IEvent &e)
{
	if (e.type() == Core::Type("Event::QuitEvent")) {
		const Event::QuitEvent *l_qe = static_cast<const Event::QuitEvent *>(&e);
		stop(l_qe ? l_qe->code() : 0);
		return(true);
	}
	return(false);
}

IEngine *
Engine::Instance(void)
{
	return(Backend::Instance());
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

