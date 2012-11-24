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

#include "game/box2d/box2dcomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/logger.h"
#include "core/weak.h"

#include "math/size2.h"

#include "graphics/meshbase.h"

#include "game/box2d/box2dscenelayer.h"
#include "game/entityscenelayer.h"
#include "game/ientity.h"
#include "game/iscene.h"
#include "game/positioncomponent.h"
#include "game/rendercomponent.h"

#include <Box2D/Box2D.h>

#include <tinyxml2.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

struct Box2DComponent::Private
{
	WeakBox2DSceneLayer   b2layer;
	WeakPositionComponent position;
	WeakRenderComponent   render;
	Math::Size2f size;
	b2Body*      body;
	int   body_type;
	float density;
	float friction;
	bool  init;
};

Box2DComponent::Box2DComponent(const Core::Identifier &i, IEntity &e)
    : ComponentBase(i, e)
    , m_p(new Private)
{
	  m_p->body = 0;
	  m_p->body_type = b2_staticBody;
	  m_p->density = 1.f;
	  m_p->friction = 0.3f;
	  m_p->init = false;
}

Box2DComponent::~Box2DComponent(void)
{
	delete m_p, m_p = 0;
}

b2Body *
Box2DComponent::body(void)
{
	return(m_p->body);
}

int &
Box2DComponent::bodyType(void)
{
	return(m_p->body_type);
}

float &
Box2DComponent::density(void)
{
	return(m_p->density);
}

float &
Box2DComponent::friction(void)
{
	return(m_p->friction);
}

Math::Size2f &
Box2DComponent::size(void)
{
	return(m_p->size);
}

void
Box2DComponent::update(float d)
{
	MMUNUSED(d);

	if (!m_p->position) {
		m_p->position = entity().getComponentType("Game::PositionComponent").
		    staticCast<PositionComponent>();
	}

	if (!m_p->render) {
		m_p->render = entity().getComponentType("Game::RenderComponent").
		    staticCast<RenderComponent>();
	}

	if (!m_p->init && !m_p->b2layer && m_p->position) {
		WeakSceneLayer l_layer = entity().layer().scene().getLayerType("Game::Box2DSceneLayer");
		m_p->b2layer = l_layer.cast<Box2DSceneLayer>();

		if (!m_p->b2layer) {
			MMWARNING("Box2DComponent used with non Box2D Scene!");
			return;
		}

		b2World &l_world = m_p->b2layer->world();

		/* create box2d body */
		b2BodyDef bodyDef;
		bodyDef.type = static_cast<b2BodyType>(m_p->body_type);
#define DEGREE_TO_RADIAN 0.0174532925f
		if (m_p->render) bodyDef.angle = m_p->render->mesh()->rotation() * DEGREE_TO_RADIAN;
		bodyDef.position.Set
		    (m_p->position->position().x,
		     m_p->position->position().y);
		m_p->body = l_world.CreateBody(&bodyDef);

		/* create shape */
		b2PolygonShape l_dynamicBox;
		l_dynamicBox.SetAsBox(m_p->size.width  / 2.f,
		                      m_p->size.height / 2.f);

		/* create fixture */
		b2FixtureDef l_fixtureDef;
		l_fixtureDef.shape = &l_dynamicBox;
		l_fixtureDef.density = m_p->density;
		l_fixtureDef.friction = m_p->friction;
		m_p->body->CreateFixture(&l_fixtureDef);

		m_p->init = true;
	}

	/* abort if not initialized */
	if (!m_p->init)
		return;

	b2Vec2 l_position = m_p->body->GetPosition();
	float32 l_angle = m_p->body->GetAngle();

	/* entity position */
	m_p->position->position().x = l_position.x;
	m_p->position->position().y = l_position.y;

	/* render mesh rotation */
	if (m_p->render) {
#define RADIAN_TO_DEGREE 57.2957795f
		Graphics::WeakMeshBase l_gbase(m_p->render->mesh().staticCast<Graphics::MeshBase>());
		if (l_gbase) l_gbase->setRotation(fmodf(l_angle * RADIAN_TO_DEGREE, 360.f));
	}
}

bool
Box2DComponent::serialize(XMLElement &n) const
{
	if (!ComponentBase::serialize(n))
	    return(false);

	switch (m_p->body_type) {
	case b2_staticBody:
	    n.SetAttribute("body", "static");
	    break;
	case b2_kinematicBody:
	    n.SetAttribute("body", "kinematic");
	    break;
	case b2_dynamicBody:
	    n.SetAttribute("body", "dynamic");
	    break;
	}

	n.SetAttribute("width", m_p->size.width);
	n.SetAttribute("height", m_p->size.height);

	n.SetAttribute("density", m_p->density);
	n.SetAttribute("friction", m_p->friction);

	return(true);
}

bool
Box2DComponent::deserialize(XMLElement &n)
{
	if (!ComponentBase::deserialize(n))
	    return(false);

	const char *l_body = n.Attribute("body");

	m_p->body_type = b2_staticBody;
	if (l_body && l_body[0] == 'k')
	    m_p->body_type = b2_kinematicBody;
	if (l_body && l_body[0] == 'd')
	    m_p->body_type = b2_dynamicBody;

	n.QueryFloatAttribute("width", &m_p->size.width);
	n.QueryFloatAttribute("height", &m_p->size.height);

	n.QueryFloatAttribute("density", &m_p->density);
	n.QueryFloatAttribute("friction", &m_p->friction);

	return(true);
}

const Core::Type &
Box2DComponent::Type(void)
{
	static const Core::Type s_type("Game::Box2DComponent");
	return(s_type);
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

