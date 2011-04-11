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

#include "game/viewbase.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/platform.h"
#include "game/engine.h"
#include "game/scenemanager.h"
#include "game/scenebase.h"
#include "game/entitybase.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Game;

ViewBase::ViewBase(int p)
    : m_phases(p)
{
}

ViewBase::~ViewBase(void)
{
}

void
ViewBase::initialize(void)
{
}

void
ViewBase::finalize(void)
{
}

void
ViewBase::render(const SharedScene &s)
{
	const EntityList &l_entityList = s->entities();

	int l_pi;
	EntityList::const_iterator l_i;
	EntityList::const_iterator l_c = l_entityList.end();
	
	for (l_pi = 0; l_pi < m_phases; ++l_pi)
		for (l_i = l_entityList.begin(); l_i != l_c; ++l_i)
			renderEntity(*l_i, l_pi);
}

void
ViewBase::renderEntity(const SharedEntity &e, int p)
{
	INFO("Entity %s needs rendering (phase %d)", e->id().str(), p);
}

