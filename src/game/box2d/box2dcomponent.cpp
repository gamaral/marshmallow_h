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

#include "game/box2d/box2dcomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <Box2D/Box2D.h>

#include "core/logger.h"

#include "graphics/meshbase.h"

#include "game/box2d/box2dscenelayer.h"
#include "game/entityscenelayer.h"
#include "game/ientity.h"
#include "game/iscene.h"
#include "game/positioncomponent.h"
#include "game/rendercomponent.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Game;

const Core::Type Box2DComponent::sType("Game::Box2DComponent");

Box2DComponent::Box2DComponent(const Core::Identifier &i, IEntity &e)
    : ComponentBase(i, e),
      m_b2layer(),
      m_position(),
      m_render(),
      m_size(),
      m_body(0),
      m_body_type(b2_staticBody),
      m_density(1.f),
      m_friction(0.3f),
      m_init(false)
{
}

Box2DComponent::~Box2DComponent(void)
{
}

void
Box2DComponent::update(TIME d)
{
	UNUSED(d);

	if (!m_position) {
		m_position = entity().getComponentType("Game::PositionComponent").
		    staticCast<PositionComponent>();
	}

	if (!m_render) {
		m_render = entity().getComponentType("Game::RenderComponent").
		    staticCast<RenderComponent>();
	}

	if (!m_init && !m_b2layer && m_position) {
		WeakSceneLayer l_layer = entity().layer().scene().getLayerType("Game::Box2DSceneLayer");
		m_b2layer = l_layer.cast<Box2DSceneLayer>();

		if (!m_b2layer) {
			MMWARNING1("Box2DComponent used with non Box2D Scene!");
			return;
		}

		b2World &l_world = m_b2layer->world();

		/* create box2d body */
		b2BodyDef bodyDef;
		bodyDef.type = static_cast<b2BodyType>(m_body_type);
#define DEGREE_TO_RADIAN 0.0174532925f
		if (m_render) bodyDef.angle = m_render->mesh()->rotation() * DEGREE_TO_RADIAN;
		bodyDef.position.Set
		    (m_position->position().rx(),
		     m_position->position().ry());
		m_body = l_world.CreateBody(&bodyDef);

		/* create shape */
		b2PolygonShape l_dynamicBox;
		l_dynamicBox.SetAsBox(m_size.width() / 2.f,
				      m_size.height() / 2.f);

		/* create fixture */
		b2FixtureDef l_fixtureDef;
		l_fixtureDef.shape = &l_dynamicBox;
		l_fixtureDef.density = m_density;
		l_fixtureDef.friction = m_friction;
		m_body->CreateFixture(&l_fixtureDef);

		m_init = true;
	}

	/* abort if not initialized */
	if (!m_init)
		return;

	b2Vec2 l_position = m_body->GetPosition();
	float32 l_angle = m_body->GetAngle();

	/* entity position */
	m_position->position().rx() = l_position.x;
	m_position->position().ry() = l_position.y;

	/* render mesh rotation */
	if (m_render) {
#define RADIAN_TO_DEGREE 57.2957795f
		Graphics::WeakMeshBase l_gbase(m_render->mesh().staticCast<Graphics::MeshBase>());
		if (l_gbase) l_gbase->setRotation(fmodf(l_angle * RADIAN_TO_DEGREE, 360.f));
	}
}

bool
Box2DComponent::serialize(TinyXML::TiXmlElement &n) const
{
	if (!ComponentBase::serialize(n))
	    return(false);

	switch (m_body_type) {
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

	n.SetDoubleAttribute("width", m_size.width());
	n.SetDoubleAttribute("height", m_size.height());

	n.SetDoubleAttribute("density", m_density);
	n.SetDoubleAttribute("friction", m_friction);

	return(true);
}

bool
Box2DComponent::deserialize(TinyXML::TiXmlElement &n)
{
	if (!ComponentBase::deserialize(n))
	    return(false);

	const char *l_body = n.Attribute("body");

	m_body_type = b2_staticBody;
	if (l_body && l_body[0] == 'k')
	    m_body_type = b2_kinematicBody;
	if (l_body && l_body[0] == 'd')
	    m_body_type = b2_dynamicBody;

	n.QueryFloatAttribute("width", &m_size.rwidth());
	n.QueryFloatAttribute("height", &m_size.rheight());

	n.QueryFloatAttribute("density", &m_density);
	n.QueryFloatAttribute("friction", &m_friction);

	return(true);
}

const Core::Type &
Box2DComponent::Type(void)
{
	return(sType);
}

