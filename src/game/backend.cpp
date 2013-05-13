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

#include "game/backend_p.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "game/iengine.h"

#include <cassert>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */
namespace { /************************************ Game::<anonymous> Namespace */
	static Game::IEngine *s_instance(0);
} /********************************************** Game::<anonymous> Namespace */

bool
Backend::Pause(void)
{
	assert(s_instance && "No valid engine instance!");

	if (s_instance->isSuspended())
		s_instance->resume();
	else
		s_instance->suspend();

	return(s_instance->isSuspended());
}

void
Backend::Suspend(void)
{
	assert(s_instance && "No valid engine instance!");
	s_instance->suspend();
}

void
Backend::Resume(void)
{
	assert(s_instance && "No valid engine instance!");
	s_instance->resume();
}

void
Backend::Stop(int exit_code)
{
	assert(s_instance && "No valid engine instance!");
	s_instance->stop(exit_code);
}

IEngine *
Backend::Instance(void)
{
	return(s_instance);
}

void
Backend::SetInstance(IEngine *instance)
{
	assert(((0 == s_instance && instance) || (s_instance && 0 == instance))
	    && "Tried to assign an invalid engine instance!");
	s_instance = instance;
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

