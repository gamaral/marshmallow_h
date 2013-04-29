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

#include "event/eventmanager.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/logger.h"
#include "core/platform.h"
#include "core/type.h"

#include "event/ievent.h"
#include "event/ieventlistener.h"

#include <algorithm>
#include <list>
#include <map>

/*
 * Implementation Notes
 * ====================
 *
 * The event manager allocates an event list for every event type encountered,
 * this means it will have to deallocate them upon destruction.
 *
 * Queued events will be automatically freed after execution.
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace { /******************************************** Anonymous Namespace */

	using namespace Event;

	static EventManager *s_instance(0);

	static bool
	SortEvent(const IEvent *lhs, const IEvent *rhs) {
		return(lhs->priority() > rhs->priority()
		   || lhs->timeStamp() < rhs->timeStamp());
	}

	typedef std::list<const IEvent *> EventList;
	typedef std::list<IEventListener *> EventListenerList;
	typedef std::map<MMUID, EventListenerList *> EventListenerMap;

} /****************************************************** Anonymous Namespace */

namespace Event { /****************************************** Event Namespace */

struct EventManager::Private
{
	Private(const Core::Identifier &i)
	    : id(i)
	    , dispatch_type(Core::Type::Null)
	    , active_queue(0)
	    , dispatch_invalid(true)
	{}

	~Private();

	inline bool connect(IEventListener *handler, const Core::Type &type);
	inline bool disconnect(IEventListener *handler, const Core::Type &type);

	inline bool queue(const IEvent *event);
	inline bool dequeue(const IEvent *event, bool all = false);

	inline bool dispatch(const IEvent &event);

	inline bool execute(void);

	EventListenerMap listener_map;
#define QUEUE_MAX 2
	EventList event_queue[QUEUE_MAX];
	Core::Identifier id;
	Core::Type dispatch_type;
	uint8_t active_queue;
	bool dispatch_invalid;
};

EventManager::Private::~Private()
{
	/* flush event queues */
	for (int i = 0; i < QUEUE_MAX; ++i) {
		EventList &l_queue = event_queue[i];
		while (!l_queue.empty()) {
			delete l_queue.front();
			l_queue.pop_front();
		}
	}
}

bool
EventManager::Private::connect(IEventListener *handler, const Core::Type &t)
{
	MMINFO("Connecting `" << &handler
	                      << "` handler to event type `"
	                      << t.str()
	                      << "`.");

	EventListenerMap::const_iterator l_listener_mapi =
	    listener_map.find(t.uid());

	/* if this is a new type, assign a new EventListenerList */
	if (l_listener_mapi == listener_map.end())
		listener_map[t.uid()] = new EventListenerList;

	EventListenerList *l_listeners(listener_map[t.uid()]);

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
	dispatch_invalid |= (dispatch_type == t);

	return(true);
}

bool
EventManager::Private::disconnect(IEventListener *handler, const Core::Type &t)
{
	MMINFO("Disconnecting `" << &handler << "` handler from event type `" << t.str() << "`");

	EventListenerMap::const_iterator l_listener_mapi =
	    listener_map.find(t.uid());

	/* if this is a new type, assign a new EventListenerList */
	if (l_listener_mapi == listener_map.end()) {
		MMWARNING("Failed! Event type not in registry.");
		return(false);
	}

	EventListenerList *l_listeners(listener_map[t.uid()]);
	l_listeners->remove(handler);
	MMINFO("Disconnected! Current listener count is: " << l_listeners->size() << ".");

	/* invalidate dispatch if needed */
	dispatch_invalid |= (dispatch_type == t);

	return(true);
}

bool
EventManager::Private::dispatch(const IEvent &event)
{
	bool l_handled = false;

	EventListenerMap::const_iterator l_listener_mapi =
	    listener_map.find(event.type().uid());
	if (l_listener_mapi == listener_map.end())
		return(false);

	dispatch_invalid = false;
	dispatch_type = event.type();

	EventListenerList *l_listeners(l_listener_mapi->second);
	EventListenerList::iterator l_listenersi;

	for (l_listenersi = l_listeners->begin();
	     !l_handled && !dispatch_invalid
	       && (l_listenersi != l_listeners->end());) {
		if (*l_listenersi)
			l_handled = (*l_listenersi++)->handleEvent(event);
		else
			l_listeners->erase(l_listenersi++);
	}

	if (dispatch_invalid && !l_handled)
		MMERROR("Dispatch listeners changed without current event being handled.");

	dispatch_type = Core::Type::Null;

	return(l_handled);
}

bool
EventManager::Private::queue(const IEvent *event)
{
	EventList &l_queue = event_queue[active_queue == 0 ? 1 : 0];
	l_queue.push_back(event);
	return(true);
}

bool
EventManager::Private::dequeue(const IEvent *event, bool all)
{
	EventList &l_queue = event_queue[active_queue == 0 ? 1 : 0];

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
EventManager::Private::execute()
{
	/* fetch and sort active queue */
	EventList &l_queue = event_queue[active_queue];
	l_queue.sort(SortEvent);

	/* dispatch events in active queue
	 *
	 * stop only if:
	 *     we ran out of messages
	 *     event timestamp is in the future
	 */
	const IEvent *l_event;
	while (!l_queue.empty()
	    && (l_event = l_queue.front())->timeStamp() <= NOW()) {
		dispatch(*l_event);
		l_queue.pop_front();
		delete l_event;
	}

	if (l_queue.empty()) {
		/* switch active queues */
		active_queue = (active_queue == 0 ? 1 : 0);
		return(true);
	}
	return(false);
}

EventManager::EventManager(const Core::Identifier &i)
    : PIMPL_CREATE_X(i)
{
	if (!s_instance) s_instance = this;
}

EventManager::~EventManager(void)
{
	if (s_instance == this) s_instance = 0;

	PIMPL_DESTROY;
}

const Core::Identifier &
EventManager::id(void) const
{
	return(PIMPL->id);
}

bool
EventManager::connect(IEventListener *handler, const Core::Type &t)
{
	return(PIMPL->connect(handler, t));
}

bool
EventManager::disconnect(IEventListener *handler, const Core::Type &t)
{
	return(PIMPL->disconnect(handler, t));
}

bool
EventManager::dispatch(const IEvent &event)
{
	return(PIMPL->dispatch(event));
}

bool
EventManager::queue(const IEvent *event)
{
	return(PIMPL->queue(event));
}

bool
EventManager::dequeue(const IEvent *event, bool all)
{
	return(PIMPL->dequeue(event, all));
}

bool
EventManager::execute()
{
	return(PIMPL->execute());
}

EventManager *
EventManager::Instance(void)
{
	return(s_instance);
}

} /********************************************************** Event Namespace */
MARSHMALLOW_NAMESPACE_END

