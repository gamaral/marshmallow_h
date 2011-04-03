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

#include "core/engine.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/platform.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Core;

Engine *Engine::s_instance = 0;

Engine::Engine(float f, float u)
    : m_manager("main"),
      m_fps(f),
      m_ups(u),
      m_frame_rate(0),
      m_running(false)
{
	if (!s_instance)
		s_instance = this;
	else
		WARNING1("Warning: Started a second engine!");
}

Engine::~Engine(void)
{
	if (this == s_instance)
		s_instance = 0;
}

void
Engine::initialize(void)
{
	Platform::Initialize();

	preTick();
	tick();
	postTick();

	preUpdate();
	update();
	postUpdate();

	preRender();
	render();
	postRender();
}

void
Engine::finalize(void)
{
	Platform::Finalize();
}

int
Engine::run(void)
{
	/*************************************************** hybrid-busy-wait */
	/*
	 * XXX: I got this idea a while back, worth try I always say 
	 */
	TIME l_tick;
	TIME l_lasttick = Platform::TimeStamp();
	TIME l_tick_target = (TIME)(floor(1000/m_fps)) * 500;

	TIME l_render = 0;
	TIME l_render_target = (TIME)(floor(1000/m_fps));

	TIME l_update = 0;
	TIME l_update_target = (TIME)(floor(1000/m_ups));

	TIME l_second = 0;
	TIME l_second_target = 1000;

	m_delta_time = 0;

	initialize();

	m_running = true;

	/* main game loop */
	do
	{
		l_tick = Platform::TimeStamp();
		m_delta_time = l_tick - l_lasttick;

		l_render += m_delta_time;
		l_update += m_delta_time;
		l_second += m_delta_time;

		/* tick */
		preTick();
		tick();
		postTick();

		if (l_render >= l_render_target) {
			/* render */
			preRender();
			render();
			postRender();

			l_render += (Platform::TimeStamp() - l_tick) - l_render_target;
			++m_frame_rate;
		}

		if (l_update >= l_update_target) {
			/* update */
			preUpdate();
			update();
			postUpdate();

			l_update += (Platform::TimeStamp() - l_tick) - l_update_target;
		}

		if (l_second >= l_second_target) {
			if (m_frame_rate < m_fps && l_tick_target > 1000) {
				l_tick_target -= 1000;
				INFO("Framerate dropping! adjusting tick target (%d).", (int)l_tick_target);
			} else if (m_frame_rate > m_fps * 1.10) {
				l_tick_target += 250;
				INFO("Framerate accelerated! adjusting tick target (%d).", (int)l_tick_target);
			}

			/* second */
			preSecond();
			second();
			postSecond();

			l_second += (Platform::TimeStamp() - l_tick) - l_second_target;
		}

		l_lasttick = l_tick;

		usleep(l_tick_target); //XXX: move to platform
	} while (m_running);

	finalize();

	return(m_exit_code);
}

void
Engine::preRender(void)
{
	Platform::PreRender();
}

void
Engine::postRender(void)
{
	Platform::PostRender();
}

void
Engine::preSecond(void)
{
	Platform::PreSecond();
}

void
Engine::postSecond(void)
{
	INFO("FPS %d!", m_frame_rate);
	m_frame_rate = 0;

	Platform::PostSecond();
}

void
Engine::preTick(TIME timeout)
{
	Platform::PreTick(timeout);
}

void
Engine::postTick(TIME timeout)
{
	Platform::PostTick(timeout);
}

void
Engine::preUpdate(TIME timeout)
{
	Platform::PreUpdate(timeout);
}

void
Engine::postUpdate(TIME timeout)
{
	Platform::PostUpdate(timeout);
}

