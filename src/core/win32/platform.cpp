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

#include <windows.h>
#include <time.h>

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
Platform::Sleep(TIME t)
{
	if (t > 0) SleepEx(static_cast<DWORD>(floor(t)), true);
}

time_t
Platform::StartTime(void)
{
	return(platform_internal.start_time);
}

TIME
Platform::TimeStamp(void)
{
	FILETIME        l_ft;
	LARGE_INTEGER   l_li;
	INT64           l_micro_seconds;
	TIME            l_mseconds;

	GetSystemTimeAsFileTime(&l_ft);
	l_li.LowPart  = l_ft.dwLowDateTime;
	l_li.HighPart = l_ft.dwHighDateTime;
#define EPOCHFILETIME (116444736000000000i64)
	l_micro_seconds = (l_li.QuadPart - EPOCHFILETIME) / 10;
#define MICROSECONDS_PER_MILLISECOND 1000.0
#define MILLISECONDS_PER_SECOND 1000.0
	l_mseconds = (l_micro_seconds / MICROSECONDS_PER_MILLISECOND)
	    -(platform_internal.start_time * MILLISECONDS_PER_SECOND);
	return(l_mseconds);
}

TimeData
Platform::TimeStampToTimeData(TIME timestamp)
{
	TimeData l_ts;
	struct tm l_time;

	l_ts.internal = timestamp;
	l_ts.system =
	    static_cast<time_t>(Platform::StartTime()+(l_ts.internal/1000));

	_gmtime64_s(&l_time, &l_ts.system);

	sprintf_s(l_ts.string, "%04d-%02d-%02dT%02d:%02d:%02dZ",
		l_time.tm_year+1900,
		l_time.tm_mon+1,
		l_time.tm_mday,
		l_time.tm_hour,
		l_time.tm_min,
		l_time.tm_sec);

	return(l_ts);
}

String
Platform::TemporaryDirectory(void)
{
	static String sTempDirectory;

	if (sTempDirectory.empty()) {
		char l_buffer[MAX_PATH];
		GetTempPath(MAX_PATH, l_buffer);
		sTempDirectory.assign(l_buffer);
	}

	return(sTempDirectory);
}

