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

#include "game/enginebase.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"
#include "event/eventmanager.h"
#include "event/renderevent.h"
#include "event/updateevent.h"
#include "graphics/viewport.h"
#include "game/engineeventlistener.h"
#include "game/factory.h"
#include "game/scenemanager.h"

extern int iAllocations;
extern int iDeallocations;

MARSHMALLOW_NAMESPACE_USE;
using namespace Core;
using namespace Event;
using namespace Graphics;
using namespace Game;

EngineBase *EngineBase::s_instance = 0;

EngineBase::EngineBase(float f, float u, bool s)
    : m_event_manager(),
      m_scene_manager(),
      m_event_listener(),
      m_factory(),
      m_fps(f),
      m_ups(u),
      m_delta_time(0),
      m_exit_code(0),
      m_frame_rate(0),
      m_suspendable(s),
      m_running(false)
{
	if (!s_instance)
		s_instance = this;
	else
		WARNING1("Started a second engine!");
}

EngineBase::~EngineBase(void)
{
	if (this == s_instance)
		s_instance = 0;
}

bool
EngineBase::initialize(void)
{
	Platform::Initialize();

	if (!Viewport::Initialize()) {
		ERROR1("Failed to initialize engine!");
		return(false);
	}

	m_event_listener = new EngineEventListener(*this);

	if (!m_event_manager)
		m_event_manager = new Event::EventManager("EngineBase.EventManager");
	if (!m_scene_manager)
		m_scene_manager = new SceneManager();
	if (!m_factory)
		m_factory = new Factory();

	eventManager()->connect(m_event_listener, "Event::QuitEvent");

	return(true);
}

void
EngineBase::finalize(void)
{
	m_scene_manager.clear();
	m_event_manager.clear();

	Viewport::Finalize();
	Platform::Finalize();
}

SharedEventManager
EngineBase::eventManager(void) const
{
	return(m_event_manager);
}

SharedSceneManager
EngineBase::sceneManager(void) const
{
	return(m_scene_manager);
}

SharedFactory
EngineBase::factory(void) const
{
	return(m_factory);
}

void
EngineBase::setEventManager(const SharedEventManager &m)
{
	m_event_manager = m;
}

void
EngineBase::setSceneManager(const SharedSceneManager &m)
{
	
	m_scene_manager = m;
}

void
EngineBase::setFactory(const SharedFactory &f)
{
	m_factory = f;
}

int
EngineBase::run(void)
{
	if (!initialize()) {
		ERROR1("Engine initialization failed");
		finalize();
		return(-1);
	}

	TIME l_render = 0;
#define MILLISECONDS_PER_SECOND static_cast<TIME>(1000.0)
	TIME l_render_target = MILLISECONDS_PER_SECOND / m_fps;

	TIME l_update = 0;
	TIME l_update_target = MILLISECONDS_PER_SECOND / m_ups;

	TIME l_second = 0;
	TIME l_second_target = MILLISECONDS_PER_SECOND;

	TIME l_tick;
	TIME l_tick_target = MIN(l_render_target, l_update_target) / static_cast<TIME>(3.0);

	m_delta_time = 0;
	m_running = true;

	/* startup */
	tick(l_tick_target);
	update(0);
	render();

	TIME l_lasttick = NOW();

	/* main game loop */
	do
	{
		l_tick = NOW();

		m_delta_time = (l_tick - l_lasttick);
		l_render += m_delta_time;
		l_update += m_delta_time;
		l_second += m_delta_time;

		if (l_render >= l_render_target) {
			render();
			m_frame_rate++;
			l_render -= l_render_target;
			if (l_render > l_render_target / 2)
				INFO1("Skipping render frame."), l_render = 0;
		}

		if (l_update >= l_update_target) {
			update(l_update / MILLISECONDS_PER_SECOND);
			l_update -= l_update_target;
			if (l_update > l_update_target / 2)
				INFO1("Skipping update frame."), l_update = 0;
		}

		if (l_second >= l_second_target) {
			second();
			m_frame_rate = 0;
			l_second -= l_second_target;
		}

		tick(l_tick_target - (NOW() - l_tick));

		if (m_suspendable)
			Platform::Sleep(l_tick_target - (NOW() - l_tick));

		l_lasttick = l_tick;
	} while (m_running);

	finalize();

	return(m_exit_code);
}

void
EngineBase::stop(int ec)
{
	INFO1("EngineBase stopped");
	m_exit_code = ec;
	m_running = false;
}

void
EngineBase::tick(TIME t)
{
	TIMEOUT_INIT;
	Viewport::Tick(TIMEOUT_DEC(t));
	if (m_event_manager) m_event_manager->execute(TIMEOUT_DEC(t));
	else WARNING1("No event manager!");
}

void
EngineBase::second(void)
{
	WARNING("FPS %d!", m_frame_rate);
	m_frame_rate = 0;
}

void
EngineBase::render(void)
{
	Event::RenderEvent event;
	eventManager()->dispatch(event);

	Graphics::Viewport::SwapBuffer();
}

void
EngineBase::update(TIME d)
{
	Event::UpdateEvent event(d);
	eventManager()->dispatch(event);
}

bool
EngineBase::serialize(TinyXML::TiXmlElement &n) const
{
	n.SetDoubleAttribute("fps", m_fps);
	n.SetDoubleAttribute("ups", m_ups);
	n.SetAttribute("suspendable", m_suspendable ? "t" : "f");

	if (m_scene_manager) {
		TinyXML::TiXmlElement l_element("scenes");

		if (!m_scene_manager->serialize(l_element)) {
			WARNING1("Scene Manager serialization failed");
			return(false);
		}

		n.InsertEndChild(l_element);
	}

	return(true);
}

bool
EngineBase::deserialize(TinyXML::TiXmlElement &n)
{
	/*
	 * Engine deserialization should ideally
	 * take place BEFORE it has been started.
	 */

	TinyXML::TiXmlElement *l_element;

	l_element = n.FirstChildElement("scenes");

	n.QueryFloatAttribute("fps", &m_fps);
	n.QueryFloatAttribute("ups", &m_ups);

	const char *l_suspendable = n.Attribute("suspendable");
	m_suspendable = (l_suspendable &&
	                (l_suspendable[0] == 't' || l_suspendable[0] == 'T'));

	if (l_element && m_scene_manager)
		m_scene_manager->deserialize(*l_element);
	else if (l_element && !m_scene_manager)
		return(false);
	
	return(true);
}

