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

#include "game/pausescenelayer.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/type.h"

#include "math/point2.h"

#include "graphics/painter.h"
#include "graphics/quadmesh.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

/* TODO: clean up */
struct PauseSceneLayer::Private
{
	Private()
	    : mesh(0)
	{}

	Graphics::IMesh *mesh;
};

PauseSceneLayer::PauseSceneLayer(const Core::Identifier &i, Game::IScene *s)
    : SceneLayer(i, s, UpdateBlock)
    , PIMPL_CREATE
{
}

PauseSceneLayer::~PauseSceneLayer(void)
{
	PIMPL_DESTROY;
}

Graphics::IMesh *
PauseSceneLayer::mesh(void) const
{
	return(PIMPL->mesh);
}

void
PauseSceneLayer::render(void)
{
	if (!PIMPL->mesh)
		return;

	Graphics::Painter::PushMatrix();
	Graphics::Painter::LoadIdentity();
	Graphics::Painter::Draw(*PIMPL->mesh, Math::Point2(0,0));
	Graphics::Painter::PopMatrix();
}

void
PauseSceneLayer::update(float)
{
	if (PIMPL->mesh)
		return;

	Graphics::QuadMesh *l_mesh = new Graphics::QuadMesh(8, 8);
	l_mesh->setColor(Graphics::Color(0.f, 0.f, 0.f, 0.25f));
	PIMPL->mesh = l_mesh;
}

const Core::Type &
PauseSceneLayer::Type(void)
{
	static const Core::Type s_type("Game::PauseSceneLayer");
	return(s_type);
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

