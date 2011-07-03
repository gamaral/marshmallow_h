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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef EVENT_EVENTMANAGER_H
#define EVENT_EVENTMANAGER_H 1

#include "EASTL/hash_map.h"
#include "EASTL/list.h"
using namespace eastl;

#include "core/global.h"
#include "core/fd.h"
#include "core/identifier.h"
#include "core/shared.h"

#include "ieventlistener.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Event
{
	struct IEventListener;
	typedef Core::Weak<IEventListener> WeakEventListener;

	struct IEvent;
	typedef Core::Shared<IEvent> SharedEvent;

	/*! @brief Event Manager */
	class EVENT_EXPORT EventManager
	{
		typedef list<WeakEventListener> EventListenerList;
		typedef Core::Shared<EventListenerList> SharedEventListenerList;
		typedef hash_map<UID, SharedEventListenerList> EventListenerMap;

		typedef list<SharedEvent> EventList;

		static EventManager *s_instance;
		EventListenerMap m_elmap;
		EventList m_queue[2];
		Core::Identifier m_id;
		UINT8 m_active_queue;

		NO_COPY(EventManager);

	public:

		EventManager(const Core::Identifier &identifier);
		virtual ~EventManager(void);

	public: /* virtual */

		virtual const Core::Identifier & id(void) const
		    { return(m_id); }

		virtual bool connect(const WeakEventListener &handler, const Core::Type &type);
		virtual bool disconnect(const WeakEventListener &handler, const Core::Type &type);

		virtual bool dequeue(const SharedEvent &event, bool all = false);
		virtual bool queue(const SharedEvent &event);

		virtual bool dispatch(const IEvent &event);

		virtual bool execute(TIME timeout);

	public: /* static */

		static EventManager *Instance(void);

	};
	typedef Core::Shared<EventManager> SharedEventManager;
	typedef Core::Weak<EventManager> WeakEventManager;
}

MARSHMALLOW_NAMESPACE_END

#endif
