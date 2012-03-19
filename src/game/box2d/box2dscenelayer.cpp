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

#include "game/box2d/box2dscenelayer.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <Box2D/Box2D.h>

#include "math/vector2.h"

MARSHMALLOW_NAMESPACE_USE
using namespace Game;

const Core::Type Box2DSceneLayer::sType("Game::Box2DSceneLayer");

struct Box2DSceneLayer::Internal
{
	b2World world;

	Internal()
	: world(b2Vec2(0.f, -10.f)) {}
};

Box2DSceneLayer::Box2DSceneLayer(const Core::Identifier &i, IScene &s)
    : SceneLayerBase(i, s)
    , m_internal(new Internal)
{
}

Box2DSceneLayer::~Box2DSceneLayer(void)
{
	delete m_internal;
	m_internal = 0;
}

void
Box2DSceneLayer::update(float d)
{
	m_internal->world.Step
	    (static_cast<float>(d),
#define VELOCITY_ITERATIONS 10
	     VELOCITY_ITERATIONS,
#define POSITION_ITERATIONS 8
	     POSITION_ITERATIONS);
	m_internal->world.ClearForces();
}

Math::Vector2
Box2DSceneLayer::gravity(void) const
{
	b2Vec2 l_gravity = m_internal->world.GetGravity();
	return(Math::Vector2(l_gravity.x, l_gravity.y));
}

void
Box2DSceneLayer::setGravity(const Math::Vector2 &g)
{
	m_internal->world.SetGravity(g);
}

b2World &
Box2DSceneLayer::world(void)
{
	return(m_internal->world);
}

bool
Box2DSceneLayer::serialize(TinyXML::TiXmlElement &n) const
{
	if (!SceneLayerBase::serialize(n))
		return(false);

	TinyXML::TiXmlElement l_child("gravity");
	b2Vec2 l_gravity = m_internal->world.GetGravity();
	l_child.SetDoubleAttribute("x", l_gravity.x);
	l_child.SetDoubleAttribute("y", l_gravity.y);
	n.InsertEndChild(l_child);

	return(true);
}

bool
Box2DSceneLayer::deserialize(TinyXML::TiXmlElement &n)
{
	if (!SceneLayerBase::deserialize(n))
		return(false);

	TinyXML::TiXmlElement *l_child = n.FirstChildElement("gravity");
	if (l_child) {
		float l_x, l_y;
		l_child->QueryFloatAttribute("x", &l_x);
		l_child->QueryFloatAttribute("y", &l_y);
		m_internal->world.SetGravity(b2Vec2(l_x, l_y));
	}
	
	return(true);
}

const Core::Type &
Box2DSceneLayer::Type(void)
{
	return(sType);
}

