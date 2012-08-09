/*
 * Copyright 2012 Marshmallow Engine. All rights reserved.
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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef MARSHMALLOW_INPUT_LINUX_EVDEV_MAP_H
#define MARSHMALLOW_INPUT_LINUX_EVDEV_MAP_H 1

#include "core/environment.h"
#include "core/namespace.h"

#include <linux/input.h>

#include <map>

#include "evdev_type.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Input { /****************************************** Input Namespace */

/**** IMPLEMENTATION NOTES *****************************************************
 *
 */
namespace Linux { /*********************************** Input::Linux Namespace */
namespace EventDevice { /**************** Input::Linux::EventDevice Namespace */
namespace Map { /******************* Input::Linux::EventDevice::Map Namespace */

	typedef std::map<uint16_t, int> EventCodes;

	bool Initialize(void);

	void Finalize(void);

	bool PopulateEventCodes(__u16 vendor, __u16 product, const char *name,
	                        Type type, int event, EventCodes &codes);

} /********************************* Input::Linux::EventDevice::Map Namespace */
} /************************************** Input::Linux::EventDevice Namespace */
} /*************************************************** Input::Linux Namespace */
} /********************************************************** Input Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
