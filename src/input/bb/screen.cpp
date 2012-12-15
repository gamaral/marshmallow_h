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

#include "screen.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"
#include "core/shared.h"

#include "event/eventmanager.h"
#include "event/keyboardevent.h"
#include "event/touchevent.h"

#include "input/keyboard_p.h"

#include <screen/screen.h>

#include <sys/keycodes.h>

/*
 * Screen Notes
 *
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace Input { /****************************************** Input Namespace */
namespace BB { /***************************************** Input::BB Namespace */
namespace { /******************************* Input::BB::<anonymous> Namespace */

inline bool HandleScreenKeyEvent(screen_event_t &e);
inline bool HandleScreenTouchEvent(int type, screen_event_t &e);
inline bool HandleScreenEvents(int type, screen_event_t &e);

bool
HandleScreenKeyEvent(screen_event_t &e)
{
	using namespace Input;

	int l_flags = 0;
        screen_get_event_property_iv(e, SCREEN_PROPERTY_KEY_FLAGS, &l_flags);

        if (KEY_REPEAT == (l_flags & KEY_REPEAT))
		return(false);

	Keyboard::Key l_key = Keyboard::KBK_NONE;
	Keyboard::Action l_action =
	    (l_flags & KEY_DOWN ? Keyboard::KeyPressed : Keyboard::KeyReleased);

	int l_key_code = 0;
	screen_get_event_property_iv(e, SCREEN_PROPERTY_KEY_SYM, &l_key_code);

	/* a-z mapping */
	if (l_key_code >= 'a' && l_key_code <= 'z')
		l_key = static_cast<Keyboard::Key>(l_key_code);

	/* A-Z mapping */
	else if (l_key_code >= 'A' && l_key_code <= 'Z')
		l_key = static_cast<Keyboard::Key>(l_key_code + 32);

	/* 0-9 mapping */
	else if (l_key_code >= '0' && l_key_code <= '9')
		l_key = static_cast<Keyboard::Key>(l_key_code);

	else switch (l_key_code) {
	case KEYCODE_BACKSPACE: l_key = Keyboard::KBK_BACKSPACE; break;
	case KEYCODE_TAB:       l_key = Keyboard::KBK_TAB; break;
	case KEYCODE_RETURN:    l_key = Keyboard::KBK_RETURN; break;
	case KEYCODE_ESCAPE:    l_key = Keyboard::KBK_ESCAPE; break;

	case KEYCODE_SPACE:  l_key = Keyboard::KBK_SPACE; break;
	case KEYCODE_COMMA:  l_key = Keyboard::KBK_COMMA; break;
	case KEYCODE_MENU:   l_key = Keyboard::KBK_MINUS; break;
	case KEYCODE_PERIOD: l_key = Keyboard::KBK_PERIOD; break;
	case KEYCODE_SLASH:  l_key = Keyboard::KBK_SLASH; break;

	case KEYCODE_SEMICOLON:  l_key = Keyboard::KBK_SEMICOLON; break;
	case KEYCODE_EQUAL:      l_key = Keyboard::KBK_EQUAL; break;
	case KEYCODE_APOSTROPHE: l_key = Keyboard::KBK_APOSTROPHE; break;

	case KEYCODE_LEFT_BRACKET:  l_key = Keyboard::KBK_BRACKET_L; break;
	case KEYCODE_RIGHT_BRACKET: l_key = Keyboard::KBK_BRACKET_R; break;
	case KEYCODE_GRAVE:         l_key = Keyboard::KBK_GRAVE; break;

	case KEYCODE_LEFT_SHIFT:  l_key = Keyboard::KBK_SHIFT_L; break;
	case KEYCODE_RIGHT_SHIFT: l_key = Keyboard::KBK_SHIFT_R; break;
	case KEYCODE_CAPS_LOCK:   l_key = Keyboard::KBK_CAPS_LOCK; break;

	case KEYCODE_LEFT_ALT:  l_key = Keyboard::KBK_ALT_L; break;
	case KEYCODE_RIGHT_ALT: l_key = Keyboard::KBK_ALT_R; break;

	case KEYCODE_LEFT_CTRL:  l_key = Keyboard::KBK_CONTROL_L; break;
	case KEYCODE_RIGHT_CTRL: l_key = Keyboard::KBK_CONTROL_R; break;

	case KEYCODE_LEFT_HYPER:  l_key = Keyboard::KBK_META_L; break;
	case KEYCODE_RIGHT_HYPER: l_key = Keyboard::KBK_META_R; break;

	case KEYCODE_F1:  l_key = Keyboard::KBK_F1; break;
	case KEYCODE_F2:  l_key = Keyboard::KBK_F2; break;
	case KEYCODE_F3:  l_key = Keyboard::KBK_F3; break;
	case KEYCODE_F4:  l_key = Keyboard::KBK_F4; break;
	case KEYCODE_F5:  l_key = Keyboard::KBK_F5; break;
	case KEYCODE_F6:  l_key = Keyboard::KBK_F6; break;
	case KEYCODE_F7:  l_key = Keyboard::KBK_F7; break;
	case KEYCODE_F8:  l_key = Keyboard::KBK_F8; break;
	case KEYCODE_F9:  l_key = Keyboard::KBK_F9; break;
	case KEYCODE_F10: l_key = Keyboard::KBK_F10; break;
	case KEYCODE_F11: l_key = Keyboard::KBK_F11; break;
	case KEYCODE_F12: l_key = Keyboard::KBK_F12; break;

	case KEYCODE_PRINT:       l_key = Keyboard::KBK_PRINT; break;
	case KEYCODE_SCROLL_LOCK: l_key = Keyboard::KBK_SCROLL_LOCK; break;
	case KEYCODE_PAUSE:       l_key = Keyboard::KBK_PAUSE; break;
	case KEYCODE_BREAK:       l_key = Keyboard::KBK_BREAK; break;

	case KEYCODE_INSERT:  l_key = Keyboard::KBK_INSERT; break;
	case KEYCODE_DELETE:  l_key = Keyboard::KBK_DELETE; break;
	case KEYCODE_HOME:    l_key = Keyboard::KBK_HOME; break;
	case KEYCODE_END:     l_key = Keyboard::KBK_END; break;
	case KEYCODE_PG_UP:   l_key = Keyboard::KBK_PAGE_UP; break;
	case KEYCODE_PG_DOWN: l_key = Keyboard::KBK_PAGE_DOWN; break;

	case KEYCODE_UP:    l_key = Keyboard::KBK_UP; break;
	case KEYCODE_DOWN:  l_key = Keyboard::KBK_DOWN; break;
	case KEYCODE_LEFT:  l_key = Keyboard::KBK_LEFT; break;
	case KEYCODE_RIGHT: l_key = Keyboard::KBK_RIGHT; break;

	default:
		MMWARNING("Unknown key pressed!");
		return(false);
	}

	const Keyboard::Action l_prev_action = Keyboard::KeyState(l_key);
	if (l_prev_action != l_action) {
		Keyboard::SetKeyState(l_key, l_action);
		Event::EventManager::Instance()->
		    queue(new Event::KeyboardEvent(l_key, l_action, 0));
	}

	return(true);
}

bool
HandleScreenTouchEvent(int type, screen_event_t &e)
{
	Touch::Action l_action;

	switch(type) {
	case SCREEN_EVENT_MTOUCH_TOUCH:   l_action = Touch::Press; break;
	case SCREEN_EVENT_MTOUCH_MOVE:    l_action = Touch::Move; break;
	case SCREEN_EVENT_MTOUCH_RELEASE: l_action = Touch::Release; break;
	default: return(false);
	}

	int l_pos[2];
        screen_get_event_property_iv(e, SCREEN_PROPERTY_SOURCE_POSITION, l_pos);

	Event::EventManager::Instance()->
	    queue(new Event::TouchEvent(l_action, l_pos[0], l_pos[1], 0));

	return(true);
}

bool
HandleScreenEvents(int type, screen_event_t &e)
{
	switch(type) {
	case SCREEN_EVENT_KEYBOARD:
		return(HandleScreenKeyEvent(e));

	case SCREEN_EVENT_MTOUCH_TOUCH:
	case SCREEN_EVENT_MTOUCH_MOVE:
	case SCREEN_EVENT_MTOUCH_RELEASE:
		return(HandleScreenTouchEvent(type, e));

	default:
		MMWARNING("Unknown input event!");
		break;
	}
	return(false);
}

} /***************************************** Input::BB::<anonymous> Namespace */

bool
Screen::HandleEvent(int type, screen_event_t &e)
{
	return(HandleScreenEvents(type, e));
}

} /****************************************************** Input::BB Namespace */
} /********************************************************** Input Namespace */
MARSHMALLOW_NAMESPACE_END

