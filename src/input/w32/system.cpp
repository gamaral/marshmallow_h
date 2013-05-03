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

#include "system.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include "event/eventmanager.h"
#include "event/keyboardevent.h"

#include "input/keyboard_p.h"

/*
 * W32 System Notes
 *
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace Input { /****************************************** Input Namespace */
namespace W32 { /*************************************** Input::W32 Namespace */
namespace { /****************************** Input::W32::<anonymous> Namespace */

inline bool HandleW32KeyboardEvent(int keycode, bool down);
inline bool HandleW32Event(UINT uMsg, WPARAM wParam, LPARAM lParam);

bool
HandleW32KeyboardEvent(int keycode, bool down)
{
	Keyboard::Key l_key = Keyboard::KBK_NONE;
	Keyboard::Action l_action =
	    (down ? Keyboard::KeyPressed : Keyboard::KeyReleased);

	/* A-Z mapping */
	if (keycode >= 'A' && keycode <= 'Z')
		l_key = static_cast<Keyboard::Key>(keycode);

	/* 0-9 mapping */
	else if (keycode >= '0' && keycode <= '9')
		l_key = static_cast<Keyboard::Key>(keycode);

	else switch (keycode) {
	case VK_BACK:         l_key = Keyboard::KBK_BACKSPACE; break;
	case VK_TAB:          l_key = Keyboard::KBK_TAB; break;
	case VK_RETURN:       l_key = Keyboard::KBK_RETURN; break;
	case VK_ESCAPE:       l_key = Keyboard::KBK_ESCAPE; break;

	case VK_SPACE:        l_key = Keyboard::KBK_SPACE; break;
	case VK_OEM_COMMA:    l_key = Keyboard::KBK_COMMA; break;
	case VK_OEM_MINUS:    l_key = Keyboard::KBK_MINUS; break;
	case VK_OEM_PERIOD:   l_key = Keyboard::KBK_PERIOD; break;

	case VK_SHIFT:
	case VK_LSHIFT:       l_key = Keyboard::KBK_SHIFT_L; break;
	case VK_RSHIFT:       l_key = Keyboard::KBK_SHIFT_R; break;
	case VK_CAPITAL:      l_key = Keyboard::KBK_CAPS_LOCK; break;

	case VK_LMENU:        l_key = Keyboard::KBK_ALT_L; break;
	case VK_RMENU:        l_key = Keyboard::KBK_ALT_R; break;

	case VK_LCONTROL:     l_key = Keyboard::KBK_CONTROL_L; break;
	case VK_RCONTROL:     l_key = Keyboard::KBK_CONTROL_R; break;

	case VK_F1:           l_key = Keyboard::KBK_F1; break;
	case VK_F2:           l_key = Keyboard::KBK_F2; break;
	case VK_F3:           l_key = Keyboard::KBK_F3; break;
	case VK_F4:           l_key = Keyboard::KBK_F4; break;
	case VK_F5:           l_key = Keyboard::KBK_F5; break;
	case VK_F6:           l_key = Keyboard::KBK_F6; break;
	case VK_F7:           l_key = Keyboard::KBK_F7; break;
	case VK_F8:           l_key = Keyboard::KBK_F8; break;
	case VK_F9:           l_key = Keyboard::KBK_F9; break;
	case VK_F10:          l_key = Keyboard::KBK_F10; break;
	case VK_F11:          l_key = Keyboard::KBK_F11; break;
	case VK_F12:          l_key = Keyboard::KBK_F12; break;
	case VK_F13:          l_key = Keyboard::KBK_F13; break;
	case VK_F14:          l_key = Keyboard::KBK_F14; break;
	case VK_F15:          l_key = Keyboard::KBK_F15; break;

	case VK_PRINT:        l_key = Keyboard::KBK_PRINT; break;
	case VK_SCROLL:       l_key = Keyboard::KBK_SCROLL_LOCK; break;
	case VK_PAUSE:        l_key = Keyboard::KBK_PAUSE; break;

	case VK_INSERT:       l_key = Keyboard::KBK_INSERT; break;
	case VK_DELETE:       l_key = Keyboard::KBK_DELETE; break;
	case VK_HOME:         l_key = Keyboard::KBK_HOME; break;
	case VK_END:          l_key = Keyboard::KBK_END; break;
	case VK_PRIOR:        l_key = Keyboard::KBK_PAGE_UP; break;
	case VK_NEXT:         l_key = Keyboard::KBK_PAGE_DOWN; break;

	case VK_UP:           l_key = Keyboard::KBK_UP; break;
	case VK_DOWN:         l_key = Keyboard::KBK_DOWN; break;
	case VK_LEFT:         l_key = Keyboard::KBK_LEFT; break;
	case VK_RIGHT:        l_key = Keyboard::KBK_RIGHT; break;

	case VK_NUMLOCK:      l_key = Keyboard::KBK_NUM_LOCK; break;
	case VK_DIVIDE:       l_key = Keyboard::KBK_KDIVIDE; break;
	case VK_MULTIPLY:     l_key = Keyboard::KBK_KMULTIPLY; break;
	case VK_SUBTRACT:     l_key = Keyboard::KBK_KSUBTRACT; break;
	case VK_NUMPAD7:      l_key = Keyboard::KBK_K7; break;
	case VK_NUMPAD8:      l_key = Keyboard::KBK_K8; break;
	case VK_NUMPAD9:      l_key = Keyboard::KBK_K9; break;
	case VK_NUMPAD4:      l_key = Keyboard::KBK_K4; break;
	case VK_NUMPAD5:      l_key = Keyboard::KBK_K5; break;
	case VK_NUMPAD6:      l_key = Keyboard::KBK_K6; break;
	case VK_ADD:          l_key = Keyboard::KBK_KADD; break;
	case VK_NUMPAD1:      l_key = Keyboard::KBK_K1; break;
	case VK_NUMPAD2:      l_key = Keyboard::KBK_K2; break;
	case VK_NUMPAD3:      l_key = Keyboard::KBK_K3; break;
	case VK_NUMPAD0:      l_key = Keyboard::KBK_K0; break;

	case VK_CLEAR:        l_key = Keyboard::KBK_CLEAR; break;
	case VK_HELP:         l_key = Keyboard::KBK_HELP; break;
	case VK_MENU:         l_key = Keyboard::KBK_MENU; break;

	default:
		MMWARNING("Unknown key pressed! KEY=" << keycode);
		return(false);
	}

	const Keyboard::Action l_prev_action = Keyboard::KeyState(l_key);
	if (l_prev_action != l_action) {
		Keyboard::SetKeyState(l_key, l_action);
		Event::IEvent *event(new Event::KeyboardEvent(l_key, l_action, 0));
		Event::EventManager::Instance()->queue(event);
	}

	return(true);
}

bool
HandleW32Event(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {

	case WM_KEYDOWN:
		return(HandleW32KeyboardEvent(static_cast<int>(wParam), true));

	case WM_KEYUP:
		return(HandleW32KeyboardEvent(static_cast<int>(wParam), false));

	}

	return(false);
}

} /**************************************** Input::W32::<anonymous> Namespace */

bool
System::HandleEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return(HandleW32Event(uMsg, wParam, lParam));
}

} /***************************************************** Input::W32 Namespace */
} /********************************************************** Input Namespace */
MARSHMALLOW_NAMESPACE_END

