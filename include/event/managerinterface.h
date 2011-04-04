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

#ifndef EVENT_IMANAGER_H
#define EVENT_IMANAGER_H 1

#include "core/global.h"
#include "core/shared.h"
#include "event/eventtype.h"
#include "event/listenerinterface.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Event
{
	/*! @brief Event manager interface */
	struct EVENT_EXPORT ManagerInterface
	{
		virtual ~ManagerInterface(void) {};

		/*!
		 * @brief VIRTUAL Connect an event listener to a specific event type
		 * @param handler Shared event handler
		 * @param type Event type
		 */
		virtual bool connect(const SharedListenerInterface &handler, const EventType &type) = 0;

		/*!
		 * @brief VIRTUAL Disconnect an event listener from a specific event type
		 * @param handler Shared event handler
		 * @param type Event type
		 */
		virtual bool disconnect(const SharedListenerInterface &handler, const EventType &type) = 0;

		/*!
		 * @brief VIRTUAL Dequeue an already queued event
		 * @param event Event
		 * @param all Remove all events of the same type
		 */
		virtual bool dequeue(const SharedEventInterface &event, bool all = false) = 0;

		/*!
		 * @brief VIRTUAL Queue an event
		 * @param event Event
		 */
		virtual bool queue(const SharedEventInterface &event) = 0;

		/*!
		 * @brief VIRTUAL Dispatch event immediately
		 * @param event Event
		 */
		virtual bool dispatch(const EventInterface &event) const = 0;

		/*!
		 * @brief VIRTUAL Dispatch shared event immediately
		 * @param event Event
		 * @return Returns true if message was consumed
		 */
		virtual bool dispatch(const SharedEventInterface &event) const = 0;

		/*!
		 * @brief VIRTUAL Render handler
		 * @param timeout Timeout
		 */
		virtual bool render(TIME timeout = INFINITE) = 0;

		/*!
		 * @brief VIRTUAL Tick handler
		 * @param timeout Timeout
		 * @return Returns true if all messages in active queue where
		 *         dispatched
		 */
		virtual bool tick(TIME timeout = INFINITE) = 0;

		/*!
		 * @brief VIRTUAL Update handler
		 * @param timeout Timeout
		 */
		virtual bool update(TIME timeout = INFINITE) = 0;
	};
}

MARSHMALLOW_NAMESPACE_END

#endif
