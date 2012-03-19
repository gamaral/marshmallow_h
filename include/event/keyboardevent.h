/*
 * Copyright 2011-2012 Marshmallow Engine. All rights reserved.
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

#ifndef MARSHMALLOW_EVENT_KEYBOARDEVENT_H
#define MARSHMALLOW_EVENT_KEYBOARDEVENT_H 1

#include <event/eventbase.h>

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
		KEY_NONE          = 0,

		KEY_BACKSPACE     = 8,
		KEY_TAB           = 9,
		KEY_CLEAR         = 12,
		KEY_RETURN        = 13,
		KEY_PAUSE         = 19,
		KEY_ESCAPE        = 27,
		KEY_SPACE         = 32,
		KEY_EXCLAIM       = 33,
		KEY_DBLQUOTE      = 34,
		KEY_HASH          = 35,
		KEY_DOLLAR        = 36,
		KEY_AMPERSAND     = 38,
		KEY_QUOTE         = 39,
		KEY_LEFTPAREN     = 40,
		KEY_RIGHTPAREN    = 41,
		KEY_ASTERISK      = 42,
		KEY_PLUS          = 43,
		KEY_COMMA         = 44,
		KEY_MINUS         = 45,
		KEY_PERIOD        = 46,
		KEY_SLASH         = 47,
		KEY_0             = 48,
		KEY_1             = 49,
		KEY_2             = 50,
		KEY_3             = 51,
		KEY_4             = 52,
		KEY_5             = 53,
		KEY_6             = 54,
		KEY_7             = 55,
		KEY_8             = 56,
		KEY_9             = 57,
		KEY_COLON         = 58,
		KEY_SEMICOLON     = 59,
		KEY_LESS          = 60,
		KEY_EQUAL         = 61,
		KEY_GREATER       = 62,
		KEY_QUESTION      = 63,
		KEY_AT            = 64,
		KEY_BRACKET_LEFT  = 91,
		KEY_BACKSLASH     = 92,
		KEY_BRACKET_RIGHT = 93,
		KEY_CARET         = 94,
		KEY_UNDERSCORE    = 95,
		KEY_BACKQUOTE     = 96,
		KEY_A             = 97,
		KEY_B             = 98,
		KEY_C             = 99,
		KEY_D             = 100,
		KEY_E             = 101,
		KEY_F             = 102,
		KEY_G             = 103,
		KEY_H             = 104,
		KEY_I             = 105,
		KEY_J             = 106,
		KEY_K             = 107,
		KEY_L             = 108,
		KEY_M             = 109,
		KEY_N             = 110,
		KEY_O             = 111,
		KEY_P             = 112,
		KEY_Q             = 113,
		KEY_R             = 114,
		KEY_S             = 115,
		KEY_T             = 116,
		KEY_U             = 117,
		KEY_V             = 118,
		KEY_W             = 119,
		KEY_X             = 120,
		KEY_Y             = 121,
		KEY_Z             = 122,
		KEY_DELETE        = 127,

		KEY_ALT_L         = 308,
		KEY_ALT_R         = 307,
		KEY_BREAK         = 318,
		KEY_CAPS_LOCK     = 301,
		KEY_CONTROL_L     = 306,
		KEY_CONTROL_R     = 305,
		KEY_DOWN          = 274,
		KEY_END           = 279,
		KEY_F1            = 282,
		KEY_F10           = 291,
		KEY_F11           = 292,
		KEY_F12           = 293,
		KEY_F13           = 294,
		KEY_F14           = 295,
		KEY_F15           = 296,
		KEY_F2            = 283,
		KEY_F3            = 284,
		KEY_F4            = 285,
		KEY_F5            = 286,
		KEY_F6            = 287,
		KEY_F7            = 288,
		KEY_F8            = 289,
		KEY_F9            = 290,
		KEY_HELP          = 315,
		KEY_HOME          = 278,
		KEY_INSERT        = 277,
		KEY_K0            = 256,
		KEY_K1            = 257,
		KEY_K2            = 258,
		KEY_K3            = 259,
		KEY_K4            = 260,
		KEY_K5            = 261,
		KEY_K6            = 262,
		KEY_K7            = 263,
		KEY_K8            = 264,
		KEY_K9            = 265,
		KEY_KDECIMAL      = 266,
		KEY_KDIVIDE       = 267,
		KEY_KMULTIPLY     = 268,
		KEY_LEFT          = 276,
		KEY_MENU          = 319,
		KEY_META_L        = 310,
		KEY_META_R        = 309,
		KEY_NUM_LOCK      = 300,
		KEY_PAGE_DOWN     = 281,
		KEY_PAGE_UP       = 280,
		KEY_PRINT         = 316,
		KEY_RIGHT         = 275,
		KEY_SCROLL_LOCK   = 302,
		KEY_SHIFT_L       = 304,
		KEY_SHIFT_R       = 303,
		KEY_UP            = 273,

		KEY_LAST
	};

	/*! @brief Keyboard Event Class */
	class MARSHMALLOW_EVENT_EXPORT
	KeyboardEvent : public EventBase
	{
		KBActions m_action;
		KBKeys    m_key;

		NO_ASSIGN_COPY(KeyboardEvent);
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
