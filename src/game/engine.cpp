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

#include "graphics/backend_p.h"
#include "graphics/display.h"
#include "graphics/painter_p.h"

#include "game/backend_p.h"
#include "game/factory.h"
#include "game/ienginefeature.h"
#include "game/scenemanager.h"

#include <cassert>
#include <cstdio>
#include <list>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */
namespace { /************************************ Game::<anonymous> Namespace */

static void
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

typedef std::list<IEngineFeature *> EngineFeatureList;

} /********************************************** Game::<anonymous> Namespace */

struct Engine::Private
{
	Private(Engine *i)
	    : _interface(i)
	    , event_manager(0)
	    , factory(0)
	    , scene_manager(0)
	    , delta_time(0)
	    , exit_code(0)
	    , frame_rate(0)
	    , running(false)
	    , suspended(false)
	{}

	~Private(void);

	bool
	initialize(void);

	void
	finalize(void);

	inline void
	render(void);

	inline void
	second(void);

	inline void
	update(float delta);

	inline int
	run(void);

	inline void
	addFeature(Game::IEngineFeature *feature);
	
	inline void
	removeFeature(Game::IEngineFeature *feature);
	
	inline Game::IEngineFeature *
	removeFeature(const Core::Type &type);
	
	inline Game::IEngineFeature *
	getFeature(const Core::Type &type);

	EngineFeatureList    features;
	Engine              *_interface;
	Event::EventManager *event_manager;
	Game::IFactory      *factory;
	Game::SceneManager  *scene_manager;
	MMTIME delta_time;
	int    exit_code;
	int    frame_rate;
	bool   running;
	bool   suspended;

};

Engine::Private::~Private(void)
{
	while (!features.empty()) {
		delete features.back();
		features.pop_back();
	}
}

bool
Engine::Private::initialize(void)
{
	using namespace Core;
	using namespace Graphics;

	/*
	 * Initialize Backends
	 */

	Platform::Initialize();

	if (!event_manager)
		event_manager = new Event::EventManager("Engine.EventManager");
	event_manager->connect(_interface, Event::QuitEvent::Type());

	Graphics::Backend::Initialize();

	/*
	 * Initialize engine features
	 */
	const EngineFeatureList::const_iterator l_e = features.end();
	for (EngineFeatureList::const_iterator l_i = features.begin();
	     l_i != l_e;
	     ++l_i)
		(*l_i)->initialize();


	if (!scene_manager)
		scene_manager = new SceneManager;

	if (!factory)
		factory = new Factory;

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

	return(_interface->initialize());
}

void
Engine::Private::finalize(void)
{
	using namespace Core;
	using namespace Graphics;

	_interface->finalize();

	if (event_manager)
		event_manager->disconnect(_interface, Event::QuitEvent::Type());

	delete factory, factory = 0;
	delete scene_manager, scene_manager = 0;

	/*
	 * Finalize engine features
	 */
	const EngineFeatureList::const_reverse_iterator l_e = features.rend();
	for (EngineFeatureList::const_reverse_iterator l_i = features.rbegin();
	     l_i != l_e; ++l_i)
		(*l_i)->finalize();

	Graphics::Backend::Finalize();

	delete event_manager, event_manager = 0;

	Platform::Finalize();
}

void
Engine::Private::render(void)
{
	using namespace Event;

	if (!Graphics::Backend::Active() || suspended)
		return;

	/*
	 * Prepare for rendering
	 */
	Graphics::Painter::Render();

	/*
	 * Execute subclass render
	 */
	_interface->render();

	/*
	 * Dispatch render event
	 */
	event_manager->dispatch(RenderEvent());

	/*
	 * Clean up and buffer swap
	 */
	Graphics::Backend::Finish();
}

void
Engine::Private::second(void)
{
	MMDEBUG("FPS=" << frame_rate);
	_interface->second();
}

void
Engine::Private::update(float d)
{
	Graphics::Backend::Tick(d);

	/*
	 * Tick engine features
	 */
	const EngineFeatureList::const_iterator l_e = features.end();
	for (EngineFeatureList::const_iterator l_i = features.begin();
	     l_i != l_e;
	     ++l_i)
		(*l_i)->tick(d);

	/*
	 * Process events in queue
	 */
	if (event_manager) event_manager->execute();

	/*
	 * Execute subclass update
	 */
	_interface->update(d);

	/*
	 * Dispatch update event
	 */
	event_manager->dispatch(Event::UpdateEvent(d));
}

