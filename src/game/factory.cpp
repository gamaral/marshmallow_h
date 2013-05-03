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

#include "game/factory.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/type.h"

#include "graphics/quadmesh.h"

#include "game/entity.h"
#include "game/entityscenelayer.h"
#include "game/movementcomponent.h"
#include "game/pausescenelayer.h"
#include "game/positioncomponent.h"
#include "game/rendercomponent.h"
#include "game/scene.h"
#include "game/splashscenelayer.h"

#if MARSHMALLOW_WITH_BOX2D
#   include "game/box2d/box2dcomponent.h"
#   include "game/box2d/box2dscenelayer.h"
#endif

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */
namespace { /************************************ Game::<anonymous> Namespace */
	static Game::IFactory *s_instance(0);
} /********************************************** Game::<anonymous> Namespace */

Factory::Factory(void)
{
	if (!s_instance) s_instance = this;
}

Factory::~Factory(void)
{
	if (s_instance == this) s_instance = 0;
}

IScene *
Factory::createScene(const Core::Type &t,
                     const Core::Identifier &i) const
{
	if (t == Scene::Type()) return(new Scene(i));
	return(0);
}

ISceneLayer *
Factory::createSceneLayer(const Core::Type &t,
                          const Core::Identifier &i,
                          Game::IScene *s) const
{
	if (t == EntitySceneLayer::Type()) return(new EntitySceneLayer(i, s));
	else if (t == PauseSceneLayer::Type()) return(new PauseSceneLayer(i, s));
	else if (t == SplashSceneLayer::Type()) return(new SplashSceneLayer(i, s));
#if MARSHMALLOW_WITH_BOX2D
	else if (t == Box2DSceneLayer::Type()) return(new Box2DSceneLayer(i, s));
#endif
	return(0);
}

Game::IEntity *
Factory::createEntity(const Core::Type &t,
                      const Core::Identifier &i,
                      Game::EntitySceneLayer *l) const
{
	if (t == Entity::Type()) return(new Entity(i, l));
	return(0);
}

IComponent *
Factory::createComponent(const Core::Type &t,
                         const Core::Identifier &i,
                         Game::IEntity *e) const
{
	if (t == MovementComponent::Type()) return(new MovementComponent(i, e));
	else if (t == RenderComponent::Type()) return(new RenderComponent(i, e));
	else if (t == PositionComponent::Type()) return(new PositionComponent(i, e));
#if MARSHMALLOW_WITH_BOX2D
	else if (t == Box2DComponent::Type()) return(new Box2DComponent(i, e));
#endif
	return(0);
}

Graphics::IMesh *
Factory::createMesh(const Core::Type &t) const
{
	if (t == Graphics::QuadMesh::Type()) return(new Graphics::QuadMesh);
	return(0);
}

IFactory *
Factory::Instance(void)
{
	return(s_instance);
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

