/*
 * Copyright 2011 Marshmallow Engine. All rights reserved.
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

#ifndef EVENT_KEYBOARDEVENT_H
#define EVENT_KEYBOARDEVENT_H 1

#include "eventbase.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Event
{

	enum KBActions
	{
		KeyReleased     = 0,
		KeyPressed      = 1
	};

	enum KBKeys
	{
		KEY_NONE        = 0,
		KEY_ALT_L,
		KEY_ALT_R,
		KEY_BACKSPACE,
		KEY_BREAK,
		KEY_CAPS_LOCK,
		KEY_CLEAR,
		KEY_CONTROL_L,
		KEY_CONTROL_R,
		KEY_DELETE,
		KEY_DOWN,
		KEY_END,
		KEY_ESCAPE,
		KEY_F1,
		KEY_F10,
		KEY_F11,
		KEY_F12,
		KEY_F13,
		KEY_F14,
		KEY_F15,
		KEY_F2,
		KEY_F3,
		KEY_F4,
		KEY_F5,
		KEY_F6,
		KEY_F7,
		KEY_F8,
		KEY_F9,
		KEY_HELP,
		KEY_HOME,
		KEY_INSERT,
		KEY_K0,
		KEY_K1,
		KEY_K2,
		KEY_K3,
		KEY_K4,
		KEY_K5,
		KEY_K6,
		KEY_K7,
		KEY_K8,
		KEY_K9,
		KEY_KDECIMAL,
		KEY_KDIVIDE,
		KEY_KMULTIPLY,
		KEY_LEFT,
		KEY_LESS,
		KEY_MENU,
		KEY_META_L,
		KEY_META_R,
		KEY_NUM_LOCK,
		KEY_PAGE_DOWN,
		KEY_PAGE_UP,
		KEY_PAUSE,
		KEY_PRINT,
		KEY_RIGHT,
		KEY_SCROLL_LOCK,
		KEY_SHIFT_L,
		KEY_SHIFT_R,
		KEY_TAB,
		KEY_UP,
		KEY_BACKSLASH    = '/',
		KEY_BRACKETLEFT = '{',
		KEY_BRACKETRIGHT = '}',
		KEY_COMMA       = ',',
		KEY_DBLQUOTE    = '"',
		KEY_EQUAL       = '=',
		KEY_GRAVE       = '`',
		KEY_MINUS       = '-',
		KEY_PERIOD      = '.',
		KEY_PLUS        = '+',
		KEY_RETURN      = '\n',
		KEY_SEMICOLON   = ';',
		KEY_SLASH       = '\\',
		KEY_SPACE       = ' ',
		KEY_0           = '0',
		KEY_1           = '1',
		KEY_2           = '2',
		KEY_3           = '3',
		KEY_4           = '4',
		KEY_5           = '5',
		KEY_6           = '6',
		KEY_7           = '7',
		KEY_8           = '8',
		KEY_9           = '9',
		KEY_A           = 'a',
		KEY_B           = 'b',
		KEY_C           = 'c',
		KEY_D           = 'd',
		KEY_E           = 'e',
		KEY_F           = 'f',
		KEY_G           = 'g',
		KEY_H           = 'h',
		KEY_I           = 'i',
		KEY_J           = 'j',
		KEY_K           = 'k',
		KEY_L           = 'l',
		KEY_M           = 'm',
		KEY_N           = 'n',
		KEY_O           = 'o',
		KEY_P           = 'p',
		KEY_Q           = 'q',
		KEY_R           = 'r',
		KEY_S           = 's',
		KEY_T           = 't',
		KEY_U           = 'u',
		KEY_V           = 'v',
		KEY_W           = 'w',
		KEY_X           = 'x',
		KEY_Y           = 'y',
		KEY_Z           = 'z'
	};

	/*! @brief Keyboard Event Class */
	class EVENT_EXPORT KeyboardEvent : public EventBase
	{
		KBKeys m_key;
		KBActions m_action;
	public:
		KeyboardEvent(KBKeys key, KBActions action, TIME timeout = 0);
		virtual ~KeyboardEvent(void);

	public: /* virtual */

		VIRTUAL const Core::Type & type(void) const
		    { return(Type); }

	public: /* static */

		static const Core::Type Type;
	};

}

MARSHMALLOW_NAMESPACE_END

#endif
