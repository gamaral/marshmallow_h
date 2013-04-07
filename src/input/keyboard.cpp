/*
 * Copyright 2012-2013 Marshmallow Engine. All rights reserved.
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

#include "keyboard_p.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifdef MARSHMALLOW_EVDEV_KEYBOARD
#  ifdef MARSHMALLOW_INPUT_LINUX_EVDEV
#    include "linux/evdev.h"
#  else
#    error "You can't use EVDEV KEYBOARD without overall EVDEV support."
#  endif
#endif

#include <map>

/*
 * Input Notes
 *
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace Input { /****************************************** Input Namespace */
namespace { /*********************************** Input::<anonymous> Namespace */

	typedef std::map<Input::Keyboard::Key,
	                 Input::Keyboard::Action> KeyState;
	KeyState s_key_state;

} /********************************************* Input::<anonymous> Namespace */

bool
Keyboard::Initialize(void)
{
	bool l_status = true;

#ifdef MARSHMALLOW_EVDEV_KEYBOARD
	l_status &= Input::Linux::EVDEV::InitializeKeyboard();
#endif

	return(l_status);
}

void
Keyboard::Finalize(void)
{
#ifdef MARSHMALLOW_EVDEV_KEYBOARD
	Input::Linux::EVDEV::FinalizeKeyboard();
#endif
	s_key_state.clear();
}

void
Keyboard::Tick(float)
{
#ifdef MARSHMALLOW_EVDEV_KEYBOARD
	Input::Linux::EVDEV::TickKeyboard();
#endif
}

Keyboard::Action
Keyboard::KeyState(Key key)
{
	return(s_key_state[key]);
}

void
Keyboard::SetKeyState(Key key, Action action)
{
	s_key_state[key] = action;
}

} /********************************************************** Input Namespace */
MARSHMALLOW_NAMESPACE_END

