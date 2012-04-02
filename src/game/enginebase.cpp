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

#include "game/enginebase.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"
#include "core/platform.h"
#include "core/weak.h"

#include "event/eventmanager.h"
#include "event/quitevent.h"
#include "event/renderevent.h"
#include "event/updateevent.h"

#include "graphics/viewport.h"

#include "game/factory.h"
#include "game/scenemanager.h"

#include <tinyxml2.h>

extern int iAllocations;
extern int iDeallocations;

MARSHMALLOW_NAMESPACE_USE
using namespace Core;
using namespace Event;
using namespace Graphics;
using namespace Game;

EngineBase *EngineBase::s_instance = 0;

struct EngineBase::Private
{
	Event::SharedEventManager  event_manager;
	Game::SharedSceneManager   scene_manager;
	Game::SharedFactory        factory;
	int    fps;
	int    ups;
	TIME   delta_time;
	int    exit_code;
	int    frame_rate;
	int    suspend_interval;
	bool   running;
	bool   valid;
};

EngineBase::EngineBase(int f, int u, int s)
    : m_p(new Private)
{
	MMINFO("Marshmallow Engine Version " << MARSHMALLOW_VERSION_MAJOR << "."
	                                     << MARSHMALLOW_VERSION_MINOR << "."
	                                     << MARSHMALLOW_VERSION_BUILD << "."
	                                     << MARSHMALLOW_VERSION_REVISION);

	m_p->fps = f;
	m_p->ups = u;
	m_p->delta_time = 0;
	m_p->exit_code = 0;
	m_p->frame_rate = 0;
	m_p->suspend_interval = s;
	m_p->running = false;
	m_p->valid = false;

	if (!s_instance) s_instance = this;
	else
		MMWARNING("Started a second engine!");
}

EngineBase::~EngineBase(void)
{
	if (this == s_instance) s_instance = 0;

	delete m_p;
	m_p = 0;
}

bool
EngineBase::initialize(void)
{
	Platform::Initialize();

	/* initialize viewport */
	if (!Viewport::Initialize()) {
		MMERROR("Failed to initialize engine!");
		return(false);
	}

	if (!m_p->event_manager)
		m_p->event_manager = new Event::EventManager("EngineBase.EventManager");
	if (!m_p->scene_manager)
		m_p->scene_manager = new SceneManager();
	if (!m_p->factory)
		m_p->factory = new Factory();

	/* validate */
	m_p->valid = true;

	eventManager()->connect(this, Event::QuitEvent::Type());

	return(true);
}

void
EngineBase::finalize(void)
{
	if (isValid())
		eventManager()->disconnect(this, Event::QuitEvent::Type());

	m_p->scene_manager.clear();
	m_p->event_manager.clear();

	Viewport::Finalize();
	Platform::Finalize();

	/* invalidate */
	m_p->valid = false;
}

bool
EngineBase::isValid(void) const
{
	return(m_p->valid);
}

void
EngineBase::setEventManager(const SharedEventManager &m)
{
	m_p->event_manager = m;
}

void
EngineBase::setSceneManager(const SharedSceneManager &m)
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

int
EngineBase::ups(void) const
{
	return(m_p->ups);
}

TIME
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
	if (!initialize()) {
		MMERROR("Engine initialization failed");
		finalize();
		return(-1);
	}

#define MILLISECONDS_PER_SECOND 1000
	TIME l_render = 0;
	TIME l_render_target = MILLISECONDS_PER_SECOND / m_p->fps;

	TIME l_update = 0;
	TIME l_update_target = MILLISECONDS_PER_SECOND / m_p->ups;

	TIME l_second = 0;
	TIME l_second_target = MILLISECONDS_PER_SECOND;

	TIME l_tick;
	TIME l_tick_target = MMMIN(l_render_target, l_update_target);

	m_p->valid = true;
	m_p->running = true;

	/* startup */
	l_tick = NOW() - l_tick_target;
	update(static_cast<float>(l_update_target) / MILLISECONDS_PER_SECOND);

	/* main game loop */
	do
	{
		m_p->delta_time = NOW() - l_tick;
		l_tick = NOW();

#ifdef MARSHMALLOW_DEBUG
		/* detect breakpoint */
		if (m_p->delta_time > MILLISECONDS_PER_SECOND) {
			MMWARNING("Abnormally long time between ticks, debugger breakpoint?");
			m_p->delta_time = l_tick_target;
		}
#endif

		l_render += m_p->delta_time;
		l_update += m_p->delta_time;
		l_second += m_p->delta_time;

		tick();

		if (l_update >= l_update_target) {
			update(static_cast<float>(l_update_target) / MILLISECONDS_PER_SECOND);
			l_update -= l_update_target;
			if (l_update >= l_update_target)
				MMINFO("Skipping update frame. TARGET=" << l_update_target), l_update = 0;
		}

		if (l_second >= l_second_target) {
			second();
			m_p->frame_rate = 0;
			l_second -= l_second_target;
		}

		if (l_render >= l_render_target) {
			render();
			m_p->frame_rate++;
			l_render -= l_render_target;
			if (l_render >= l_render_target)
				MMINFO("Skipping render frame. TARGET=" << l_render_target), l_render = 0;
		}

#if !MARSHMALLOW_VIEWPORT_VSYNC
		/*
		 * Suspended Wait
		 *
		 * Might cause some choppiness but it's worth it for 20% CPU
		 * usage (very battery friendly).
		 */
		if (m_p->suspend_interval > 0) {
			const TIME l_nap = (l_tick_target - (NOW() - l_tick)) / m_p->suspend_interval;
			while (l_tick_target > (NOW() - l_tick)) {
				tick();
				Platform::Sleep(l_nap);
			}
		}

		/*
		 * Busy Wait
		 *
		 * High CPU usage (80%-90%) not kind to battery life.
		 */
		else while (l_tick_target > (NOW() - l_tick)) { tick(); }
#endif
	} while (m_p->running);

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

SharedEventManager
EngineBase::eventManager(void) const
{
	return(m_p->event_manager);
}

SharedSceneManager
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
EngineBase::tick(void)
{
	Viewport::Tick();
	if (m_p->event_manager) m_p->event_manager->execute();
	else MMWARNING("No event manager!");
}

void
EngineBase::second(void)
{
	MMINFO("FPS=" << m_p->frame_rate);
	m_p->frame_rate = 0;
}

void
EngineBase::render(void)
{
	Event::RenderEvent event;
	eventManager()->dispatch(event);

	Graphics::Viewport::SwapBuffer();
}

void
EngineBase::update(float d)
{
	Event::UpdateEvent event(d);
	eventManager()->dispatch(event);
}

bool
EngineBase::serialize(XMLElement &n) const
{
	n.SetAttribute("fps", m_p->fps);
	n.SetAttribute("ups", m_p->ups);
	n.SetAttribute("si",  m_p->suspend_interval);

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
	n.QueryIntAttribute("ups", &m_p->ups);
	n.QueryIntAttribute("si", &m_p->suspend_interval);

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

EngineBase *
EngineBase::Instance(void)
{
	return(s_instance);
}
