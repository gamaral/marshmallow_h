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

#include "game/engine.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/irenderable.h"
#include "core/iupdateable.h"
#include "core/platform.h"
#include "event/eventmanager.h"
#include "event/renderevent.h"
#include "event/rendereventlistener.h"
#include "event/updateevent.h"
#include "event/updateeventlistener.h"
#include "game/engineeventlistener.h"
#include "game/iscene.h"
#include "game/scenemanager.h"
#include "graphics/viewport.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Core;
using namespace Event;
using namespace Graphics;
using namespace Game;

Engine *Engine::s_instance = 0;

Engine::Engine(float f)
    : m_event_manager(),
      m_scene_manager(),
      m_event_listener(),
      m_fps(f),
      m_delta_time(0),
      m_exit_code(0),
      m_frame_rate(0),
      m_running(false)
{
	if (!s_instance)
		s_instance = this;
	else
		WARNING1("Started a second engine!");
}

Engine::~Engine(void)
{
	if (this == s_instance)
		s_instance = 0;
}

void
Engine::setup(void)
{
	m_event_listener = new EngineEventListener("Engine.EngineEventListener");

	if (!m_event_manager)
		m_event_manager = new Event::EventManager("Engine.EventManager");
	if (!m_scene_manager)
		m_scene_manager = new SceneManager();
}

void
Engine::initialize(void)
{
	Platform::Initialize();

	if (!Viewport::Initialize())
		FATAL1("Failed to initialize engine!");

	eventManager()->connect(m_event_listener, "Event::QuitEvent");
}

void
Engine::finalize(void)
{
	m_scene_manager.clear();
	m_event_manager.clear();

	Viewport::Finalize();
	Platform::Finalize();
}

int
Engine::run(void)
{
	setup();

	const TIME l_mpf = static_cast<TIME>(floor(1000./m_fps)); // milliseconds per frame

	TIME l_tick;
	TIME l_tick_target = l_mpf / 4;
	TIME l_tick_sleep = l_tick_target;

	TIME l_frame = 0;
	TIME l_frame_target = l_mpf;

	TIME l_second = 0;
	TIME l_second_target = 1000;

	m_delta_time = 0;
	m_running = true;

	initialize();

	/* startup */
	tick(l_tick_target);
	update(0);
	render();

	TIME l_lasttick = NOW();

	/* main game loop */
	do
	{
		l_tick = NOW();
		m_delta_time = l_tick - l_lasttick;

		l_frame += m_delta_time;
		l_second += m_delta_time;
		l_tick_sleep -= m_delta_time;
		l_tick_sleep += l_tick_target;
		INFO("XXX: TICK SLEEP %d", l_tick_sleep);

		if (l_frame >= l_frame_target) {
			render();
			update(l_frame);
			l_frame -= l_frame_target;
			if (l_frame > l_frame_target) {
				INFO1("Skipping frame.");
				l_frame = 0;
			}
		}

		if (l_second >= l_second_target) {
			second();
			l_second -= l_second_target;
		}

		tick(l_tick_target - (NOW() - l_tick));
		l_lasttick = NOW();

		Platform::Sleep(l_tick_sleep);
	} while (m_running);

	finalize();

	return(m_exit_code);
}

void
Engine::stop(int ec)
{
	INFO1("Engine stopped");
	m_exit_code = ec;
	m_running = false;
}

SharedEventManager
Engine::eventManager(void) const
{
	return(m_event_manager);
}

SharedSceneManager
Engine::sceneManager(void) const
{
	return(m_scene_manager);
}

void
Engine::setEventManager(SharedEventManager &m)
{
	m_event_manager = m;
}

void
Engine::setSceneManager(SharedSceneManager &m)
{
	
	m_scene_manager = m;
}

void
Engine::render(void)
{
	Event::RenderEvent event;
	eventManager()->dispatch(event);
	INFO1("Render event dispatched!");

	Graphics::Viewport::SwapBuffer();
}

void
Engine::tick(TIME t)
{
	TIMEOUT_INIT;
	Viewport::Tick(TIMEOUT_DEC(t));
	if (m_event_manager) m_event_manager->execute(TIMEOUT_DEC(t));
	else WARNING1("No event manager!");
}

void
Engine::update(TIME d)
{
	Event::UpdateEvent event(d);
	eventManager()->dispatch(event);
	INFO1("Update event dispatched!");
}

void
Engine::second(void)
{
	INFO("FPS %d!", m_frame_rate);
	m_frame_rate = 0;
}

