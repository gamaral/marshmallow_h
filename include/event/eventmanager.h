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

#include "event/ieventmanager.h"

#include "EASTL/hash_map.h"
#include "EASTL/list.h"
using namespace eastl;

MARSHMALLOW_NAMESPACE_BEGIN

namespace Event
{
	struct IEventListener;

	/*! @brief Event Manager */
	class EVENT_EXPORT EventManager : public IEventManager
	{
		typedef Core::Shared<IEvent> SharedEvent;
		typedef Core::Shared<IEventListener> SharedEventListener;

		typedef list<SharedEventListener> EventListenerList;
		typedef hash_map<UID, EventListenerList> EventListenerMap;

		typedef list<SharedEvent> EventList;

		EventListenerMap m_elmap;
		EventList m_queue[2];
		UINT8 m_active_queue;
		Core::Identifier m_id;

	public:

		EventManager(const Core::Identifier &identifier);
		virtual ~EventManager(void);

		bool dispatch(const SharedEvent &event) const
		    { return(dispatch(*event)); }

	public: /* virtual */

		VIRTUAL const Core::Identifier & id(void) const
		    { return(m_id); }

		VIRTUAL bool connect(const SharedEventListener &handler, const Core::Type &type);
		VIRTUAL bool disconnect(const SharedEventListener &handler, const Core::Type &type);

		VIRTUAL bool dequeue(const SharedEvent &event, bool all = false);
		VIRTUAL bool queue(const SharedEvent &event);

		VIRTUAL bool dispatch(const IEvent &event) const;

		VIRTUAL bool execute(TIME timeout);
	};
}

MARSHMALLOW_NAMESPACE_END

#endif
