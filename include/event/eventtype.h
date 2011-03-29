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

#ifndef EVENT_EVENTTYPE_H
#define EVENT_EVENTTYPE_H 1

#include <sstream>
#include <stdint.h>
#include <unistd.h>

#include "core/global.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Event
{

	/*! @brief Unique-Event-ID type */
	typedef UINT32 UID;

	/*! @brief Event type class */
	class EventType
	{
		char *m_name;
		UID m_uid;

	public:

		/*!
		 * @brief Event type constructor
		 * @param name Event type name
		 */
		EventType(const char *name);
		EventType(const EventType &copy);
		virtual ~EventType(void);

		/*! @brief Unique ID */
		UID uid(void) const
		    { return(m_uid); }

		/*! @brief Event type name */
		const char * name(void) const
		    { return(m_name); }

	public: /* operator */

		/*! @brief Equal comparison operator */
		bool operator ==(const EventType &rhs) const
		    { return(m_uid == rhs.m_uid); }

		/*! @brief Less comparison operator */
		bool operator <(const EventType &rhs) const
		    { return(m_uid < rhs.m_uid); }

		/*! @brief Name cast operator */
		operator const char *() const
		    { return(m_name); }

		/*! @brief UID cast operator */
		operator const UID() const
		    { return(m_uid); }

	public: /* static */

		/*! @brief Hash used to calculate uid
		 * One-at-a-Time Hash
		 */
		static UID Hash(const char *data, size_t len, UID mask);
	};

}

MARSHMALLOW_NAMESPACE_END

#endif
