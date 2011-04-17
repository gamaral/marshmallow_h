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

Engine::Engine(float f, float u)
    : m_event_manager(),
      m_scene_manager(),
      m_event_listener(),
      m_fps(f),
      m_ups(u),
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
	Viewport::Initialize();

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
	const TIME l_mpu = static_cast<TIME>(floor(1000./m_ups)); // milliseconds per update

	TIME l_tick;
	TIME l_tick_target = MIN(l_mpu, l_mpf) / 2;

	TIME l_render = 0;
	TIME l_render_target = l_mpf;

	TIME l_update = 0;
	TIME l_update_target = l_mpu;

	TIME l_second = 0;
	TIME l_second_target = 1000;

	m_delta_time = 0;
	m_running = true;

	initialize();

	/* startup */
	tick(l_tick_target);
	update(0);
	render();

	TIME l_lasttick = Platform::TimeStamp();
	TIME l_timeout;

	/* main game loop */
	do
	{
		l_tick = Platform::TimeStamp();
		m_delta_time = l_tick - l_lasttick;

		l_timeout = l_tick_target - 1;

		l_render += m_delta_time;
		l_update += m_delta_time;
		l_second += m_delta_time;

		/* tick */
		tick(l_timeout);

		if (l_render >= l_render_target) {
			render();
			l_render += (Platform::TimeStamp() - l_tick) - l_render_target;
			++m_frame_rate;
		}

		if (l_update >= l_update_target) {
			update(l_update);
			l_update += (Platform::TimeStamp() - l_tick) - l_update_target;
		}

		if (l_second >= l_second_target) {
			if (m_frame_rate < m_fps && l_tick_target > 1) {
				l_tick_target -= 1;
				INFO("Framerate dropping! adjusting tick target (%d).",
				    static_cast<int>(l_tick_target));
			} else if (m_frame_rate > (m_fps * 1.10)) {
				l_tick_target += 2;
				INFO("Framerate accelerated! adjusting tick target (%d).",
				    static_cast<int>(l_tick_target));
			}

			second();
			l_second += (Platform::TimeStamp() - l_tick) - l_second_target;
		}

		l_lasttick = l_tick;

		Platform::Sleep(l_tick_target);
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
Engine::tick(TIME &t)
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

