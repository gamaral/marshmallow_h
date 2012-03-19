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

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#pragma once

#ifndef MARSHMALLOW_CORE_LOGGER_H
#define MARSHMALLOW_CORE_LOGGER_H 1

#include <cstdio>
#include <cstdlib>

#include <core/config.h>
#include <core/environment.h>
#include <core/platform.h>

/*
 * TODO: Replace with Core::Logger
 */

#define MMFATAL(x,...) fprintf(stderr, "%s [%s:%d]: FATAL: "x"\n", Core::Platform::TimeStampToTimeData(NOW()).string, __FILE__, __LINE__, __VA_ARGS__),exit(-1)
#define MMFATAL1(x) fprintf(stderr, "%s [%s:%d]: FATAL: "x"\n", Core::Platform::TimeStampToTimeData(NOW()).string, __FILE__, __LINE__),exit(-1)

#define MMERROR(x,...) fprintf(stderr, "%s [%s:%d]: ERROR: "x"\n", Core::Platform::TimeStampToTimeData(NOW()).string, __FILE__, __LINE__, __VA_ARGS__)
#define MMERROR1(x) fprintf(stderr, "%s [%s:%d]: ERROR: "x"\n", Core::Platform::TimeStampToTimeData(NOW()).string, __FILE__, __LINE__)

#if MARSHMALLOW_DEBUG && MARSHMALLOW_DEBUG_VERBOSITY >= 1
#   define MMWARNING(x,...) fprintf(stderr, "%s [%s:%d]: WARN: "x"\n", Core::Platform::TimeStampToTimeData(NOW()).string, __FILE__, __LINE__, __VA_ARGS__)
#   define MMWARNING1(x) fprintf(stderr, "%s [%s:%d]: WARN: "x"\n", Core::Platform::TimeStampToTimeData(NOW()).string, __FILE__, __LINE__)
#else
#   define MMWARNING(x,...) (void)0
#   define MMWARNING1(x) (void)0
#endif

#if MARSHMALLOW_DEBUG && MARSHMALLOW_DEBUG_VERBOSITY >= 2
#   define MMINFO(x,...) fprintf(stderr, "%s [%s:%d]: INFO: "x"\n", Core::Platform::TimeStampToTimeData(NOW()).string, __FILE__, __LINE__, __VA_ARGS__)
#   define MMINFO1(x) fprintf(stderr, "%s [%s:%d]: INFO: "x"\n", Core::Platform::TimeStampToTimeData(NOW()).string, __FILE__, __LINE__)
#else
#   define MMINFO(...) (void)0
#   define MMINFO1(...) (void)0
#endif

#if MARSHMALLOW_DEBUG && MARSHMALLOW_DEBUG_VERBOSITY >= 3
#   define MMVERBOSE(x,...) fprintf(stderr, "%s [%s:%d]: INFO: "x"\n", Core::Platform::TimeStampToTimeData(NOW()).string, __FILE__, __LINE__, __VA_ARGS__)
#   define MMVERBOSE1(x) fprintf(stderr, "%s [%s:%d]: INFO: "x"\n", Core::Platform::TimeStampToTimeData(NOW()).string, __FILE__, __LINE__)
#else
#   define MMVERBOSE(...) (void)0
#   define MMVERBOS1(...) (void)0
#endif

#endif
