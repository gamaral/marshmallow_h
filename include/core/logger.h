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

#include <core/config.h>
#include <core/environment.h>

#include <cstdlib>
#include <iostream>

#define MMLOG(type, x) std::cerr << __FILE__ << ":" << __LINE__ << " ["type"]" << std::endl \
                                 << "\t" << MMFUNCTION << ": " << x << std::endl

#define MMFATAL(x) MMLOG("FATAL", x), exit(-1)
#define MMERROR(x) MMLOG("ERROR", x)

#if MARSHMALLOW_DEBUG
#   define MMDEBUG(x) MMLOG("DEBUG", x)
#else
#   define MMDEBUG(x) MMNOOP
#endif

#if MARSHMALLOW_DEBUG && MARSHMALLOW_DEBUG_VERBOSITY >= 1
#   define MMWARNING(x) MMLOG("WARN", x)
#else
#   define MMWARNING(x) MMNOOP
#endif

#if MARSHMALLOW_DEBUG && MARSHMALLOW_DEBUG_VERBOSITY >= 2
#   define MMINFO(x) MMLOG("INFO", x)
#else
#   define MMINFO(x) MMNOOP
#endif

#if MARSHMALLOW_DEBUG && MARSHMALLOW_DEBUG_VERBOSITY >= 3
#   define MMVERBOSE(x) MMLOG("VERB", x)
#else
#   define MMVERBOSE(x) MMNOOP
#endif

#endif
