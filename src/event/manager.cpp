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

#include "event/manager.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "EASTL/algorithm.h"

#include "core/platform.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Core;
using namespace Event;
using namespace eastl;

Manager::Manager(const char *name)
    : m_active_queue(0)
{
	UNUSED(name);
}

Manager::~Manager(void)
{
}

bool
Manager::connect(const SharedListenerInterface &handler, const EventType &type)
{
	INFO("Connecting `%p` handler to event type `%s`", (void *)&handler, type.name());

	EventListenerMap::const_iterator l_elmapi =
	    m_elmap.find(type.uid());

	/* if this is a new type, assign a new SharedListenerInterfaceList */
	if (l_elmapi == m_elmap.end())
		m_elmap[type.uid()] = EventListenerList();

	EventListenerList &l_listeners = m_elmap[type.uid()];

	EventListenerList::const_iterator l_listenersi =
	    find(l_listeners.begin(), l_listeners.end(), handler);

	if (l_listenersi == l_listeners.end())
		l_listeners.push_back(handler);
	else {
		WARNING1("Failed! Listener already connected to this event type.");
		return(false);
	}

	INFO("Connected! Current listener count is: %d", l_listeners.size());

	return(false);
}

bool
Manager::disconnect(const SharedListenerInterface &handler, const EventType &type)
{
	UNUSED(handler);
	UNUSED(type);
	
	return(false);
}

bool
Manager::dispatch(const EventInterface &event) const
{
	bool l_handled = false;

	EventListenerMap::const_iterator l_elmapi =
	    m_elmap.find(event.type().uid());
	if (l_elmapi == m_elmap.end())
		return(false);

	const EventListenerList &l_listeners = l_elmapi->second;
	EventListenerList::const_iterator l_listenersi;
	const EventListenerList::const_iterator l_listenerse = l_listeners.end();

	for (l_listenersi = l_listeners.begin();
	    l_handled || (l_listenersi != l_listenerse);
	    ++l_listenersi)
		l_handled = (*l_listenersi)->handle(event);

	return(l_handled);
}

bool
Manager::dequeue(const SharedEventInterface &event, bool all)
{
	EventList &l_queue = m_queue[m_active_queue == 0 ? 1 : 0];

	if (all) {
		const UID type = event->type();
		const EventList::reverse_iterator l_c = l_queue.rend();
		EventList::reverse_iterator l_i;

		for (l_i = l_queue.rbegin(); l_i != l_c; ++l_i) {
			if (type == (*l_i)->type())
				l_queue.erase(l_i);
		}
	} else {
		l_queue.remove(event);
	}

	return(true);
}

bool
Manager::queue(const SharedEventInterface &event)
{
	EventList &l_queue = m_queue[m_active_queue == 0 ? 1 : 0];
	l_queue.push_back(event);
	return(true);
}

bool
Manager::render(TIME timeout)
{
	UNUSED(timeout);
	return(false);
}

bool
Manager::tick(TIME timeout)
{
	const TIME l_start_time = Platform::TimeStamp();
	bool l_abort = false;

	/* dispatch messages in active queue */
	EventList &l_queue = m_queue[m_active_queue];
	while (!l_queue.empty() && !l_abort) {
		dispatch(l_queue.front());
		l_queue.pop_front();
		l_abort = ((Platform::TimeStamp() - l_start_time) >= timeout);
	}

	if (!l_abort) {
		/* switch queues */
		m_active_queue = (m_active_queue == 0 ? 1 : 0);
	}

	return(!l_abort);
}

bool
Manager::update(TIME timeout)
{
	UNUSED(timeout);
	return(false);
}

