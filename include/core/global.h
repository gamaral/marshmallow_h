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

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#pragma once

#ifndef CORE_GLOBAL_H
#define CORE_GLOBAL_H 1

#include <cstdio>
#include <cstdlib>

#include "core/environment.h"

#include "config.h"

#ifdef MARSHMALLOW_NAMESPACE
#   define MARSHMALLOW_NAMESPACE_BEGIN namespace MARSHMALLOW_NAMESPACE {
#   define MARSHMALLOW_NAMESPACE_END   }
#   define MARSHMALLOW_NAMESPACE_USE   using namespace MARSHMALLOW_NAMESPACE
#else
#   define MARSHMALLOW_NAMESPACE_BEGIN
#   define MARSHMALLOW_NAMESPACE_END
#   define MARSHMALLOW_NAMESPACE_USE
#endif

#if defined(DEBUG) && DEBUG_VERBOSITY >= 1
#   define WARNING(x,...) fprintf(stderr, "%s [%s:%d]: WARN: "x"\n", Core::Platform::TimeStampToTimeData(NOW()).string, __FILE__, __LINE__, __VA_ARGS__)
#   define WARNING1(x) fprintf(stderr, "%s [%s:%d]: WARN: "x"\n", Core::Platform::TimeStampToTimeData(NOW()).string, __FILE__, __LINE__)
#else
#   define WARNING(x,...) (void)0
#   define WARNING1(x) (void)0
#endif

#if defined(DEBUG) && DEBUG_VERBOSITY >= 2
#   define INFO(x,...) fprintf(stderr, "%s [%s:%d]: INFO: "x"\n", Core::Platform::TimeStampToTimeData(NOW()).string, __FILE__, __LINE__, __VA_ARGS__)
#   define INFO1(x) fprintf(stderr, "%s [%s:%d]: INFO: "x"\n", Core::Platform::TimeStampToTimeData(NOW()).string, __FILE__, __LINE__)
#else
#   define INFO(...) (void)0
#   define INFO1(...) (void)0
#endif

#define VIRTUAL
#define UNUSED(x) (void)x
#define MIN(x,y) (x < y ? x : y)
#define MAX(x,y) (x > y ? x : y)
#define NOW Core::Platform::TimeStamp

#define TIMEOUT_INIT const TIME l_start_time = Platform::TimeStamp()
#define TIMEOUT_DEC(x) (x -= (NOW() - l_start_time))

/******************************************************************** global */

MARSHMALLOW_NAMESPACE_BEGIN

struct TimeData {
    char string[18];
    time_t system;
    TIME internal;
};

MARSHMALLOW_NAMESPACE_END

#endif
