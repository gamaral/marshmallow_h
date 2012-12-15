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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef MARSHMALLOW_INPUT_KEYBOARD_H
#define MARSHMALLOW_INPUT_KEYBOARD_H 1

#include <core/environment.h>
#include <core/fd.h>
#include <core/namespace.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Input { /****************************************** Input Namespace */

/**** IMPLEMENTATION NOTES *****************************************************
 *
 */
namespace Keyboard { /***************************** Input::Keyboard Namespace */

	/*! @brief Keyboard Key Actions */
	enum Action
	{
		Unknown = 0,                              /*!< Unknown Action */
		KeyReleased,
		KeyPressed,
		Actions                                     /*!< Action Count */
	};

	/*! @brief Keyboard Keys */
	enum Key
	{
		KBK_NONE          = 0,
		KBK_FIRST         = 0,

		KBK_BACKSPACE     = 8,
		KBK_TAB           = 9,
		KBK_CLEAR         = 12,
		KBK_RETURN        = 13,
		KBK_ENTER         = KBK_RETURN,
		KBK_PAUSE         = 19,
		KBK_ESCAPE        = 27,

		KBK_SPACE         = ' ',
		KBK_COMMA         = ',',
		KBK_HYPHEN        = '-',
		KBK_DASH          = KBK_HYPHEN,
		KBK_MINUS         = KBK_HYPHEN,
		KBK_PERIOD        = '.',
		KBK_DECIMAL       = KBK_PERIOD,
		KBK_DOT           = KBK_PERIOD,
		KBK_SLASH         = '/',

		KBK_0             = '0',
		KBK_1             = '1',
		KBK_2             = '2',
		KBK_3             = '3',
		KBK_4             = '4',
		KBK_5             = '5',
		KBK_6             = '6',
		KBK_7             = '7',
		KBK_8             = '8',
		KBK_9             = '9',

		KBK_SEMICOLON     = ';',
		KBK_EQUAL         = '=',
		KBK_EQUALS        = KBK_EQUAL,
		KBK_APOSTROPHE    = '\'',
		KBK_QUOTE         = KBK_APOSTROPHE,

		KBK_A             = 'a',
		KBK_B             = 'b',
		KBK_C             = 'c',
		KBK_D             = 'd',
		KBK_E             = 'e',
		KBK_F             = 'f',
		KBK_G             = 'g',
		KBK_H             = 'h',
		KBK_I             = 'i',
		KBK_J             = 'j',
		KBK_K             = 'k',
		KBK_L             = 'l',
		KBK_M             = 'm',
		KBK_N             = 'n',
		KBK_O             = 'o',
		KBK_P             = 'p',
		KBK_Q             = 'q',
		KBK_R             = 'r',
		KBK_S             = 's',
		KBK_T             = 't',
		KBK_U             = 'u',
		KBK_V             = 'v',
		KBK_W             = 'w',
		KBK_X             = 'x',
		KBK_Y             = 'y',
		KBK_Z             = 'z',

		KBK_BRACKET_L     = '[',
		KBK_BACKSLASH     = '\\',
		KBK_BRACKET_R     = ']',
		KBK_GRAVE         = '`',

		KBK_DELETE        = 127,

		KBK_SHIFT_L,
		KBK_SHIFT_R,
		KBK_CAPS_LOCK,

		KBK_ALT_L,
		KBK_ALT_R,

		KBK_CONTROL_L,
		KBK_CONTROL_R,

		KBK_META_L,
		KBK_META_R,

		KBK_SUPER_L,
		KBK_SUPER_R,

		KBK_F1,
		KBK_F2,
		KBK_F3,
		KBK_F4,
		KBK_F5,
		KBK_F6,
		KBK_F7,
		KBK_F8,
		KBK_F9,
		KBK_F10,
		KBK_F11,
		KBK_F12,
		KBK_F13,
		KBK_F14,
		KBK_F15,

		KBK_PRINT,
		KBK_SCROLL_LOCK,
		KBK_BREAK,

		KBK_INSERT,
		KBK_HOME,
		KBK_END,
		KBK_PAGE_UP,
		KBK_PAGE_DOWN,

		KBK_UP,
		KBK_DOWN,
		KBK_LEFT,
		KBK_RIGHT,

		KBK_NUM_LOCK,
		KBK_KDIVIDE,
		KBK_KSLASH        = KBK_KDIVIDE,
		KBK_KMULTIPLY,
		KBK_KASTERISK     = KBK_KMULTIPLY,
		KBK_KSUBTRACT,
		KBK_KDASH         = KBK_KSUBTRACT,
		KBK_KHYPHEN       = KBK_KSUBTRACT,
		KBK_KMINUS        = KBK_KSUBTRACT,
		KBK_K7,
		KBK_K8,
		KBK_K9,
		KBK_K4,
		KBK_K5,
		KBK_K6,
		KBK_KADD,
		KBK_KPLUS         = KBK_KADD,
		KBK_K1,
		KBK_K2,
		KBK_K3,
		KBK_K0,
		KBK_KDECIMAL,
		KBK_KPERIOD       = KBK_KDECIMAL,
		KBK_KDOT          = KBK_KDECIMAL,
		KBK_KENTER,

		KBK_CLOSE,
		KBK_HELP,
		KBK_MENU,

		KBK_LAST
	};

	MARSHMALLOW_INPUT_EXPORT
	Action KeyState(Key key);

} /************************************************ Input::Keyboard Namespace */
} /********************************************************** Input Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