int
Engine::Private::run(void)
{
	using namespace Core;

	if (!initialize()) {
		MMERROR("Engine initialization failed");
		finalize();
		return(-1);
	}

#define MILLISECONDS_PER_SECOND 1000
	const MMTIME l_render_target = MILLISECONDS_PER_SECOND / MARSHMALLOW_ENGINE_FRAMERATE;
	MMTIME l_delta = 0;
	MMTIME l_render = 0;
	MMTIME l_second = 0;
	MMTIME l_tick;
#ifdef MARSHMALLOW_ENGINE_FIXEDTIMESTEP
	MMTIME l_update = l_render_target / 2; /* offset by 1/2 */
#endif

	/* start */
	running = true;

	update(.0f);
	l_tick = NOW() - l_render_target;

	const Graphics::Display &l_display =
		Graphics::Backend::Display();
	MMDEBUG("VSync set to " << int(l_display.vsync));

	/*
	 * Game Loop
	 */
	while (running) {
		l_tick = NOW();

#if MARSHMALLOW_DEBUG
		/* detect breakpoint */
		if (l_delta > MILLISECONDS_PER_SECOND) {
			MMWARNING("Abnormally long time between ticks, resetting!");
			l_delta = l_render_target;
		}
#endif
		/*
		 * Second
		 */
		l_second += l_delta;
		if (l_second >= MILLISECONDS_PER_SECOND) {
			second();
			frame_rate = 0;
			l_second %= MILLISECONDS_PER_SECOND;
		}

		/*
		 * Variable TimeStep Update
		 */
#ifdef MARSHMALLOW_ENGINE_VARIABLETIMESTEP
		update(float(l_delta) / MILLISECONDS_PER_SECOND);
#endif

		/*
		 * VSync
		 */
		if (l_display.vsync > 0) {
			/*
			 * Fixed TimeStep Update
			 */
#ifdef MARSHMALLOW_ENGINE_FIXEDTIMESTEP
			update(float(l_render_target) / MILLISECONDS_PER_SECOND);
#endif

			/*
			 * Render
			 */
			render();
			frame_rate++;

			/*
			 * Sleep only when graphics backend is inactive.
			 */
			if (!Graphics::Backend::Active())
				Platform::Sleep(l_render_target);
		}

		/*
		 * Non-VSync
		 */
		else {
			/*
			 * Fixed TimeStep Update
			 */
#ifdef MARSHMALLOW_ENGINE_FIXEDTIMESTEP
			l_update += l_delta;
			if (l_update >= l_render_target) {
				update(float(l_render_target) / MILLISECONDS_PER_SECOND);
				l_update %= l_render_target;
			}
#endif

			/*
			 * Render
			 */
			l_render += l_delta;
			if (l_render >= l_render_target) {
				render();
				frame_rate++;
				l_render %= l_render_target;
			}

			/*
			 * Sleep only when graphics backend is inactive.
			 */
			Platform::Sleep((Graphics::Backend::Active() ? 1 : l_render_target));
		}

		l_delta = NOW() - l_tick;
		delta_time = l_delta;
	}

	/*
	 * Exit
	 */

	finalize();
	return(exit_code);
}

void
Engine::Private::addFeature(Game::IEngineFeature *f)
{
	assert(f && "Invalid feature!");
	features.push_back(f);
}

void
Engine::Private::removeFeature(Game::IEngineFeature *f)
{
	assert(f && "Invalid feature!");
	features.remove(f);
}

Game::IEngineFeature *
Engine::Private::removeFeature(const Core::Type &t)
{
	Game::IEngineFeature *l_feature = getFeature(t);

	if (l_feature)
		removeFeature(l_feature);

	return(l_feature);
}

Game::IEngineFeature *
Engine::Private::getFeature(const Core::Type &t)
{
	EngineFeatureList::const_iterator l_i;
	const EngineFeatureList::const_iterator l_e = features.end();
	for (l_i = features.begin();
	     l_i != l_e && (*l_i)->type() != t;
	     ++l_i) {}
	return(l_i != l_e ? *l_i : 0);
}

Engine::Engine(void)
    : PIMPL_CREATE_X(this)
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
Engine::isSuspended(void) const
{
	return(PIMPL->suspended);
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
	return(PIMPL->run());
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

bool
Engine::handleEvent(const Event::IEvent &e)
{
	if (e.type() == Event::QuitEvent::Type()) {
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

void
Engine::addFeature(Game::IEngineFeature *f)
{
	PIMPL->addFeature(f);
}

void
Engine::removeFeature(Game::IEngineFeature *f)
{
	PIMPL->removeFeature(f);
}

Game::IEngineFeature *
Engine::removeFeature(const Core::Type &t)
{
	return(PIMPL->removeFeature(t));
}

Game::IEngineFeature *
Engine::getFeature(const Core::Type &t)
{
	return(PIMPL->getFeature(t));
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

