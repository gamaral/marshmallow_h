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

#include "game/collidercomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include "graphics/meshbase.h"

#include "game/collisionscenelayer.h"
#include "game/entityscenelayer.h"
#include "game/ientity.h"
#include "game/iscene.h"
#include "game/movementcomponent.h"
#include "game/positioncomponent.h"
#include "game/sizecomponent.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Game;

#define DELTA_STEPS 32

const Core::Type ColliderComponent::sType("Game::ColliderComponent");

ColliderComponent::ColliderComponent(const Core::Identifier &i, IEntity &e)
    : ComponentBase(i, e)
    , m_layer()
    , m_position()
    , m_body(BoxType)
    , m_active(true)
    , m_bullet(false)
    , m_init(false)
{
}

ColliderComponent::~ColliderComponent(void)
{
	if (m_layer) {
		/* deregister as collider */
		m_layer->deregisterCollider(*this);
	}
}

float
ColliderComponent::radius2(void) const
{
	float l_radius2 = 0;
	if (m_size) {
		const Math::Size2f &l_size = m_size->size();
		l_radius2 = powf(l_size.width()  / 2.f, 2) +
		            powf(l_size.height() / 2.f, 2);
	}
	else MMWARNING1("Collider component found no size component!");
	return(l_radius2);
}

bool
ColliderComponent::isColliding(ColliderComponent &c, float *p) const
{
	if (m_position && c.position()) {
		const Math::Point2 &l_pos_a = m_position->position();
		const Math::Point2 &l_pos_b = c.position()->position();

		switch(m_body) {
		case SphereType: {
			float l_distance2 = l_pos_b.difference(l_pos_a).magnitude2();

			l_distance2 -= c.radius2() + radius2();

			if (l_distance2 < 0)
				if (p) *p = l_distance2 * -1;
			} break;
		case BoxType: {
			const Math::Size2f l_size_a = m_size->size()   / 2.f;
			const Math::Size2f l_size_b = c.size()->size() / 2.f;

			if (l_pos_b.x() + l_size_b.width()  > l_pos_a.x() - l_size_a.width() &&
			    l_pos_b.x() - l_size_b.width()  < l_pos_a.x() + l_size_a.width() &&
			    l_pos_b.y() - l_size_b.height() < l_pos_a.y() + l_size_a.height() &&
			    l_pos_b.y() + l_size_b.height() > l_pos_a.y() - l_size_a.height())
				return(true);
			} break;
		default: return(false);
		}
	}

	return(false);
}

void
ColliderComponent::update(float d)
{
	if (!m_movement) {
		m_movement = entity().getComponentType("Game::MovementComponent").
		    staticCast<MovementComponent>();
	}

	if (!m_position) {
		m_position = entity().getComponentType("Game::PositionComponent").
		    staticCast<PositionComponent>();
	}

	if (!m_size) {
		m_size = entity().getComponentType("Game::SizeComponent").
		    staticCast<SizeComponent>();
	}

	if (!m_init && !m_layer && m_position && m_size) {
		m_layer = entity().layer().scene()
		    .getLayerType("Game::CollisionSceneLayer").staticCast<CollisionSceneLayer>();
		if (!m_layer) {
			MMWARNING1("Collider component used with no collision scene layer!");
			return;
		}

		/* register as collider */
		m_layer->registerCollider(*this);

		m_init = true;
	}

	if (m_active && m_init && m_movement && m_size && m_position) {
		ColliderList::const_iterator l_i;
		ColliderList::const_iterator l_c = m_layer->colliders().end();

		for (l_i = m_layer->colliders().begin(); l_i != m_layer->colliders().end(); ++l_i) {
			if (*l_i == this) continue;

			ColliderComponent *l_collider = *l_i;
			float l_penetration;

			if (m_bullet) {
				const float l_delta_step = d / DELTA_STEPS;
				float l_bullet_delta = l_delta_step;

				for(int i = 1; i < DELTA_STEPS; ++i) {
					if (isColliding(*l_collider, &l_penetration)) {
						collision(*l_collider, l_penetration, l_bullet_delta);
						continue;
					}
					else { m_movement->move(l_bullet_delta += l_delta_step); }
				}
			}
			else {
				m_movement->move(d);

				if (isColliding(*l_collider, &l_penetration))
					collision(*l_collider, l_penetration, d);
				continue;
			}
		}
	}
}

bool
ColliderComponent::serialize(TinyXML::TiXmlElement &n) const
{
	if (!ComponentBase::serialize(n))
	    return(false);

	return(true);
}

bool
ColliderComponent::deserialize(TinyXML::TiXmlElement &n)
{
	if (!ComponentBase::deserialize(n))
	    return(false);

	return(true);
}

bool
ColliderComponent::collision(ColliderComponent& c, float p, float d)
{
	MMUNUSED(p);

	if (m_movement) {
		/* move to a position before collision happened */
		const float l_delta_step = d / DELTA_STEPS;
		for(int i = 0; i < DELTA_STEPS && isColliding(c); ++i)
			m_movement->move(d - (static_cast<float>(i) * l_delta_step));
	} else return(false);

	return(true);
}

const Core::Type &
ColliderComponent::Type(void)
{
	return(sType);
}

/************************************************** BounceColliderComponent */

BounceColliderComponent::BounceColliderComponent(const Core::Identifier &i, IEntity &e)
    : ColliderComponent(i, e)
    , m_factor(-1.f, -1.f)
{
}

bool
BounceColliderComponent::collision(ColliderComponent& c, float p, float d)
{
	if (ColliderComponent::collision(c, p, d)) {
		movement()->direction() *= m_factor;
		return(true);
	}
	return(false);
}

