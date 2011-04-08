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

#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H 1

#include "event/imanagerinterface.h"

#include "EASTL/hash_map.h"
#include "EASTL/list.h"
using namespace eastl;

MARSHMALLOW_NAMESPACE_BEGIN

namespace Core { class Type; }

namespace Event
{
	struct IListenerInterface;

	/*! @brief Event manager base */
	class EVENT_EXPORT Manager : public IManagerInterface
	{
		typedef Core::Shared<IEventInterface> SharedEvent;
		typedef Core::Shared<IListenerInterface> SharedListener;

		typedef list<SharedListener> EventListenerList;
		typedef hash_map<UID, EventListenerList> EventListenerMap;

		typedef list<SharedEvent> EventList;

		EventListenerMap m_elmap;
		EventList m_queue[2];
		UINT8 m_active_queue;

	public:

		Manager(const char *name = "");
		virtual ~Manager(void);

		bool dispatch(const SharedEvent &event) const
		    { return(dispatch(*event)); }

	public: /* virtual */

		VIRTUAL bool connect(const SharedListener &handler, const Core::Type &type);
		VIRTUAL bool disconnect(const SharedListener &handler, const Core::Type &type);

		VIRTUAL bool dequeue(const SharedEvent &event, bool all = false);
		VIRTUAL bool queue(const SharedEvent &event);

		VIRTUAL bool dispatch(const IEventInterface &event) const;

		VIRTUAL bool execute(TIME timeout);
	};
}

MARSHMALLOW_NAMESPACE_END

#endif
