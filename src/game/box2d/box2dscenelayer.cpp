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

#include "game/box2d/box2dscenelayer.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/type.h"

#include "math/vector2.h"

#include "graphics/transform.h"

#include "game/config.h"

#include <Box2D/Box2D.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

struct Box2DSceneLayer::Private
{
	Private()
	    : world(b2Vec2(.0f, -10.f))
	    , accomulator(.0f)
	{}

	Graphics::Transform transform;
	b2World world;
	float accomulator;
};

Box2DSceneLayer::Box2DSceneLayer(const Core::Identifier &i, Game::IScene *s)
    : SceneLayer(i, s)
    , PIMPL_CREATE
{
}

Box2DSceneLayer::~Box2DSceneLayer(void)
{
	PIMPL_DESTROY;
}

void
Box2DSceneLayer::update(float d)
{
	static const float step(1.f/MARSHMALLOW_ENGINE_FRAMERATE);

	PIMPL->accomulator += d;
	if (PIMPL->accomulator < step)
		return;
	PIMPL->accomulator -= step;

	PIMPL->world.Step
	    (step,
#define VELOCITY_ITERATIONS 10
	     VELOCITY_ITERATIONS,
#define POSITION_ITERATIONS 8
	     POSITION_ITERATIONS);
	PIMPL->world.ClearForces();
}

Math::Vector2
Box2DSceneLayer::gravity(void) const
{
	b2Vec2 l_gravity = PIMPL->world.GetGravity();
	return(Math::Vector2(l_gravity.x, l_gravity.y));
}

void
Box2DSceneLayer::setGravity(const Math::Vector2 &gravity_)
{
	b2Vec2 l_gravity(gravity_.x, gravity_.y);
	PIMPL->world.SetGravity(l_gravity);
}

Graphics::Transform &
Box2DSceneLayer::transform(void) const
{
	return(PIMPL->transform);
}

void
Box2DSceneLayer::setTransform(const Graphics::Transform &_transform)
{
	PIMPL->transform = _transform;
}

b2World &
Box2DSceneLayer::world(void)
{
	return(PIMPL->world);
}

const Core::Type &
Box2DSceneLayer::Type(void)
{
	static const Core::Type s_type("Game::Box2DSceneLayer");
	return(s_type);
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

