/*
 * Copyright (c) 2011-2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
 * All rights reserved.
 *
 * This file is part of Marshmallow Game Engine.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the project as a whole.
 */

#include "core/platform.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <sys/time.h>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <libgen.h>
#include <unistd.h>

#include "core/logger.h"

MARSHMALLOW_NAMESPACE_USE
using namespace Core;

#undef HAVE_CLOCK_GETTIME

/******************************************************************************/

namespace
{
#ifdef HAVE_CLOCK_GETTIME
	static struct timespec s_start_time;
#else
	static struct timeval s_start_time;
#endif
} // namespace

/******************************************************************************/

void
Platform::Initialize(void)
{
#ifdef HAVE_CLOCK_GETTIME
	clock_gettime(CLOCK_MONOTONIC_RAW, &s_start_time);
#else
	gettimeofday(&s_start_time, 0);
#endif
	srand(static_cast<unsigned int>(s_start_time.tv_sec));
}

void
Platform::Finalize(void)
{
}

void
Platform::Sleep(MMTIME timeout)
{
	if (timeout <= 0) return;

	MMVERBOSE("Sleeping for " << timeout << " seconds.");

	struct timespec l_ts;
	l_ts.tv_sec = static_cast<__time_t>(trunc(timeout));
#define NANOSECONDS_PER_SECOND 1000000000
	l_ts.tv_nsec = static_cast<__syscall_slong_t>
	    ((timeout - float(l_ts.tv_sec)) * NANOSECONDS_PER_SECOND);
	nanosleep(&l_ts, 0);
}

time_t
Platform::StartTime(void)
{
	return(s_start_time.tv_sec);
}

MMTIME
Platform::TimeStamp(void)
{
	MMTIME out;
#ifdef HAVE_CLOCK_GETTIME
	struct timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);
	out = static_cast<MMTIME>(time.tv_sec - s_start_time.tv_sec)
	    + ((time.tv_nsec - s_start_time.tv_nsec) / 1000000000.);
#else
	struct timeval time;
	gettimeofday(&time, 0);
	out = static_cast<MMTIME>(time.tv_sec - s_start_time.tv_sec)
	    + ((time.tv_usec - s_start_time.tv_usec) / 1000000.);
#endif
	return(out);
}

TimeData
Platform::TimeStampToTimeData(MMTIME timestamp)
{
	TimeData l_ts;
	struct tm l_time;

	l_ts.internal = timestamp;
	l_ts.system =
	    static_cast<time_t>(Platform::StartTime() +
	    static_cast<time_t>(l_ts.internal/1000));

	gmtime_r(&l_ts.system, &l_time);

	sprintf(l_ts.string, "%04d-%02d-%02dT%02d:%02d:%02dZ",
		l_time.tm_year + 1900,
		l_time.tm_mon  + 1,
		l_time.tm_mday,
		l_time.tm_hour,
		l_time.tm_min,
		l_time.tm_sec);

	return(l_ts);
}

std::string
Platform::PathDirectory(const std::string &path)
{
	char l_path[PATH_MAX + 1];

	memset(&l_path, 0, sizeof(l_path));
	path.copy(l_path, PATH_MAX);

	return(dirname(l_path));
}

std::string
Platform::TemporaryDirectory(void)
{
	return("/tmp/");
}

