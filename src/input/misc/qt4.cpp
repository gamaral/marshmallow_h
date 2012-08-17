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

#include "qt4.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include "event/eventmanager.h"
#include "event/keyboardevent.h"

#include "input/keyboard_p.h"

#include <QtGui/QKeyEvent>

/*
 * QT4 Notes
 *
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace Input { /****************************************** Input Namespace */
namespace Misc { /************************************* Input::Misc Namespace */
namespace { /***************************** Input::Misc::<anonymous> Namespace */

inline bool HandleQt4KeyEvent(const QKeyEvent &e, bool down);

bool
HandleQt4KeyEvent(const QKeyEvent &e, bool down)
{
	Keyboard::Key l_key = Keyboard::KBK_NONE;
	Keyboard::Action l_action =
	    (down ? Keyboard::KeyPressed : Keyboard::KeyReleased);

	int keycode = e.key();

	/* force lower case */
	if (keycode >= 65 && keycode <= 90)
		keycode += 32;

	if ((keycode >= 'a' && keycode <= 'z')
	    || (keycode >= '0' && keycode <= '9') )
		l_key = static_cast<Keyboard::Key>(keycode);
	switch (keycode) {
	case Qt::Key_Backspace:    l_key = Keyboard::KBK_BACKSPACE; break;
	case Qt::Key_Tab:          l_key = Keyboard::KBK_TAB; break;
	case Qt::Key_Clear:        l_key = Keyboard::KBK_CLEAR; break;
	case Qt::Key_Return:       l_key = Keyboard::KBK_RETURN; break;
	case Qt::Key_Pause:        l_key = Keyboard::KBK_PAUSE; break;
	case Qt::Key_Escape:       l_key = Keyboard::KBK_ESCAPE; break;

	case Qt::Key_Space:        l_key = Keyboard::KBK_SPACE; break;
	case Qt::Key_Comma:        l_key = Keyboard::KBK_COMMA; break;
	case Qt::Key_Less:         l_key = Keyboard::KBK_MINUS; break;
	case Qt::Key_Period:       l_key = Keyboard::KBK_PERIOD; break;
	case Qt::Key_Slash:        l_key = Keyboard::KBK_SLASH; break;

	case Qt::Key_Semicolon:    l_key = Keyboard::KBK_SEMICOLON; break;
	case Qt::Key_Equal:        l_key = Keyboard::KBK_EQUAL; break;
	case Qt::Key_Apostrophe:   l_key = Keyboard::KBK_APOSTROPHE; break;

	case Qt::Key_BraceLeft:    l_key = Keyboard::KBK_BRACKET_L; break;
	case Qt::Key_Backslash:    l_key = Keyboard::KBK_BACKSLASH; break;
	case Qt::Key_BraceRight:   l_key = Keyboard::KBK_BRACKET_R; break;
	case Qt::Key_acute:        l_key = Keyboard::KBK_GRAVE; break;

	case Qt::Key_Shift:        l_key = Keyboard::KBK_SHIFT_L; break;
	case Qt::Key_CapsLock:     l_key = Keyboard::KBK_CAPS_LOCK; break;

	case Qt::Key_Alt:          l_key = Keyboard::KBK_ALT_L; break;

	case Qt::Key_Control:      l_key = Keyboard::KBK_CONTROL_L; break;

	case Qt::Key_Meta:         l_key = Keyboard::KBK_META_L; break;

	case Qt::Key_Super_L:      l_key = Keyboard::KBK_SUPER_L; break;
	case Qt::Key_Super_R:      l_key = Keyboard::KBK_SUPER_R; break;

	case Qt::Key_F1:           l_key = Keyboard::KBK_F1; break;
	case Qt::Key_F2:           l_key = Keyboard::KBK_F2; break;
	case Qt::Key_F3:           l_key = Keyboard::KBK_F3; break;
	case Qt::Key_F4:           l_key = Keyboard::KBK_F4; break;
	case Qt::Key_F5:           l_key = Keyboard::KBK_F5; break;
	case Qt::Key_F6:           l_key = Keyboard::KBK_F6; break;
	case Qt::Key_F7:           l_key = Keyboard::KBK_F7; break;
	case Qt::Key_F8:           l_key = Keyboard::KBK_F8; break;
	case Qt::Key_F9:           l_key = Keyboard::KBK_F9; break;
	case Qt::Key_F10:          l_key = Keyboard::KBK_F10; break;
	case Qt::Key_F11:          l_key = Keyboard::KBK_F11; break;
	case Qt::Key_F12:          l_key = Keyboard::KBK_F12; break;
	case Qt::Key_F13:          l_key = Keyboard::KBK_F13; break;
	case Qt::Key_F14:          l_key = Keyboard::KBK_F14; break;
	case Qt::Key_F15:          l_key = Keyboard::KBK_F15; break;

	case Qt::Key_Print:        l_key = Keyboard::KBK_PRINT; break;
	case Qt::Key_ScrollLock:   l_key = Keyboard::KBK_SCROLL_LOCK; break;

	case Qt::Key_Insert:       l_key = Keyboard::KBK_INSERT; break;
	case Qt::Key_Delete:       l_key = Keyboard::KBK_DELETE; break;
	case Qt::Key_Home:         l_key = Keyboard::KBK_HOME; break;
	case Qt::Key_End:          l_key = Keyboard::KBK_END; break;
	case Qt::Key_PageUp:       l_key = Keyboard::KBK_PAGE_UP; break;
	case Qt::Key_PageDown:     l_key = Keyboard::KBK_PAGE_DOWN; break;

	case Qt::Key_Up:           l_key = Keyboard::KBK_UP; break;
	case Qt::Key_Down:         l_key = Keyboard::KBK_DOWN; break;
	case Qt::Key_Left:         l_key = Keyboard::KBK_LEFT; break;
	case Qt::Key_Right:        l_key = Keyboard::KBK_RIGHT; break;

	case Qt::Key_Close:        l_key = Keyboard::KBK_CLOSE; break;
	case Qt::Key_Help:         l_key = Keyboard::KBK_HELP; break;
	case Qt::Key_Menu:         l_key = Keyboard::KBK_MENU; break;

	default:
		MMWARNING("Unknown key pressed!");
		return(false);
	}

	const Keyboard::Action l_prev_action = Keyboard::KeyState(l_key);
	if (l_prev_action != l_action) {
		Keyboard::SetKeyState(l_key, l_action);
		Event::SharedEvent event(new Event::KeyboardEvent(l_key, l_action, 0));
		Event::EventManager::Instance()->queue(event);
	}

	return(true);
}

} /*************************************** Input::Misc::<anonymous> Namespace */

bool
Qt4::HandleKeyboardEvent(const QKeyEvent &e, bool down)
{
	return(HandleQt4KeyEvent(e, down));
}

} /**************************************************** Input::Unix Namespace */
} /********************************************************** Input Namespace */
MARSHMALLOW_NAMESPACE_END

