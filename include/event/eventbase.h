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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef MARSHMALLOW_EVENT_EVENTBASE_H
#define MARSHMALLOW_EVENT_EVENTBASE_H 1

#include <event/ievent.h>

#include <core/identifier.h>

MARSHMALLOW_NAMESPACE_BEGIN

namespace Event
{
	/*! @brief Event Priority */
	enum EventPriority
	{
		LowestPriority  = 0,
		LowerPriority   = 1,
		LowPriority     = 2,
		NormalPriority  = 5,
		HighPriority    = 8,
		HigherPriority  = 9,
		HighestPriority = 10
	};

	/*! @brief Event Base Class */
	class MARSHMALLOW_EVENT_EXPORT
	EventBase : public IEvent
	{
		struct Private;
		Private *m_p;

		NO_ASSIGN_COPY(EventBase);
	public:

		/*! @brief Event constructor
		 *
		 *  @param timestamp Event engine will process this message at
		 *                   timestamp, use 0 for NOW.
		 *
		 *  @param priority  Use a higher value to get higher priority.
		 */

		EventBase(TIME timestamp = 0, uint8_t priority = 0);
		virtual ~EventBase(void);

	public: /* virtual */

		VIRTUAL uint8_t priority(void) const;

		VIRTUAL TIME timeStamp(void) const;
	};
}

MARSHMALLOW_NAMESPACE_END

#endif
