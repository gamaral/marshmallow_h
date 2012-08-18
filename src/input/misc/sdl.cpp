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

#include "sdl.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include "event/eventmanager.h"
#include "event/keyboardevent.h"

#include "../keyboard_p.h"

/*
 * SDL Notes
 *
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace Input { /****************************************** Input Namespace */
namespace Misc { /************************************* Input::Misc Namespace */
namespace { /***************************** Input::Misc::<anonymous> Namespace */

inline bool HandleSDLKeyboardEvent(const SDL_KeyboardEvent &e);

bool
HandleSDLKeyboardEvent(const SDL_KeyboardEvent &e)
{
	Keyboard::Key l_key = Keyboard::KBK_NONE;
	Keyboard::Action l_action =
	    (e.state == SDL_PRESSED ? Keyboard::KeyPressed : Keyboard::KeyReleased);

	if ((e.keysym.sym >= SDLK_a && e.keysym.sym <= SDLK_z)
	    || (e.keysym.sym >= SDLK_0 && e.keysym.sym <= SDLK_9))
		l_key = static_cast<Keyboard::Key>(e.keysym.sym);
	else switch (e.keysym.sym) {
	case SDLK_BACKSPACE:   l_key = Keyboard::KBK_BACKSPACE; break;
	case SDLK_TAB:         l_key = Keyboard::KBK_TAB; break;
	case SDLK_CLEAR:       l_key = Keyboard::KBK_CLEAR; break;
	case SDLK_RETURN:      l_key = Keyboard::KBK_RETURN; break;
	case SDLK_PAUSE:       l_key = Keyboard::KBK_PAUSE; break;
	case SDLK_ESCAPE:      l_key = Keyboard::KBK_ESCAPE; break;

	case SDLK_SPACE:       l_key = Keyboard::KBK_SPACE; break;
	case SDLK_COMMA:       l_key = Keyboard::KBK_COMMA; break;
	case SDLK_MINUS:       l_key = Keyboard::KBK_MINUS; break;
	case SDLK_PERIOD:      l_key = Keyboard::KBK_PERIOD; break;
	case SDLK_SLASH:       l_key = Keyboard::KBK_SLASH; break;

	case SDLK_SEMICOLON:   l_key = Keyboard::KBK_SEMICOLON; break;
	case SDLK_EQUALS:      l_key = Keyboard::KBK_EQUALS; break;
	case SDLK_QUOTE:       l_key = Keyboard::KBK_QUOTE; break;

	case SDLK_LEFTBRACKET:  l_key = Keyboard::KBK_BRACKET_L; break;
	case SDLK_BACKSLASH:    l_key = Keyboard::KBK_BACKSLASH; break;
	case SDLK_RIGHTBRACKET: l_key = Keyboard::KBK_BRACKET_R; break;
	case SDLK_BACKQUOTE:    l_key = Keyboard::KBK_GRAVE; break;

	case SDLK_LSHIFT:      l_key = Keyboard::KBK_SHIFT_L; break;
	case SDLK_RSHIFT:      l_key = Keyboard::KBK_SHIFT_R; break;
	case SDLK_CAPSLOCK:    l_key = Keyboard::KBK_CAPS_LOCK; break;

	case SDLK_LALT:        l_key = Keyboard::KBK_ALT_L; break;
	case SDLK_RALT:        l_key = Keyboard::KBK_ALT_R; break;

	case SDLK_LCTRL:       l_key = Keyboard::KBK_CONTROL_L; break;
	case SDLK_RCTRL:       l_key = Keyboard::KBK_CONTROL_R; break;

	case SDLK_LMETA:       l_key = Keyboard::KBK_META_L; break;
	case SDLK_RMETA:       l_key = Keyboard::KBK_META_R; break;

	case SDLK_LSUPER:      l_key = Keyboard::KBK_SUPER_L; break;
	case SDLK_RSUPER:      l_key = Keyboard::KBK_SUPER_R; break;

	case SDLK_F1:          l_key = Keyboard::KBK_F1; break;
	case SDLK_F2:          l_key = Keyboard::KBK_F2; break;
	case SDLK_F3:          l_key = Keyboard::KBK_F3; break;
	case SDLK_F4:          l_key = Keyboard::KBK_F4; break;
	case SDLK_F5:          l_key = Keyboard::KBK_F5; break;
	case SDLK_F6:          l_key = Keyboard::KBK_F6; break;
	case SDLK_F7:          l_key = Keyboard::KBK_F7; break;
	case SDLK_F8:          l_key = Keyboard::KBK_F8; break;
	case SDLK_F9:          l_key = Keyboard::KBK_F9; break;
	case SDLK_F10:         l_key = Keyboard::KBK_F10; break;
	case SDLK_F11:         l_key = Keyboard::KBK_F11; break;
	case SDLK_F12:         l_key = Keyboard::KBK_F12; break;
	case SDLK_F13:         l_key = Keyboard::KBK_F13; break;
	case SDLK_F14:         l_key = Keyboard::KBK_F14; break;
	case SDLK_F15:         l_key = Keyboard::KBK_F15; break;

	case SDLK_PRINT:       l_key = Keyboard::KBK_PRINT; break;
	case SDLK_SCROLLOCK:  l_key = Keyboard::KBK_SCROLL_LOCK; break;
	case SDLK_BREAK:       l_key = Keyboard::KBK_BREAK; break;

	case SDLK_INSERT:      l_key = Keyboard::KBK_INSERT; break;
	case SDLK_DELETE:      l_key = Keyboard::KBK_DELETE; break;
	case SDLK_HOME:        l_key = Keyboard::KBK_HOME; break;
	case SDLK_END:         l_key = Keyboard::KBK_END; break;
	case SDLK_PAGEUP:      l_key = Keyboard::KBK_PAGE_UP; break;
	case SDLK_PAGEDOWN:    l_key = Keyboard::KBK_PAGE_DOWN; break;

	case SDLK_UP:          l_key = Keyboard::KBK_UP; break;
	case SDLK_DOWN:        l_key = Keyboard::KBK_DOWN; break;
	case SDLK_LEFT:        l_key = Keyboard::KBK_LEFT; break;
	case SDLK_RIGHT:       l_key = Keyboard::KBK_RIGHT; break;

	case SDLK_NUMLOCK:     l_key = Keyboard::KBK_NUM_LOCK; break;
	case SDLK_KP_DIVIDE:   l_key = Keyboard::KBK_KDIVIDE; break;
	case SDLK_KP_MULTIPLY: l_key = Keyboard::KBK_KMULTIPLY; break;
	case SDLK_KP_MINUS:    l_key = Keyboard::KBK_KMINUS; break;
	case SDLK_KP7:         l_key = Keyboard::KBK_K7; break;
	case SDLK_KP8:         l_key = Keyboard::KBK_K8; break;
	case SDLK_KP9:         l_key = Keyboard::KBK_K9; break;
	case SDLK_KP4:         l_key = Keyboard::KBK_K4; break;
	case SDLK_KP5:         l_key = Keyboard::KBK_K5; break;
	case SDLK_KP6:         l_key = Keyboard::KBK_K6; break;
	case SDLK_KP_PLUS:     l_key = Keyboard::KBK_KPLUS; break;
	case SDLK_KP1:         l_key = Keyboard::KBK_K1; break;
	case SDLK_KP2:         l_key = Keyboard::KBK_K2; break;
	case SDLK_KP3:         l_key = Keyboard::KBK_K3; break;
	case SDLK_KP0:         l_key = Keyboard::KBK_K0; break;
	case SDLK_KP_PERIOD:   l_key = Keyboard::KBK_KPERIOD; break;
	case SDLK_KP_ENTER:    l_key = Keyboard::KBK_KENTER; break;

	case SDLK_HELP:        l_key = Keyboard::KBK_HELP; break;
	case SDLK_MENU:        l_key = Keyboard::KBK_MENU; break;
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
SDL::HandleKeyboardEvent(const SDL_KeyboardEvent &e)
{
	return(HandleSDLKeyboardEvent(e));
}

} /**************************************************** Input::Unix Namespace */
} /********************************************************** Input Namespace */
MARSHMALLOW_NAMESPACE_END

