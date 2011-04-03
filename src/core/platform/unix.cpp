/*
 * Copyright 2011 Marshmallow Platform. All rights reserved.
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

#include "core/platform.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <sys/time.h>

#include <ctime>
#include <unistd.h>

MARSHMALLOW_NAMESPACE_USE;
using namespace Core;


struct Platform::Internal
{
	Internal(void);
	time_t start_time;
} platform_internal;

Platform::Internal::Internal(void)
    : start_time(time(0))
{
}

void
Platform::Initialize(void)
{
}

void
Platform::Finalize(void)
{
}

void
Platform::PreRender(TIME timeout)
{
	UNUSED(timeout);
}

void
Platform::PostRender(TIME timeout)
{
	UNUSED(timeout);
}

void
Platform::PreSecond(TIME timeout)
{
	UNUSED(timeout);
}

void
Platform::PostSecond(TIME timeout)
{
	UNUSED(timeout);
}

void
Platform::PreTick(TIME timeout)
{
	UNUSED(timeout);
}

void
Platform::PostTick(TIME timeout)
{
	UNUSED(timeout);
}

void
Platform::PreUpdate(TIME timeout)
{
	UNUSED(timeout);
}

void
Platform::PostUpdate(TIME timeout)
{
	UNUSED(timeout);
}

void
Platform::Sleep(TIME timeout)
{
	usleep(timeout * 1000);
}

time_t
Platform::StartTime(void)
{
	return(platform_internal.start_time);
}

TIME
Platform::TimeStamp(void)
{
	struct timeval time;
	gettimeofday(&time, 0);
	return((TIME)(((double)(time.tv_sec - platform_internal.start_time) * 1000)
	    + ((double)time.tv_usec / 1000) + 0.5));
}

TimeData
Platform::TimeStampToTimeData(TIME timestamp)
{
	TimeData l_ts;
	struct tm l_time;

	l_ts.internal = (timestamp == INFINITE ? Platform::TimeStamp() : timestamp);
	l_ts.system =
	    static_cast<time_t>(Platform::StartTime()+(l_ts.internal/1000));

	gmtime_r(&l_ts.system, &l_time);

	sprintf(l_ts.string, "%04d-%02d-%02dT%02d:%02d:%02dZ",
		l_time.tm_year+1900,
		l_time.tm_mon+1,
		l_time.tm_mday,
		l_time.tm_hour,
		l_time.tm_min,
		l_time.tm_sec);

	return(l_ts);
}

