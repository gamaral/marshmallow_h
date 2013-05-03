/*
 * Copyright (c) 2012-2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
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

#include "evdev_keyboarddevice.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include "event/eventmanager.h"
#include "event/keyboardevent.h"

#include <linux/input.h>

#include "../keyboard_p.h"

/*
 * EVDEV KeyboardDevice Notes
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace Input { /****************************************** Input Namespace */
namespace Linux { /*********************************** Input::Linux Namespace */
namespace EVDEV { /**************************** Input::Linux::EVDEV Namespace */

KeyboardDevice::KeyboardDevice(int fd_, Type type_, uint16_t vendor,
    uint16_t product)
    : EventDevice(fd_, type_)
{
	MMDEBUG("Keyboard slot id " << id() << " assigned.");

	Map::PopulateEventCodes(vendor, product, name(),
	    type(), EV_KEY, m_key_map);
}

bool
KeyboardDevice::handleEvent(struct input_event &event)
{
	using namespace Event;

	Keyboard::Key l_key = Keyboard::KBK_NONE;
	Keyboard::Action l_action;

	Map::EventCodes::const_iterator l_entry =
	    m_key_map.find(event.code);

	/* per-device mapping */
	if (l_entry != m_key_map.end())
		l_key = static_cast<Keyboard::Key>(l_entry->second);

	/* default mapping */
	if (l_key == Keyboard::KBK_NONE)
		switch (event.code) {
		case KEY_BACKSPACE:   l_key = Keyboard::KBK_BACKSPACE; break;
		case KEY_TAB:         l_key = Keyboard::KBK_TAB; break;
		case KEY_CLEAR:       l_key = Keyboard::KBK_CLEAR; break;
		case KEY_ENTER:       l_key = Keyboard::KBK_ENTER; break;
		case KEY_PAUSE:       l_key = Keyboard::KBK_PAUSE; break;
		case KEY_ESC:         l_key = Keyboard::KBK_ESCAPE; break;

		case KEY_SPACE:       l_key = Keyboard::KBK_SPACE; break;
		case KEY_COMMA:       l_key = Keyboard::KBK_COMMA; break;
		case KEY_MINUS:       l_key = Keyboard::KBK_MINUS; break;
		case KEY_DOT:         l_key = Keyboard::KBK_DOT; break;
		case KEY_SLASH:       l_key = Keyboard::KBK_SLASH; break;

		case KEY_0:           l_key = Keyboard::KBK_0; break;
		case KEY_1:           l_key = Keyboard::KBK_1; break;
		case KEY_2:           l_key = Keyboard::KBK_2; break;
		case KEY_3:           l_key = Keyboard::KBK_3; break;
		case KEY_4:           l_key = Keyboard::KBK_4; break;
		case KEY_5:           l_key = Keyboard::KBK_5; break;
		case KEY_6:           l_key = Keyboard::KBK_6; break;
		case KEY_7:           l_key = Keyboard::KBK_7; break;
		case KEY_8:           l_key = Keyboard::KBK_8; break;
		case KEY_9:           l_key = Keyboard::KBK_9; break;

		case KEY_SEMICOLON:   l_key = Keyboard::KBK_SEMICOLON; break;
		case KEY_EQUAL:       l_key = Keyboard::KBK_EQUAL; break;

		case KEY_A:           l_key = Keyboard::KBK_A; break;
		case KEY_B:           l_key = Keyboard::KBK_B; break;
		case KEY_C:           l_key = Keyboard::KBK_C; break;
		case KEY_D:           l_key = Keyboard::KBK_D; break;
		case KEY_E:           l_key = Keyboard::KBK_E; break;
		case KEY_F:           l_key = Keyboard::KBK_F; break;
		case KEY_G:           l_key = Keyboard::KBK_G; break;
		case KEY_H:           l_key = Keyboard::KBK_H; break;
		case KEY_I:           l_key = Keyboard::KBK_I; break;
		case KEY_J:           l_key = Keyboard::KBK_J; break;
		case KEY_K:           l_key = Keyboard::KBK_K; break;
		case KEY_L:           l_key = Keyboard::KBK_L; break;
		case KEY_M:           l_key = Keyboard::KBK_M; break;
		case KEY_N:           l_key = Keyboard::KBK_N; break;
		case KEY_O:           l_key = Keyboard::KBK_O; break;
		case KEY_P:           l_key = Keyboard::KBK_P; break;
		case KEY_Q:           l_key = Keyboard::KBK_Q; break;
		case KEY_R:           l_key = Keyboard::KBK_R; break;
		case KEY_S:           l_key = Keyboard::KBK_S; break;
		case KEY_T:           l_key = Keyboard::KBK_T; break;
		case KEY_U:           l_key = Keyboard::KBK_U; break;
		case KEY_V:           l_key = Keyboard::KBK_V; break;
		case KEY_W:           l_key = Keyboard::KBK_W; break;
		case KEY_X:           l_key = Keyboard::KBK_X; break;
		case KEY_Y:           l_key = Keyboard::KBK_Y; break;
		case KEY_Z:           l_key = Keyboard::KBK_Z; break;

		case KEY_LEFTBRACE:   l_key = Keyboard::KBK_BRACKET_L; break;
		case KEY_BACKSLASH:   l_key = Keyboard::KBK_BACKSLASH; break;
		case KEY_RIGHTBRACE:  l_key = Keyboard::KBK_BRACKET_R; break;
		case KEY_GRAVE:       l_key = Keyboard::KBK_GRAVE; break;

		case KEY_LEFTSHIFT:   l_key = Keyboard::KBK_SHIFT_L; break;
		case KEY_RIGHTSHIFT:  l_key = Keyboard::KBK_SHIFT_R; break;
		case KEY_CAPSLOCK:    l_key = Keyboard::KBK_CAPS_LOCK; break;

		case KEY_LEFTALT:     l_key = Keyboard::KBK_ALT_L; break;
		case KEY_RIGHTALT:    l_key = Keyboard::KBK_ALT_R; break;

		case KEY_LEFTCTRL:    l_key = Keyboard::KBK_CONTROL_L; break;
		case KEY_RIGHTCTRL:   l_key = Keyboard::KBK_CONTROL_R; break;

		case KEY_LEFTMETA:    l_key = Keyboard::KBK_META_L; break;
		case KEY_RIGHTMETA:   l_key = Keyboard::KBK_META_R; break;

		case KEY_F1:          l_key = Keyboard::KBK_F1; break;
		case KEY_F2:          l_key = Keyboard::KBK_F2; break;
		case KEY_F3:          l_key = Keyboard::KBK_F3; break;
		case KEY_F4:          l_key = Keyboard::KBK_F4; break;
		case KEY_F5:          l_key = Keyboard::KBK_F5; break;
		case KEY_F6:          l_key = Keyboard::KBK_F6; break;
		case KEY_F7:          l_key = Keyboard::KBK_F7; break;
		case KEY_F8:          l_key = Keyboard::KBK_F8; break;
		case KEY_F9:          l_key = Keyboard::KBK_F9; break;
		case KEY_F10:         l_key = Keyboard::KBK_F10; break;
		case KEY_F11:         l_key = Keyboard::KBK_F11; break;
		case KEY_F12:         l_key = Keyboard::KBK_F12; break;
		case KEY_F13:         l_key = Keyboard::KBK_F13; break;
		case KEY_F14:         l_key = Keyboard::KBK_F14; break;
		case KEY_F15:         l_key = Keyboard::KBK_F15; break;

		case KEY_PRINT:       l_key = Keyboard::KBK_PRINT; break;
		case KEY_SCROLLLOCK:  l_key = Keyboard::KBK_SCROLL_LOCK; break;
		case KEY_BREAK:       l_key = Keyboard::KBK_BREAK; break;

		case KEY_INSERT:      l_key = Keyboard::KBK_INSERT; break;
		case KEY_DELETE:      l_key = Keyboard::KBK_DELETE; break;
		case KEY_HOME:        l_key = Keyboard::KBK_HOME; break;
		case KEY_END:         l_key = Keyboard::KBK_END; break;
		case KEY_PAGEUP:      l_key = Keyboard::KBK_PAGE_UP; break;
		case KEY_PAGEDOWN:    l_key = Keyboard::KBK_PAGE_DOWN; break;

		case KEY_UP:          l_key = Keyboard::KBK_UP; break;
		case KEY_DOWN:        l_key = Keyboard::KBK_DOWN; break;
		case KEY_LEFT:        l_key = Keyboard::KBK_LEFT; break;
		case KEY_RIGHT:       l_key = Keyboard::KBK_RIGHT; break;

		case KEY_NUMLOCK:     l_key = Keyboard::KBK_NUM_LOCK; break;
		case KEY_KPSLASH:     l_key = Keyboard::KBK_KSLASH; break;
		case KEY_KPASTERISK:  l_key = Keyboard::KBK_KASTERISK; break;
		case KEY_KPMINUS:     l_key = Keyboard::KBK_KMINUS; break;
		case KEY_KP7:         l_key = Keyboard::KBK_K7; break;
		case KEY_KP8:         l_key = Keyboard::KBK_K8; break;
		case KEY_KP9:         l_key = Keyboard::KBK_K9; break;
		case KEY_KP4:         l_key = Keyboard::KBK_K4; break;
		case KEY_KP5:         l_key = Keyboard::KBK_K5; break;
		case KEY_KP6:         l_key = Keyboard::KBK_K6; break;
		case KEY_KPPLUS:      l_key = Keyboard::KBK_KPLUS; break;
		case KEY_KP1:         l_key = Keyboard::KBK_K1; break;
		case KEY_KP2:         l_key = Keyboard::KBK_K2; break;
		case KEY_KP3:         l_key = Keyboard::KBK_K3; break;
		case KEY_KP0:         l_key = Keyboard::KBK_K0; break;
		case KEY_KPDOT:       l_key = Keyboard::KBK_KDOT; break;
		case KEY_KPENTER:     l_key = Keyboard::KBK_KENTER; break;

		case KEY_CLOSE:       l_key = Keyboard::KBK_CLOSE; break;
		case KEY_HELP:        l_key = Keyboard::KBK_HELP; break;
		case KEY_MENU:        l_key = Keyboard::KBK_MENU; break;

		default:
			MMDEBUG("Unknown key code: " << event.code
			    << " name[" << name() << "]"
			    << " id["   << id()   << "]");
			return(false);
		}

	/* translate action */
	switch(event.value) {
	case 0: l_action = Keyboard::KeyReleased; break;
	case 1: l_action = Keyboard::KeyPressed; break;
	default: return(false);
	}

	const Keyboard::Action l_prev_action = Keyboard::KeyState(l_key);
	if (l_prev_action != l_action) {
		Keyboard::SetKeyState(l_key, l_action);

		IEvent *l_event(new KeyboardEvent(l_key, l_action, id()));
		EventManager::Instance()->queue(l_event);
	}

	return(true);
}

} /******************************************** Input::Linux::EVDEV Namespace */
} /*************************************************** Input::Linux Namespace */
} /********************************************************** Input Namespace */
MARSHMALLOW_NAMESPACE_END

