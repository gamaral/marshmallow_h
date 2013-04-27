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

#include "game/enginebase.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <tinyxml2.h>

#include "core/identifier.h"
#include "core/logger.h"
#include "core/platform.h"
#include "core/shared.h"
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

#include "game/engine_p.h"
#include "game/factory.h"
#include "game/scenemanager.h"

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

struct EngineBase::Private
{
	Event::SharedEventManager  event_manager;
	Game::SharedSceneManager   scene_manager;
	Game::SharedFactory        factory;
	MMTIME delta_time;
	int    exit_code;
	int    fps;
	int    frame_rate;
	int    sleep;
	bool   running;
	bool   suspended;
	bool   valid;

	Private(int fps_, int sleep_)
	    : delta_time(0)
	    , exit_code(0)
	    , fps(fps_)
	    , frame_rate(0)
	    , sleep(sleep_)
	    , running(false)
	    , suspended(false)
	    , valid(false) {}
};

EngineBase::EngineBase(int fps_, int sleep)
    : m_p(new Private(fps_, sleep))
{
	MMINFO("Marshmallow Engine Version "
	    << MARSHMALLOW_VERSION_MAJOR << "."
	    << MARSHMALLOW_VERSION_MINOR << "."
	    << MARSHMALLOW_VERSION_BUILD << "."
	    << MARSHMALLOW_VERSION_REVISION);

	Engine::SetInstance(this);
}

EngineBase::~EngineBase(void)
{
	Engine::SetInstance(0);

	delete m_p, m_p = 0;
}

bool
EngineBase::initialize(void)
{
	using namespace Core;
	using namespace Graphics;
	using namespace Input;

	/*
	 * Initialize Backends
	 */

	Platform::Initialize();

	if (!m_p->event_manager)
		m_p->event_manager = new Event::EventManager("EngineBase.EventManager");
	eventManager()->connect(this, Event::QuitEvent::Type());

	Audio::Backend::Initialize();
	Graphics::Backend::Initialize();
	Input::Joystick::Initialize();
	Input::Keyboard::Initialize();

	if (!m_p->scene_manager)
		m_p->scene_manager = new SceneManager();

	if (!m_p->factory)
		m_p->factory = new Factory();

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
	m_p->valid = true;

	return(true);
}

void
EngineBase::finalize(void)
{
	using namespace Core;
	using namespace Graphics;
	using namespace Input;

	if (isValid())
		eventManager()->disconnect(this, Event::QuitEvent::Type());

	m_p->factory.clear();
	m_p->scene_manager.clear();

	Joystick::Finalize();
	Keyboard::Finalize();
	Graphics::Backend::Finalize();
	Audio::Backend::Finalize();

	m_p->event_manager.clear();

	Platform::Finalize();

	/* invalidate */
	m_p->valid = false;
}

bool
EngineBase::isSuspended(void) const
{
	return(m_p->suspended);
}

bool
EngineBase::isValid(void) const
{
	return(m_p->valid);
}

void
EngineBase::setEventManager(const Event::SharedEventManager &m)
{
	m_p->event_manager = m;
}

void
EngineBase::setSceneManager(const Game::SharedSceneManager &m)
{
	
	m_p->scene_manager = m;
}

void
EngineBase::setFactory(const SharedFactory &f)
{
	m_p->factory = f;
}

int
EngineBase::fps(void) const
{
	return(m_p->fps);
}

MMTIME
EngineBase::deltaTime(void) const
{
	return(m_p->delta_time);
}

int
EngineBase::frameRate(void)
{
	return(m_p->frame_rate);
}

