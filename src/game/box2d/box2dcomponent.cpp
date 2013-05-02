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

#include "game/box2d/box2dcomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"
#include "core/type.h"

#include "math/size2.h"

#include "graphics/meshbase.h"

#include "game/box2d/box2dscenelayer.h"
#include "game/entityscenelayer.h"
#include "game/ientity.h"
#include "game/iscene.h"
#include "game/positioncomponent.h"
#include "game/rendercomponent.h"

#include <Box2D/Box2D.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

struct Box2DComponent::Private
{
	Private()
	    : b2layer(0)
	    , position(0)
	    , render(0)
	    , body(0)
	    , body_type(b2_staticBody)
	    , density(1.f)
	    , friction(0.3f)
	    , init(false)
	{}

	Math::Size2f size;

	Box2DSceneLayer   *b2layer;
	PositionComponent *position;
	RenderComponent   *render;
	b2Body            *body;

	int   body_type;
	float density;
	float friction;
	bool  init;
};

Box2DComponent::Box2DComponent(const Core::Identifier &i, IEntity &e)
    : Component(i, e)
    , PIMPL_CREATE
{
}

Box2DComponent::~Box2DComponent(void)
{
	PIMPL_DESTROY;
}

b2Body *
Box2DComponent::body(void)
{
	return(PIMPL->body);
}

int &
Box2DComponent::bodyType(void)
{
	return(PIMPL->body_type);
}

float &
Box2DComponent::density(void)
{
	return(PIMPL->density);
}

float &
Box2DComponent::friction(void)
{
	return(PIMPL->friction);
}

Math::Size2f &
Box2DComponent::size(void)
{
	return(PIMPL->size);
}

void
Box2DComponent::update(float d)
{
	MMUNUSED(d);

	if (!PIMPL->position) {
		PIMPL->position = static_cast<PositionComponent *>
		    (entity().getComponentType("Game::PositionComponent"));
	}

	if (!PIMPL->render) {
		PIMPL->render = static_cast<RenderComponent *>
		    (entity().getComponentType("Game::RenderComponent"));
	}

	if (!PIMPL->init && !PIMPL->b2layer && PIMPL->position) {
		PIMPL->b2layer = static_cast<Box2DSceneLayer *>
		    (entity().layer().scene().getLayerType("Game::Box2DSceneLayer"));

		if (!PIMPL->b2layer) {
			MMWARNING("Box2DComponent used with non Box2D Scene!");
			return;
		}

		b2World &l_world = PIMPL->b2layer->world();

		/* create box2d body */
		b2BodyDef bodyDef;
		bodyDef.type = static_cast<b2BodyType>(PIMPL->body_type);
#define DEGREE_TO_RADIAN 0.0174532925f
		if (PIMPL->render) bodyDef.angle = PIMPL->render->mesh()->rotation() * DEGREE_TO_RADIAN;
		bodyDef.position.Set
		    (PIMPL->position->position().x,
		     PIMPL->position->position().y);
		PIMPL->body = l_world.CreateBody(&bodyDef);

		/* create shape */
		b2PolygonShape l_dynamicBox;
		l_dynamicBox.SetAsBox(PIMPL->size.width  / 2.f,
		                      PIMPL->size.height / 2.f);

		/* create fixture */
		b2FixtureDef l_fixtureDef;
		l_fixtureDef.shape = &l_dynamicBox;
		l_fixtureDef.density = PIMPL->density;
		l_fixtureDef.friction = PIMPL->friction;
		PIMPL->body->CreateFixture(&l_fixtureDef);

		PIMPL->init = true;
	}

	/* abort if not initialized */
	if (!PIMPL->init)
		return;

	b2Vec2 l_position = PIMPL->body->GetPosition();
	float32 l_angle = PIMPL->body->GetAngle();

	/* entity position */
	PIMPL->position->setPosition(l_position.x, l_position.y);

	/* render mesh rotation */
	if (PIMPL->render) {
#define RADIAN_TO_DEGREE 57.2957795f
		Graphics::MeshBase *l_gbase =
		    static_cast<Graphics::MeshBase *>(PIMPL->render->mesh());
		if (l_gbase) l_gbase->setRotation(fmodf(l_angle * RADIAN_TO_DEGREE, 360.f));
	}
}

const Core::Type &
Box2DComponent::Type(void)
{
	static const Core::Type s_type("Game::Box2DComponent");
	return(s_type);
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

