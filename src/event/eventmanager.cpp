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

#include "event/eventmanager.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <algorithm>
#include <list>
#include <map>

#include "core/identifier.h"
#include "core/logger.h"
#include "core/platform.h"
#include "core/shared.h"
#include "core/weak.h"

#include "event/ievent.h"
#include "event/ieventlistener.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace { /******************************************** Anonymous Namespace */

	using namespace Event;

	EventManager *s_instance(0);

	bool
	SortSharedEvent(const SharedEvent& lhs, const SharedEvent& rhs) {
		return(lhs->priority() > rhs->priority()
		   || lhs->timeStamp() < rhs->timeStamp());
	}

	typedef std::list<IEventListener *> EventListenerList;
	typedef Core::Shared<EventListenerList> SharedEventListenerList;
	typedef std::map<MMUID, SharedEventListenerList> EventListenerMap;
	typedef std::list<SharedEvent> EventList;

} /****************************************************** Anonymous Namespace */

namespace Event { /****************************************** Event Namespace */

struct EventManager::Private
{
	EventListenerMap elmap;
	EventList queue[2];
	Core::Identifier id;
	uint8_t active_queue;
	Core::Type dispatch_type;
	bool dispatch_invalid;
};

EventManager::EventManager(const Core::Identifier &i)
    : m_p(new Private)
{
	m_p->id = i;
	m_p->active_queue = 0;
	m_p->dispatch_type = Core::Type::Null;
	m_p->dispatch_invalid = true;

	if (!s_instance) s_instance = this;
}

EventManager::~EventManager(void)
{
	if (s_instance == this) s_instance = 0;
	delete m_p, m_p = 0;
}

const Core::Identifier &
EventManager::id(void) const
{
	return(m_p->id);
}

bool
EventManager::connect(IEventListener *handler, const Core::Type &t)
{
	MMINFO("Connecting `" << &handler << "` handler to event type `" << t.str() << "`.");

	EventListenerMap::const_iterator l_elmapi =
	    m_p->elmap.find(t.uid());

	/* if this is a new type, assign a new EventListenerList */
	if (l_elmapi == m_p->elmap.end())
		m_p->elmap[t.uid()] = new EventListenerList;

	SharedEventListenerList l_listeners(m_p->elmap[t.uid()]);

	EventListenerList::const_iterator l_listenersi =
	    std::find(l_listeners->begin(), l_listeners->end(), handler);

	if (l_listenersi == l_listeners->end())
		l_listeners->push_back(handler);
	else {
		MMWARNING("Failed! Listener already connected to this event type.");
		return(false);
	}

	MMINFO("Connected! Current listener count is: " << l_listeners->size() << ".");

	/* invalidate dispatch if needed */
	m_p->dispatch_invalid |= (m_p->dispatch_type == t);

	return(true);
}

bool
EventManager::disconnect(IEventListener *handler, const Core::Type &t)
{
	MMINFO("Disconnecting `" << &handler << "` handler from event type `" << t.str() << "`");

	EventListenerMap::const_iterator l_elmapi =
	    m_p->elmap.find(t.uid());

	/* if this is a new type, assign a new EventListenerList */
	if (l_elmapi == m_p->elmap.end()) {
		MMWARNING("Failed! Event type not in registry.");
		return(false);
	}

	SharedEventListenerList l_listeners(m_p->elmap[t.uid()]);
	l_listeners->remove(handler);
	MMINFO("Disconnected! Current listener count is: " << l_listeners->size() << ".");

	/* invalidate dispatch if needed */
	m_p->dispatch_invalid |= (m_p->dispatch_type == t);

	return(true);
}

bool
EventManager::dispatch(const IEvent &event)
{
	bool l_handled = false;

	EventListenerMap::const_iterator l_elmapi =
	    m_p->elmap.find(event.type().uid());
	if (l_elmapi == m_p->elmap.end())
		return(false);

	m_p->dispatch_invalid = false;
	m_p->dispatch_type = event.type();

	SharedEventListenerList l_listeners(l_elmapi->second);
	EventListenerList::iterator l_listenersi;

	for (l_listenersi = l_listeners->begin();
	     !l_handled && !m_p->dispatch_invalid
	       && (l_listenersi != l_listeners->end());) {
		if (*l_listenersi)
			l_handled = (*l_listenersi++)->handleEvent(event);
		else
			l_listeners->erase(l_listenersi++);
	}

	if (m_p->dispatch_invalid && !l_handled)
		MMERROR("Dispatch listeners changed without current event being handled.");

	m_p->dispatch_type = Core::Type::Null;

	return(l_handled);
}

bool
EventManager::dequeue(const SharedEvent &event, bool all)
{
	EventList &l_queue = m_p->queue[m_p->active_queue == 0 ? 1 : 0];

	if (all) {
		const MMUID l_type = event->type();
		EventList::reverse_iterator l_i = l_queue.rbegin();

		while (l_i != l_queue.rend()) {
			if (l_type == (*l_i)->type()) {
			    EventList::iterator l_tmp = l_queue.erase(--l_i.base());
			    l_i = EventList::reverse_iterator(l_tmp);
			}
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
	EventList &l_queue = m_p->queue[m_p->active_queue == 0 ? 1 : 0];
	l_queue.push_back(event);
	return(true);
}

bool
EventManager::execute()
{
	/* fetch and sort active queue */
	EventList &l_queue = m_p->queue[m_p->active_queue];
	l_queue.sort(SortSharedEvent);

	/* dispatch events in active queue
	 *
	 * stop only if:
	 *     we ran out of messages
	 *     event timestamp is in the future
	 */
	SharedEvent event;
	while (!l_queue.empty()
	    && (event = l_queue.front())->timeStamp() <= NOW()) {
		dispatch(*event); l_queue.pop_front();
	}

	if (l_queue.empty()) {
		/* switch active queues */
		m_p->active_queue = (m_p->active_queue == 0 ? 1 : 0);
		return(true);
	}
	return(false);
}

EventManager *
EventManager::Instance(void)
{
	return(s_instance);
}

} /********************************************************** Event Namespace */
MARSHMALLOW_NAMESPACE_END