int
EngineBase::run(void)
{
	using namespace Core;

	if (!initialize()) {
		MMERROR("Engine initialization failed");
		finalize();
		return(-1);
	}

#define MILLISECONDS_PER_SECOND 1000
	const MMTIME l_tick_target = MILLISECONDS_PER_SECOND / m_p->fps;
	const MMTIME l_tick_fast_target = (l_tick_target * 2) / 3;
	MMTIME l_second = 0;
	MMTIME l_tock = 0;
	MMTIME l_tick;

	/* start */
	bool l_wait  = false;
	m_p->valid   = true;
	m_p->running = true;

	tick(.0f);
	update(.0f);
	l_tick = NOW() - l_tick_target;

	/*
	 * Game Loop
	 */
	while (m_p->running) {
		l_tick = NOW();

#if MARSHMALLOW_DEBUG
		/* detect breakpoint */
		if (m_p->delta_time > MILLISECONDS_PER_SECOND) {
			MMWARNING("Abnormally long time between ticks, debugger breakpoint?");
			m_p->delta_time = l_tick_target;
		}
#endif

		/* update dt counters */
		l_tock   += m_p->delta_time;
		l_second += m_p->delta_time;

		/* wait if no vsync or cpu/gpu too fast */
		l_wait |= (m_p->delta_time <= l_tick_fast_target);

		/*
		 * Second
		 */
		if (l_second >= MILLISECONDS_PER_SECOND) {
			second();

			l_wait = (m_p->delta_time <= (l_tick_fast_target));
			m_p->frame_rate = 0;

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
			m_p->frame_rate++;

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
		if (l_wait) Platform::Sleep(m_p->sleep);

		/*
		 * Tick
		 */
		tick(static_cast<float>(m_p->delta_time) / MILLISECONDS_PER_SECOND);

		m_p->delta_time = NOW() - l_tick;
	}

	/*
	 * Exit
	 */

	finalize();
	return(m_p->exit_code);
}

void
EngineBase::stop(int ec)
{
	MMINFO("EngineBase stopped.");
	m_p->exit_code = ec;
	m_p->running = false;
}

void
EngineBase::suspend(void)
{
	MMINFO("EngineBase suspended.");
	m_p->suspended = true;
}

void
EngineBase::resume(void)
{
	MMINFO("EngineBase resumed.");
	m_p->suspended = false;
}

Event::SharedEventManager
EngineBase::eventManager(void) const
{
	return(m_p->event_manager);
}

Game::SharedSceneManager
EngineBase::sceneManager(void) const
{
	return(m_p->scene_manager);
}

SharedFactory
EngineBase::factory(void) const
{
	return(m_p->factory);
}

void
EngineBase::tick(float delta)
{
	using namespace Input;

	Audio::Backend::Tick(delta);
	Graphics::Backend::Tick(delta);
	Keyboard::Tick(delta);
	Joystick::Tick(delta);

	if (m_p->event_manager) m_p->event_manager->execute();
	else MMWARNING("No event manager!");
}

void
EngineBase::second(void)
{
	MMDEBUG("FPS=" << m_p->frame_rate);
}

void
EngineBase::render(void)
{
	using namespace Event;

	if (!Graphics::Backend::Active() || m_p->suspended)
		return;

	Graphics::Painter::Render();

	RenderEvent event;
	eventManager()->dispatch(event);

	Graphics::Backend::Finish();
}

void
EngineBase::update(float d)
{
	if (!Graphics::Backend::Active() || m_p->suspended)
		return;

	Event::UpdateEvent event(d);
	eventManager()->dispatch(event);
}

bool
EngineBase::serialize(XMLElement &n) const
{
	n.SetAttribute("fps", m_p->fps);
	n.SetAttribute("sleep",  m_p->sleep);

	if (m_p->scene_manager) {
		XMLElement *l_element = n.GetDocument()->NewElement("scenes");

		if (!m_p->scene_manager->serialize(*l_element)) {
			MMWARNING("Scene Manager serialization failed");
			return(false);
		}

		n.InsertEndChild(l_element);
	}

	return(true);
}

bool
EngineBase::deserialize(XMLElement &n)
{
	/*
	 * Engine deserialization should ideally
	 * take place BEFORE it has been started.
	 */

	XMLElement *l_element;

	l_element = n.FirstChildElement("scenes");

	n.QueryIntAttribute("fps", &m_p->fps);
	n.QueryIntAttribute("sleep",  &m_p->sleep);

	if (l_element && m_p->scene_manager)
		m_p->scene_manager->deserialize(*l_element);
	else if (l_element && !m_p->scene_manager)
		return(false);
	
	return(true);
}

bool
EngineBase::handleEvent(const Event::IEvent &e)
{
	if (e.type() == Core::Type("Event::QuitEvent")) {
		const Event::QuitEvent *l_qe = static_cast<const Event::QuitEvent *>(&e);
		stop(l_qe ? l_qe->code() : 0);
		return(true);
	}
	return(false);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

