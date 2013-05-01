/*
 * Copyright (c) 2011-2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef MARSHMALLOW_GAME_BACKEND_H
#define MARSHMALLOW_GAME_BACKEND_H 1

#include <core/environment.h>
#include <core/namespace.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

	struct IEngine;

namespace Backend { /******************************** Game::Backend Namespace */

	/*!
	 * @brief Toggle engine between suspend and resume
	 * @return true when suspended
	 */
	MARSHMALLOW_GAME_EXPORT
	bool Pause(void);

	/*!
	 * @brief Suspend game engine
	 */
	MARSHMALLOW_GAME_EXPORT
	void Suspend(void);

	/*!
	 * @brief Resume game engine
	 */
	MARSHMALLOW_GAME_EXPORT
	void Resume(void);

	/*!
	 * @brief Stop game engine
	 * @param exit_code Exit code returned by game binary
	 */
	MARSHMALLOW_GAME_EXPORT
	void Stop(int exit_code = 0);

	/*!
	 * @brief Game engine singleton
	 * @return Pointer to game engine instance
	 */
	MARSHMALLOW_GAME_EXPORT
	IEngine * Instance(void);

} /************************************************** Game::Backend Namespace */
} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
