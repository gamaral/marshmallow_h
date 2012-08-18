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

#include "joystick_p.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifdef MARSHMALLOW_EVDEV_JOYSTICK
#  ifdef MARSHMALLOW_INPUT_LINUX_EVDEV
#    include "linux/evdev.h"
#  else
#    error "You can't use EVDEV JOYSTICK without overall EVDEV support."
#  endif
#endif

#include <map>

/*
 * Input Notes
 *
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace Input { /****************************************** Input Namespace */

bool
Joystick::Initialize(void)
{
	bool l_status = true;

#ifdef MARSHMALLOW_EVDEV_JOYSTICK
	l_status &= Input::Linux::EVDEV::InitializeJoystick();
#endif

	return(l_status);
}

void
Joystick::Finalize(void)
{
#ifdef MARSHMALLOW_EVDEV_JOYSTICK
	Input::Linux::EVDEV::FinalizeJoystick();
#endif
}

void
Joystick::Tick(float)
{
#ifdef MARSHMALLOW_EVDEV_JOYSTICK
	Input::Linux::EVDEV::TickJoystick();
#endif
}

} /********************************************************** Input Namespace */
MARSHMALLOW_NAMESPACE_END

