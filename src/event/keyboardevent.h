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
		KEY_NONE         = 0,
		KEY_ALT_L        = 64,
		KEY_ALT_R        = 108,
		KEY_BACKSPACE    = 22,
		KEY_BREAK,
		KEY_CAPS_LOCK    = 66,
		KEY_CLEAR,
		KEY_CONTROL_L    = 37,
		KEY_CONTROL_R    = 105,
		KEY_DELETE       = 119,
		KEY_DOWN         = 116,
		KEY_END          = 115,
		KEY_ESCAPE       = 9,
		KEY_F1           = 67,
		KEY_F2           = 68,
		KEY_F3           = 69,
		KEY_F4           = 70,
		KEY_F5           = 71,
		KEY_F6           = 72,
		KEY_F7           = 73,
		KEY_F8           = 74,
		KEY_F9           = 75,
		KEY_F10          = 76,
		KEY_F11          = 95,
		KEY_F12          = 96,
		KEY_F13          = 98,
		KEY_F14          = 99,
		KEY_F15          = 100,
		KEY_HELP,
		KEY_HOME         = 110,
		KEY_INSERT       = 108,
		KEY_K0           = 90,
		KEY_K1           = 87,
		KEY_K2           = 88,
		KEY_K3           = 89,
		KEY_K4           = 83,
		KEY_K5           = 84,
		KEY_K6           = 85,
		KEY_K7           = 79,
		KEY_K8           = 80,
		KEY_K9           = 81,
		KEY_KDECIMAL     = 91,
		KEY_KDIVIDE      = 106,
		KEY_KMULTIPLY    = 63,
		KEY_LEFT         = 113,
		KEY_LESS         = 20,
		KEY_MENU         = 135,
		KEY_META_L       = 133,
		KEY_META_R       = 134,
		KEY_NUM_LOCK     = 77,
		KEY_PAGE_DOWN    = 117,
		KEY_PAGE_UP      = 112,
		KEY_PAUSE        = 127,
		KEY_PRINT        = 99,
		KEY_RIGHT        = 114,
		KEY_SCROLL_LOCK  = 78,
		KEY_SHIFT_L      = 50,
		KEY_SHIFT_R      = 62,
		KEY_TAB          = 23,
		KEY_UP           = 111,
		KEY_BACKSLASH    = '/',
		KEY_BRACKETLEFT  = '{',
		KEY_BRACKETRIGHT = '}',
		KEY_COMMA        = ',',
		KEY_DBLQUOTE     = '"',
		KEY_EQUAL        = '=',
		KEY_GRAVE        = '`',
		KEY_MINUS        = '-',
		KEY_PERIOD       = '.',
		KEY_PLUS         = '+',
		KEY_RETURN       = '\n',
		KEY_SEMICOLON    = ';',
		KEY_SLASH        = '\\',
		KEY_SPACE        = ' ',
		KEY_0            = '0',
		KEY_1            = '1',
		KEY_2            = '2',
		KEY_3            = '3',
		KEY_4            = '4',
		KEY_5            = '5',
		KEY_6            = '6',
		KEY_7            = '7',
		KEY_8            = '8',
		KEY_9            = '9',
		KEY_A            = 'a',
		KEY_B            = 'b',
		KEY_C            = 'c',
		KEY_D            = 'd',
		KEY_E            = 'e',
		KEY_F            = 'f',
		KEY_G            = 'g',
		KEY_H            = 'h',
		KEY_I            = 'i',
		KEY_J            = 'j',
		KEY_K            = 'k',
		KEY_L            = 'l',
		KEY_M            = 'm',
		KEY_N            = 'n',
		KEY_O            = 'o',
		KEY_P            = 'p',
		KEY_Q            = 'q',
		KEY_R            = 'r',
		KEY_S            = 's',
		KEY_T            = 't',
		KEY_U            = 'u',
		KEY_V            = 'v',
		KEY_W            = 'w',
		KEY_X            = 'x',
		KEY_Y            = 'y',
		KEY_Z            = 'z'
	};

	/*! @brief Keyboard Event Class */
	class EVENT_EXPORT KeyboardEvent : public EventBase
	{
		NO_ASSIGN(KeyboardEvent);
		NO_COPY(KeyboardEvent);

		KBActions m_action;
		KBKeys    m_key;

	public:

		KeyboardEvent(KBKeys key, KBActions action, TIME timestamp = 0);
		virtual ~KeyboardEvent(void);

		KBActions action(void) const
		    { return(m_action); }

		KBKeys key(void) const
		    { return(m_key); }

	public: /* virtual */

		VIRTUAL const Core::Type & type(void) const
		    { return(Type()); }

	public: /* static */

		static const Core::Type & Type(void);

	private: /* static */

		static const Core::Type sType;
	};
}

MARSHMALLOW_NAMESPACE_END

#endif
