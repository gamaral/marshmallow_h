/*
 * Copyright 2011-2012 Marshmallow Platform. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY MARSHMALLOW PLATFORM ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MARSHMALLOW PLATFORM OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those of the
 * authors and should not be interpreted as representing official policies, either expressed
 * or implied, of Marshmallow Platform.
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

#define NOW Core::Platform::TimeStamp

MARSHMALLOW_NAMESPACE_BEGIN

struct TimeData;

namespace Core
{
	/*! @brief Platform specific class */
	namespace Platform
	{
		/*!
		 * @brief Platform specific initialization
		 */
		MARSHMALLOW_CORE_EXPORT
		void Initialize(void);

		/*!
		 * @brief Platform specific finalization
		 */
		MARSHMALLOW_CORE_EXPORT
		void Finalize(void);

		/****************************************************** time */

		/*!
		 * @brief Sleep in milliseconds
		 */
		MARSHMALLOW_CORE_EXPORT
		void Sleep(TIME timeout);

		/*!
		 * @brief Start system time
		 */
		MARSHMALLOW_CORE_EXPORT
		time_t StartTime(void);

		/*!
		 * @brief Milliseconds since StartTime()
		 */
		MARSHMALLOW_CORE_EXPORT
		TIME TimeStamp(void);

		/*!
		 * @brief Reinterpret an internal timestamp
		 * @return Internal timestamp in system timestamp and string
		 *         formats.
		 */
		MARSHMALLOW_CORE_EXPORT
		TimeData TimeStampToTimeData(TIME timestamp);

		/*************************************************** location */

		MARSHMALLOW_CORE_EXPORT
		std::string TemporaryDirectory(void);
	}
}

/*! @brief Time Data Struct */
struct TimeData
{
    char string[21];
    time_t system;
    TIME internal;
};

MARSHMALLOW_NAMESPACE_END

#endif
