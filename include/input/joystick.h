/*
 * Copyright 2012-2013 Marshmallow Engine. All rights reserved.
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

#ifndef MARSHMALLOW_INPUT_JOYSTICK_H
#define MARSHMALLOW_INPUT_JOYSTICK_H 1

#include <core/environment.h>
#include <core/namespace.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Input { /****************************************** Input Namespace */

/**** IMPLEMENTATION NOTES *****************************************************
 *
 */
namespace Joystick { /***************************** Input::Joystick Namespace */

	/*! @brief Joystick Actions */
	enum Action
	{
		Unknown = 0,                                /*!< Unknown Action */
		ButtonReleased,
		ButtonPressed,
		AxisChanged,
		Actions                                     /*!< Action Count */
	};

	/*! @brief Joystick Axes */
	enum Axis
	{
		JSA_NONE = 0,

		JSA_HX,
		JSA_HY,

		JSA_X,
		JSA_Y,
		JSA_Z,

		JSA_RX,
		JSA_RY,
		JSA_RZ
	};

	/*! @brief Joystick Buttons */
	enum Button
	{
		JSB_NONE    = 0,

		JSB_A       = (1 << 0),
		JSB_B       = (1 << 1),
		JSB_X       = (1 << 2),
		JSB_Y       = (1 << 3),
		JSB_L       = (1 << 4),
		JSB_R       = (1 << 5),

		JSB_THUMB_L = (1 << 6),
		JSB_THUMB_R = (1 << 7),

		JSB_START   = (1 << 8),
		JSB_SELECT  = (1 << 9),

		JSB_MENU    = (1 << 10),

		JSB_DOWN    = (1 << 11),
		JSB_LEFT    = (1 << 12),
		JSB_RIGHT   = (1 << 13),
		JSB_UP      = (1 << 14)
	};

} /************************************************ Input::Joystick Namespace */
} /********************************************************** Input Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
