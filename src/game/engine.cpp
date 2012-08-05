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

#include "game/engine_p.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "game/iengine.h"

#include <cassert>

MARSHMALLOW_NAMESPACE_BEGIN
namespace { /******************************************** Anonymous Namespace */

	Game::IEngine *s_instance;

} /****************************************************** Anonymous Namespace */

namespace Game { /******************************************** Game Namespace */

bool
Engine::Pause(void)
{
	assert(s_instance && s_instance->isValid()
	    && "No valid engine instance!");

	if (s_instance->isSuspended())
		s_instance->resume();
	else
		s_instance->suspend();

	return(s_instance->isSuspended());
}

void
Engine::Suspend(void)
{
	assert(s_instance && s_instance->isValid()
	    && "No valid engine instance!");
	s_instance->suspend();
}

void
Engine::Resume(void)
{
	assert(s_instance && s_instance->isValid()
	    && "No valid engine instance!");
	s_instance->resume();
}

void
Engine::Stop(int exit_code)
{
	assert(s_instance && s_instance->isValid()
	    && "No valid engine instance!");
	s_instance->stop(exit_code);
}

IEngine *
Engine::Instance(void)
{
	return(s_instance);
}

void
Engine::SetInstance(IEngine *instance)
{
	assert(((0 == s_instance && instance) || (s_instance && 0 == instance))
	    && "Tried to assign an invalid engine instance!");
	s_instance = instance;
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

