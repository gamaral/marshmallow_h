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

#ifndef EVENT_IEVENTMANAGER_H
#define EVENT_IEVENTMANAGER_H 1

#include "core/shared.h"
#include "core/identifier.h"
#include "core/type.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Event
{
	struct IEvent;
	typedef Core::Shared<IEvent> SharedEvent;

	struct IEventListener;
	typedef Core::Shared<IEventListener> SharedEventListener;

	/*! @brief Event EventManager Interface */
	struct EVENT_EXPORT IEventManager
	{
		virtual ~IEventManager(void) {};

		/*!
		 * @brief Event Id
		 */
		virtual const Core::Identifier & id(void) const = 0;

		/*!
		 * @brief Event Listener Connector
		 *
		 * Connect an event listener to a specific event type
		 *
		 * @param handler Shared event handler
		 * @param type Event type
		 */
		virtual bool connect(const SharedEventListener &handler,
		    const Core::Type &type) = 0;

		/*!
		 * @brief Event Listener Disconnector
		 *
		 * Disconnect an event listener from a specific event type
		 *
		 * @param handler Shared event handler
		 * @param type Event type
		 */
		virtual bool disconnect(const SharedEventListener &handler,
		    const Core::Type &type) = 0;

		/*!
		 * @brief Event Dequeue
		 *
		 * Abort a queued event
		 *
		 * @param event Event
		 * @param all Remove all events of the same type
		 */
		virtual bool dequeue(const SharedEvent &event, bool all = false) = 0;

		/*!
		 * @brief Event Queue
		 *
		 * Add event to queue
		 *
		 * @param event Event
		 */
		virtual bool queue(const SharedEvent &event) = 0;

		/*!
		 * @brief Event Dispatcher
		 *
		 * Dispatch an event immediately
		 *
		 * @param event Event
		 * @return Returns true if message was consumed
		 */
		virtual bool dispatch(const IEvent &event) const = 0;

		/*!
		 * @brief Execute Queue
		 *
		 * Execute queued events
		 *
		 * @param timeout Timeout
		 * @return Returns true if all messages in active queue where
		 *         dispatched
		 */
		virtual bool execute(TIME timeout) = 0;
	};
	typedef Core::Shared<IEventManager> SharedEventManager;
}

MARSHMALLOW_NAMESPACE_END

#endif
