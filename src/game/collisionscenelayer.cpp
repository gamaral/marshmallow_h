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

#include "game/collisionscenelayer.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include "math/point2.h"
#include "math/vector2.h"

#include <tinyxml2.h>

MARSHMALLOW_NAMESPACE_USE
using namespace Game;

struct CollisionSceneLayer::Private
{
	ColliderList colliders;
};

CollisionSceneLayer::CollisionSceneLayer(const Core::Identifier &i, IScene &s)
    : SceneLayerBase(i, s)
    , m_p(new Private)
{
}

CollisionSceneLayer::~CollisionSceneLayer(void)
{
	delete m_p, m_p = 0;
}

void
CollisionSceneLayer::registerCollider(ColliderComponent &collider)
{
	m_p->colliders.push_back(&collider);
}

void
CollisionSceneLayer::deregisterCollider(ColliderComponent &collider)
{
	m_p->colliders.remove(&collider);
}

const ColliderList &
CollisionSceneLayer::colliders(void) const
{
	return(m_p->colliders);
}

void
CollisionSceneLayer::update(float)
{
}

bool
CollisionSceneLayer::serialize(XMLElement &n) const
{
	if (!SceneLayerBase::serialize(n))
		return(false);

	return(true);
}

bool
CollisionSceneLayer::deserialize(XMLElement &n)
{
	if (!SceneLayerBase::deserialize(n))
		return(false);

	return(true);
}

const Core::Type &
CollisionSceneLayer::Type(void)
{
	static const Core::Type s_type("Game::CollisionSceneLayer");
	return(s_type);
}

