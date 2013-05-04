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

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#pragma once

#ifndef MARSHMALLOW_CORE_ENVIRONMENT_H
#define MARSHMALLOW_CORE_ENVIRONMENT_H 1

#include <ctime>
#include <direct.h>
#include <stdint.h>
#include <windows.h>

/********************************************************************** types */

#define MMTIME     int
#define MMTIME_MAX INT_MAX
#define MMUID      uint32_t
typedef SIZE_T     size_t;
typedef SSIZE_T    ssize_t;

/******************************************************************** defines */

#define MMCHDIR      _chdir
#define MMFUNCTION __FUNCTION__
#define MMINFINITE   TIME_MAX
#define MMNOOP       __noop
#define MMSTRCASECMP lstrcmpiA
#define MMSTRDUP     _strdup

/******************************************************************** exports */

#ifdef MARSHMALLOW_SHARED
#   define MARSHMALLOW_DLL_EXPORT __declspec(dllexport)
#   define MARSHMALLOW_DLL_IMPORT __declspec(dllimport)
#else
#   define MARSHMALLOW_DLL_EXPORT
#   define MARSHMALLOW_DLL_IMPORT
#endif

#ifdef MARSHMALLOW_CORE_LIBRARY
#   define MARSHMALLOW_CORE_EXPORT MARSHMALLOW_DLL_EXPORT
#else
#   define MARSHMALLOW_CORE_EXPORT MARSHMALLOW_DLL_IMPORT
#endif

#ifdef MARSHMALLOW_ENTRYPOINT_LIBRARY
#   define MARSHMALLOW_ENTRYPOINT_EXPORT MARSHMALLOW_DLL_EXPORT
#else
#   define MARSHMALLOW_ENTRYPOINT_EXPORT MARSHMALLOW_DLL_IMPORT
#endif

#ifdef MARSHMALLOW_MATH_LIBRARY
#   define MARSHMALLOW_MATH_EXPORT MARSHMALLOW_DLL_EXPORT
#else
#   define MARSHMALLOW_MATH_EXPORT MARSHMALLOW_DLL_IMPORT
#endif

#ifdef MARSHMALLOW_EVENT_LIBRARY
#   define MARSHMALLOW_EVENT_EXPORT MARSHMALLOW_DLL_EXPORT
#else
#   define MARSHMALLOW_EVENT_EXPORT MARSHMALLOW_DLL_IMPORT
#endif

#ifdef MARSHMALLOW_INPUT_LIBRARY
#   define MARSHMALLOW_INPUT_EXPORT MARSHMALLOW_DLL_EXPORT
#else
#   define MARSHMALLOW_INPUT_EXPORT MARSHMALLOW_DLL_IMPORT
#endif

#ifdef MARSHMALLOW_GRAPHICS_LIBRARY
#   define MARSHMALLOW_GRAPHICS_EXPORT MARSHMALLOW_DLL_EXPORT
#else
#   define MARSHMALLOW_GRAPHICS_EXPORT MARSHMALLOW_DLL_IMPORT
#endif

#ifdef MARSHMALLOW_AUDIO_LIBRARY
#   define MARSHMALLOW_AUDIO_EXPORT MARSHMALLOW_DLL_EXPORT
#else
#   define MARSHMALLOW_AUDIO_EXPORT MARSHMALLOW_DLL_IMPORT
#endif

#ifdef MARSHMALLOW_GAME_LIBRARY
#   define MARSHMALLOW_GAME_EXPORT MARSHMALLOW_DLL_EXPORT
#else
#   define MARSHMALLOW_GAME_EXPORT MARSHMALLOW_DLL_IMPORT
#endif

#ifdef MARSHMALLOW_EXTRA_LIBRARY
#   define MARSHMALLOW_EXTRA_EXPORT MARSHMALLOW_DLL_EXPORT
#else
#   define MARSHMALLOW_EXTRA_EXPORT MARSHMALLOW_DLL_IMPORT
#endif

#endif
