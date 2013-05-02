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

#include "game/collidercomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"
#include "core/type.h"

#include "math/size2.h"

#include "game/collisionscenelayer.h"
#include "game/entityscenelayer.h"
#include "game/ientity.h"
#include "game/iscene.h"
#include "game/movementcomponent.h"
#include "game/positioncomponent.h"
#include "game/sizecomponent.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

#define DELTA_STEPS 32

struct ColliderComponent::Private
{
	Private(ColliderComponent &i)
	    : component(i)
	    , layer(0)
	    , movement(0)
	    , position(0)
	    , size(0)
	    , body(btBox)
	    , bullet_resolution(DELTA_STEPS)
	    , active(true)
	    , bullet(false)
	{}

	inline bool
	isColliding(ColliderComponent &c, float d, CollisionData *data) const;

	inline float
	radius2(void) const;

	inline void
	update(float d);

	ColliderComponent   &component;
	CollisionSceneLayer *layer;
	MovementComponent   *movement;
	PositionComponent   *position;
	SizeComponent       *size;
	int  body;
	int  bullet_resolution;
	bool active;
	bool bullet;
	bool init;
};

bool
ColliderComponent::Private::isColliding(ColliderComponent &cc, float d, CollisionData *data) const
{
	ColliderComponent::Private &c = *cc.PIMPL;

	if (movement && c.position) {
		const Math::Point2 l_pos_a = movement->simulate(d);
		const Math::Point2 &l_pos_b = c.position->position();

		switch(body) {
		case btSphere: {
			float l_distance2 = l_pos_b.difference(l_pos_a).magnitude2();
			l_distance2 -= c.radius2() + radius2();

			if (l_distance2 < 0) {
				if (data)
					data->sphere.penetration2 = l_distance2;
				return(true);
			}
			} break;

		case btBox: {
			const Math::Size2f l_size_a = size->size() / 2.f;
			const Math::Size2f l_size_b = c.size->size() / 2.f;

			const float l =
			    (l_pos_a.x + l_size_a.width)  - (l_pos_b.x - l_size_b.width);
			const float r =
			    (l_pos_b.x + l_size_b.width)  - (l_pos_a.x - l_size_a.width);
			const float t =
			    (l_pos_b.y + l_size_b.height) - (l_pos_a.y - l_size_a.height);
			const float b =
			    (l_pos_a.y + l_size_a.height) - (l_pos_b.y - l_size_b.height);

			if (data) {
				data->rect.left = l;
				data->rect.right = r;
				data->rect.top = t;
				data->rect.bottom = b;
			}

			if (l > 0 && r > 0 && t > 0 && b > 0)
				return(true);
			} break;

		case btCapsule:
		default: return(false);
		}
	}

	return(false);
}

float
ColliderComponent::Private::radius2(void) const
{
	float l_radius2 = 0;
	if (size) {
		const Math::Size2f &l_size = size->size();
		l_radius2 = powf(l_size.width  / 2.f, 2) +
		            powf(l_size.height / 2.f, 2);
	}
	else MMWARNING("Collider component found no size component!");
	return(l_radius2);
}

void
ColliderComponent::Private::update(float d)
{
	if (!movement) {
		movement = static_cast<MovementComponent *>
		    (component.entity().getComponentType("Game::MovementComponent"));
	}

	if (!position) {
		position = static_cast<PositionComponent *>
		    (component.entity().getComponentType("Game::PositionComponent"));
	}

	if (!size) {
		size = static_cast<SizeComponent *>
		    (component.entity().getComponentType("Game::SizeComponent"));
	}

	if (!init && !layer && position && size) {
		layer = static_cast<CollisionSceneLayer *>
		    (component.entity().layer().scene().getLayerType("Game::CollisionSceneLayer"));

		if (!layer) {
			MMWARNING("Collider component used with no collision scene layer!");
			return;
		}

		/* register as collider */
		layer->registerCollider(&component);

		init = true;
	}

	if (active && init && movement && size && position) {
		ColliderList::const_iterator l_i;
		ColliderList::const_iterator l_c = layer->colliders().end();

		for (l_i = layer->colliders().begin(); l_i != l_c; ++l_i) {
			if (*l_i == &component) continue;

			ColliderComponent *l_collider = *l_i;
			CollisionData data;

			if (bullet) {
				int l_steps = bullet_resolution;
				const float l_delta_step = d / static_cast<float>(l_steps);
				float l_bullet_delta = 0;

				for(int i = 1; i < l_steps; ++i) {
					if (component.isColliding(*l_collider, l_bullet_delta += l_delta_step, &data)) {
						component.collision(*l_collider, l_bullet_delta, data);
						continue;
					}
				}
			}
			else {
				if (component.isColliding(*l_collider, d, &data))
					component.collision(*l_collider, d, data);
				continue;
			}
		}
	}
}

ColliderComponent::ColliderComponent(const Core::Identifier &i, IEntity &e)
    : Component(i, e)
    , PIMPL_CREATE_X(*this)
{
}

ColliderComponent::~ColliderComponent(void)
{
	/* deregister as collider */
	if (PIMPL->layer)
		PIMPL->layer->deregisterCollider(this);

	PIMPL_DESTROY;
}

int
ColliderComponent::body(void) const
{
	return(PIMPL->body);
}

bool
ColliderComponent::active(void) const
{
	return(PIMPL->active);
}

bool
ColliderComponent::bullet(void) const
{
	return(PIMPL->bullet);
}

float
ColliderComponent::radius2(void) const
{
	return(PIMPL->radius2());
}

bool
ColliderComponent::isColliding(ColliderComponent &c, float d, CollisionData *data) const
{
	return(PIMPL->isColliding(c, d, data));
}

void
ColliderComponent::update(float d)
{
	PIMPL->update(d);
}

Game::CollisionSceneLayer *
ColliderComponent::layer(void) const
{
	return(PIMPL->layer);
}

Game::MovementComponent *
ColliderComponent::movement(void) const
{
	return(PIMPL->movement);
}

Game::PositionComponent *
ColliderComponent::position(void) const
{
	return(PIMPL->position);
}

Game::SizeComponent *
ColliderComponent::size(void) const
{
	return(PIMPL->size);
}

bool
ColliderComponent::collision(ColliderComponent &, float, const CollisionData &)
{
	return(false);
}

const Core::Type &
ColliderComponent::Type(void)
{
	static const Core::Type s_type("Game::ColliderComponent");
	return(s_type);
}

/************************************************** BounceColliderComponent */

BounceColliderComponent::BounceColliderComponent(const Core::Identifier &i, IEntity &e)
    : ColliderComponent(i, e)
{
}

bool
BounceColliderComponent::collision(ColliderComponent &c, float d, const CollisionData &data)
{
	MMUNUSED(c);
	MMUNUSED(d);
	MMUNUSED(data);

	const Math::Vector2 &l_vel = movement()->velocity();
	const float l_mag = l_vel.magnitude();
	const Math::Vector2 l_normal = l_vel.normalized(l_mag);
	Math::Vector2 l_pvel = (l_normal * (2.f * l_normal.dot(l_vel * -1.f)) + l_vel);
	movement()->velocity() = l_pvel.normalize(l_pvel.magnitude()) * l_mag;

	return(true);
}

const Core::Type &
BounceColliderComponent::Type(void)
{
	static const Core::Type s_type("Game::BounceColliderComponent");
	return(s_type);
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

