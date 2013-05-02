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

#include "game/rendercomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/logger.h"
#include "core/type.h"

#include "graphics/imesh.h"
#include "graphics/painter.h"

#include "game/factory.h"
#include "game/ientity.h"
#include "game/positioncomponent.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

struct RenderComponent::Private
{
	Private()
	    : position(0)
	    , mesh(0)
	{}

	~Private()
	{
		delete mesh, mesh = 0;
		position = 0;
	}

	PositionComponent *position;
	Graphics::IMesh *mesh;
};

RenderComponent::RenderComponent(const Core::Identifier &i, IEntity &e)
    : Component(i, e)
    , PIMPL_CREATE
{
}

RenderComponent::~RenderComponent(void)
{
	PIMPL_DESTROY;
}

Graphics::IMesh *
RenderComponent::mesh(void) const
{
	return(PIMPL->mesh);
}

void
RenderComponent::setMesh(Graphics::IMesh *m)
{
	PIMPL->mesh = m;
}

void
RenderComponent::update(float)
{
	if (!PIMPL->position)
		PIMPL->position = static_cast<PositionComponent *>
		    (entity().getComponentType("Game::PositionComponent"));
}

void
RenderComponent::render(void)
{
	if (PIMPL->position && PIMPL->mesh)
		Graphics::Painter::Draw(*PIMPL->mesh, PIMPL->position->position());
}

const Core::Type &
RenderComponent::Type(void)
{
	static const Core::Type s_type("Game::RenderComponent");
	return(s_type);
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

