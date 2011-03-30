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

#include <ctime>
#include <sys/time.h>

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

