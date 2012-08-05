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

#include "input/unix/x11.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include "event/eventmanager.h"
#include "event/keyboardevent.h"

#include "input/keyboard_p.h"

#include <X11/XKBlib.h>
#include <X11/Xutil.h>

/*
 * X11 Notes
 *
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace { /******************************************** Anonymous Namespace */

const Core::Identifier s_x11_input_source("X11");

inline bool HandleX11KeyEvent(XKeyEvent &key);
inline bool HandleX11Messages(XEvent &e);

bool
HandleX11KeyEvent(XKeyEvent &key)
{
	using namespace Input;

	Keyboard::Key l_key = Keyboard::KBK_NONE;
	Keyboard::Action l_action =
	    (key.type == KeyPress ? Keyboard::KeyPressed : Keyboard::KeyReleased);

	unsigned long l_key_code = XLookupKeysym(&key, 0);

	/* a-z mapping */
	if (l_key_code >= 'a' && l_key_code <= 'z')
		l_key = static_cast<Keyboard::Key>(l_key_code - 32);

	/* A-Z mapping */
	else if (l_key_code >= 'A' && l_key_code <= 'Z')
		l_key = static_cast<Keyboard::Key>(l_key_code);

	/* 0-9 mapping */
	else if (l_key_code >= '0' && l_key_code <= '9')
		l_key = static_cast<Keyboard::Key>(l_key_code);

	else switch (l_key_code) {
	case XK_BackSpace:    l_key = Keyboard::KBK_BACKSPACE; break;
	case XK_Tab:          l_key = Keyboard::KBK_TAB; break;
	case XK_Return:       l_key = Keyboard::KBK_RETURN; break;
	case XK_Escape:       l_key = Keyboard::KBK_ESCAPE; break;

	case XK_space:        l_key = Keyboard::KBK_SPACE; break;
	case XK_comma:        l_key = Keyboard::KBK_COMMA; break;
	case XK_minus:        l_key = Keyboard::KBK_MINUS; break;
	case XK_period:       l_key = Keyboard::KBK_PERIOD; break;
	case XK_slash:        l_key = Keyboard::KBK_SLASH; break;

	case XK_semicolon:    l_key = Keyboard::KBK_SEMICOLON; break;
	case XK_equal:        l_key = Keyboard::KBK_EQUAL; break;

	case XK_bracketleft:  l_key = Keyboard::KBK_BRACKET_LEFT; break;
	case XK_backslash:    l_key = Keyboard::KBK_BACKSPACE; break;
	case XK_bracketright: l_key = Keyboard::KBK_BRACKET_RIGHT; break;
	case XK_grave:        l_key = Keyboard::KBK_GRAVE; break;

	case XK_Shift_L:      l_key = Keyboard::KBK_SHIFT_L; break;
	case XK_Shift_R:      l_key = Keyboard::KBK_SHIFT_R; break;
	case XK_Caps_Lock:    l_key = Keyboard::KBK_CAPS_LOCK; break;

	case XK_Alt_L:        l_key = Keyboard::KBK_ALT_L; break;
	case XK_Alt_R:        l_key = Keyboard::KBK_ALT_R; break;

	case XK_Control_L:    l_key = Keyboard::KBK_CONTROL_R; break;
	case XK_Control_R:    l_key = Keyboard::KBK_CONTROL_L; break;

	case XK_Meta_L:       l_key = Keyboard::KBK_META_L; break;
	case XK_Meta_R:       l_key = Keyboard::KBK_META_R; break;

	case XK_F1:           l_key = Keyboard::KBK_F1; break;
	case XK_F2:           l_key = Keyboard::KBK_F2; break;
	case XK_F3:           l_key = Keyboard::KBK_F3; break;
	case XK_F4:           l_key = Keyboard::KBK_F4; break;
	case XK_F5:           l_key = Keyboard::KBK_F5; break;
	case XK_F6:           l_key = Keyboard::KBK_F6; break;
	case XK_F7:           l_key = Keyboard::KBK_F7; break;
	case XK_F8:           l_key = Keyboard::KBK_F8; break;
	case XK_F9:           l_key = Keyboard::KBK_F9; break;
	case XK_F10:          l_key = Keyboard::KBK_F10; break;
	case XK_F11:          l_key = Keyboard::KBK_F11; break;
	case XK_F12:          l_key = Keyboard::KBK_F12; break;
	case XK_F13:          l_key = Keyboard::KBK_F13; break;
	case XK_F14:          l_key = Keyboard::KBK_F14; break;
	case XK_F15:          l_key = Keyboard::KBK_F15; break;

	case XK_Print:        l_key = Keyboard::KBK_PRINT; break;
	case XK_Scroll_Lock:  l_key = Keyboard::KBK_SCROLL_LOCK; break;
	case XK_Pause:        l_key = Keyboard::KBK_PAUSE; break;
	case XK_Break:        l_key = Keyboard::KBK_BREAK; break;

	case XK_Insert:       l_key = Keyboard::KBK_INSERT; break;
	case XK_Delete:       l_key = Keyboard::KBK_DELETE; break;
	case XK_Home:         l_key = Keyboard::KBK_HOME; break;
	case XK_End:          l_key = Keyboard::KBK_END; break;
	case XK_Page_Up:      l_key = Keyboard::KBK_PAGE_UP; break;
	case XK_Page_Down:    l_key = Keyboard::KBK_PAGE_DOWN; break;

	case XK_Up:           l_key = Keyboard::KBK_UP; break;
	case XK_Down:         l_key = Keyboard::KBK_DOWN; break;
	case XK_Left:         l_key = Keyboard::KBK_LEFT; break;
	case XK_Right:        l_key = Keyboard::KBK_RIGHT; break;

	case XK_Num_Lock:     l_key = Keyboard::KBK_NUM_LOCK; break;
	case XK_KP_Divide:    l_key = Keyboard::KBK_KDIVIDE; break;
	case XK_KP_Multiply:  l_key = Keyboard::KBK_KMULTIPLY; break;
	case XK_KP_Subtract:  l_key = Keyboard::KBK_KSUBTRACT; break;
	case XK_KP_7:         l_key = Keyboard::KBK_K7; break;
	case XK_KP_8:         l_key = Keyboard::KBK_K8; break;
	case XK_KP_9:         l_key = Keyboard::KBK_K9; break;
	case XK_KP_4:         l_key = Keyboard::KBK_K4; break;
	case XK_KP_5:         l_key = Keyboard::KBK_K5; break;
	case XK_KP_6:         l_key = Keyboard::KBK_K6; break;
	case XK_KP_Add:       l_key = Keyboard::KBK_KADD; break;
	case XK_KP_1:         l_key = Keyboard::KBK_K1; break;
	case XK_KP_2:         l_key = Keyboard::KBK_K2; break;
	case XK_KP_3:         l_key = Keyboard::KBK_K3; break;
	case XK_KP_0:         l_key = Keyboard::KBK_K0; break;
	case XK_KP_Decimal:   l_key = Keyboard::KBK_KDECIMAL; break;
	case XK_KP_Enter:     l_key = Keyboard::KBK_KENTER; break;

	case XK_Clear:        l_key = Keyboard::KBK_CLEAR; break;
	case XK_Help:         l_key = Keyboard::KBK_HELP; break;
	case XK_Menu:         l_key = Keyboard::KBK_MENU; break;

	default:
		MMWARNING("Unknown key pressed!");
		return(false);
	}

	const Keyboard::Action l_prev_action = Keyboard::KeyState(l_key);
	if (l_prev_action != l_action) {
		Keyboard::SetKeyState(l_key, l_action);
		Event::SharedEvent event(new Event::KeyboardEvent(l_key, l_action, s_x11_input_source));
		Event::EventManager::Instance()->queue(event);
	}

	return(true);
}

bool
HandleX11Messages(XEvent &e)
{
	switch(e.type) {
	case KeyPress:
	case KeyRelease:
		return(HandleX11KeyEvent(e.xkey));
		break;

	case ButtonPress:
	case ButtonRelease:
		/* TODO: Handle event type */
	break;

	case MotionNotify:
		/* TODO: Handle event type */
		break;

	default:
		MMWARNING("Unknown input event!");
		break;
	}
	return(false);
}

} /****************************************************** Anonymous Namespace */

namespace Input { /****************************************** Input Namespace */
namespace Unix { /************************************* Input::Unix Namespace */

void
X11::InitializeKeyboard(Display *xdpy)
{
	XkbSetDetectableAutoRepeat(xdpy, true, 0);
}

bool
X11::HandleEvent(XEvent &e)
{
	return(HandleX11Messages(e));
}

} /**************************************************** Input::Unix Namespace */
} /********************************************************** Input Namespace */
MARSHMALLOW_NAMESPACE_END

