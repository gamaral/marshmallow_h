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

#ifndef MARSHMALLOW_CORE_ENVIRONMENT_H
#define MARSHMALLOW_CORE_ENVIRONMENT_H 1

#include <climits>
#include <ctime>
#include <stdint.h>
#include <unistd.h>

/********************************************************************** types */

#define MMTIME     int
#define MMTIME_MAX INT_MAX
#define MMUID      uint32_t

/******************************************************************** defines */

#define MMCHDIR      chdir
#define MMINFINITE   TIME_MAX
#define MMFUNCTION __PRETTY_FUNCTION__
#define MMNOOP       (void)0
#define MMSTRCASECMP strcasecmp
#define MMSTRDUP     strdup

/******************************************************************** unused */

#define MARSHMALLOW_ENTRYPOINT_EXPORT
#define MARSHMALLOW_CORE_EXPORT
#define MARSHMALLOW_MATH_EXPORT
#define MARSHMALLOW_EVENT_EXPORT
#define MARSHMALLOW_GRAPHICS_EXPORT
#define MARSHMALLOW_GAME_EXPORT
#define MARSHMALLOW_EXTRA_EXPORT

#endif
