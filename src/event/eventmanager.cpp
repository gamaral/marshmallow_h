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

#include "event/eventmanager.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"
#include "event/ievent.h"
#include "event/ieventlistener.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Core;
using namespace Event;
using namespace eastl;

static bool SortSharedEvent(const SharedEvent& lhs, const SharedEvent& rhs);

EventManager * EventManager::s_instance(0);

EventManager::EventManager(const Core::Identifier &i)
    : m_elmap(),
      m_id(i),
      m_active_queue(0)
{
	if (!s_instance) s_instance = this;
}

EventManager::~EventManager(void)
{
	if (s_instance == this) s_instance = 0;
}

bool
EventManager::connect(const WeakEventListener &handler, const Core::Type &t)
{
	MMINFO("Connecting `%p` handler to event type `%s`", static_cast<const void *>(&handler), t.str().c_str());

	EventListenerMap::const_iterator l_elmapi =
	    m_elmap.find(t.uid());

	/* if this is a new type, assign a new EventListenerList */
	if (l_elmapi == m_elmap.end())
		m_elmap[t.uid()] = new EventListenerList;

	SharedEventListenerList l_listeners(m_elmap[t.uid()]);

	EventListenerList::const_iterator l_listenersi =
	    find(l_listeners->begin(), l_listeners->end(), handler);

	if (l_listenersi == l_listeners->end())
		l_listeners->push_back(handler);
	else {
		MMWARNING1("Failed! Listener already connected to this event type.");
		return(false);
	}

	MMINFO("Connected! Current listener count is: %d", l_listeners->size());

	return(true);
}

bool
EventManager::disconnect(const WeakEventListener &handler, const Core::Type &t)
{
	MMINFO("Disconnecting `%p` handler from event type `%s`", static_cast<const void *>(&handler), t.str().c_str());

	EventListenerMap::const_iterator l_elmapi =
	    m_elmap.find(t.uid());

	/* if this is a new type, assign a new EventListenerList */
	if (l_elmapi == m_elmap.end()) {
		MMWARNING1("Failed! Event type not in registry.");
		return(false);
	}

	SharedEventListenerList l_listeners(m_elmap[t.uid()]);
	l_listeners->remove(handler);
	MMINFO("Disconnected! Current listener count is: %d", l_listeners->size());

	return(true);
}

bool
EventManager::dispatch(const IEvent &event)
{
	bool l_handled = false;

	EventListenerMap::const_iterator l_elmapi =
	    m_elmap.find(event.type().uid());
	if (l_elmapi == m_elmap.end())
		return(false);

	SharedEventListenerList l_listeners(l_elmapi->second);
	EventListenerList::iterator l_listenersi;

	for (l_listenersi = l_listeners->begin();
	    !l_handled && (l_listenersi != l_listeners->end());) {
		if (*l_listenersi)
			l_handled = (*l_listenersi++)->handleEvent(event);
		else
			l_listeners->erase(l_listenersi++);
	}

	return(l_handled);
}

bool
EventManager::dequeue(const SharedEvent &event, bool all)
{
	EventList &l_queue = m_queue[m_active_queue == 0 ? 1 : 0];

	if (all) {
		const UID l_type = event->type();
		EventList::reverse_iterator l_i = l_queue.rbegin();

		while (l_i != l_queue.rend()) {
			if (l_type == (*l_i)->type())
			    l_i = l_queue.erase(l_i);
			else ++l_i;
		}
	} else {
		l_queue.remove(event);
	}

	return(true);
}

bool
EventManager::queue(const SharedEvent &event)
{
	EventList &l_queue = m_queue[m_active_queue == 0 ? 1 : 0];
	l_queue.push_back(event);
	return(true);
}

bool
EventManager::execute(TIME timeout)
{
	const TIME l_start_time = NOW();

	/* fetch and sort active queue */
	EventList &l_queue = m_queue[m_active_queue];
	l_queue.sort(SortSharedEvent);

	/* dispatch events in active queue
	 *
	 * stop only if:
	 *     we ran out of messages
	 *     we timed out
	 *     event timestamp is in the future
	 */
	SharedEvent event;
	while (!l_queue.empty()
	    && (timeout == INFINITE || timeout > (NOW() - l_start_time))
	    && (event = l_queue.front())->timeStamp() <= NOW()) {
		dispatch(*event); l_queue.pop_front();
	}

	if (l_queue.empty()) {
		/* switch active queues */
		m_active_queue = (m_active_queue == 0 ? 1 : 0);
		return(true);
	}
	return(false);
}

EventManager *
EventManager::Instance(void)
{
	return(s_instance);
}

/******************************************************************** helpers */

bool
SortSharedEvent(const SharedEvent& lhs, const SharedEvent& rhs) {
	return(lhs->priority() > rhs->priority() ||
	    lhs->timeStamp() < rhs->timeStamp());
}
