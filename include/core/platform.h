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

#ifndef MARSHMALLOW_CORE_PLATFORM_H
#define MARSHMALLOW_CORE_PLATFORM_H 1

#include <string>

#include <core/environment.h>
#include <core/namespace.h>

#define NOW MARSHMALLOW_NAMESPACE::Core::Platform::TimeStamp

MARSHMALLOW_NAMESPACE_BEGIN

struct TimeData;

namespace Core
{
	/*!
	 * @brief A collection of platform-specific methods
	 */
	namespace Platform
	{
		MARSHMALLOW_CORE_EXPORT
		void Initialize(void);

		MARSHMALLOW_CORE_EXPORT
		void Finalize(void);

		/****************************************************** time */

		/*!
		 * Call the platform specific sleep function
		 *
		 * @param timeout Timeout in milliseconds
		 */
		MARSHMALLOW_CORE_EXPORT
		void Sleep(MMTIME timeout);

		/*!
		 * Returns the engine start system time
		 */
		MARSHMALLOW_CORE_EXPORT
		time_t StartTime(void);

		/*!
		 * Returns Milliseconds since engine was started (StartTime())
		 */
		MARSHMALLOW_CORE_EXPORT
		MMTIME TimeStamp(void);

		/*!
		 * Reinterprets an internal timestamp into TimeData
		 *
		 * @param timestamp Internal timestamp
		 */
		MARSHMALLOW_CORE_EXPORT
		TimeData TimeStampToTimeData(MMTIME timestamp);

		/*************************************************** location */

		MARSHMALLOW_CORE_EXPORT
		std::string PathDirectory(const std::string &path);

		MARSHMALLOW_CORE_EXPORT
		std::string TemporaryDirectory(void);
	}
}

/*!
 * The TimeData structure contains a reinterpreted internal time stamp as both
 * system time and a string
 */
struct TimeData
{
    char string[21];
    time_t system;
    MMTIME internal;
};

MARSHMALLOW_NAMESPACE_END

#endif
