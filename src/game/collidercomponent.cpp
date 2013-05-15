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

#include <cstdio>
#include <cstring>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

#define DELTA_STEPS 8

struct ColliderComponent::Private
{
	Private(ColliderComponent &i)
	    : component(i)
	    , layer(0)
	    , movement(0)
	    , position(0)
	    , size(0)
	    , body(Box)
	    , bullet_resolution(DELTA_STEPS)
	    , flags(Active)
	    , init(false)
	{}

	~Private(void);

	inline bool
	isColliding(ColliderComponent &c, float d, CollisionData *data) const;

	inline bool
	hasFlag(ColliderComponent::Flags flag) const;

	inline float
	radius2(void) const;

	inline void
	update(float d);

	ColliderComponent   &component;
	CollisionSceneLayer *layer;
	MovementComponent   *movement;
	PositionComponent   *position;
	SizeComponent       *size;
	BodyType body;
	int  bullet_resolution;
	int  flags;
	bool init;
};

ColliderComponent::Private::~Private(void)
{
	if (layer) layer->deregisterCollider(&component);
}

bool
ColliderComponent::Private::isColliding(ColliderComponent &cc, float d, CollisionData *data) const
{
	ColliderComponent::Private &c = *cc.PIMPL;

	if (!movement || !c.position)
		return(false);

	const Math::Point2 l_pos_a = movement->simulate(d);
	const Math::Point2 l_pos_b =
	    (c.movement != 0 ? c.movement->simulate(d) : c.position->position());

	switch(body) {

	case Box: {
		const Math::Size2f l_size_a = size->size() / 2.f;
		const Math::Size2f l_size_b = c.size->size() / 2.f;

		const float l =
		    (l_pos_a.x + l_size_a.width) - (l_pos_b.x - l_size_b.width);
		if (l <= 0) return(false);

		const float r =
		    (l_pos_b.x + l_size_b.width) - (l_pos_a.x - l_size_a.width);
		if (r <= 0) return(false);

		const float t =
		    (l_pos_b.y + l_size_b.height) - (l_pos_a.y - l_size_a.height);
		if (t <= 0) return(false);

		const float b =
		    (l_pos_a.y + l_size_a.height) - (l_pos_b.y - l_size_b.height);
		if (b <= 0) return(false);

		if (data) {
			data->velocity.x = -movement->velocityX();
			data->velocity.y = -movement->velocityX();
			if (c.movement != 0) {
				data->velocity.x += c.movement->velocityX();
				data->velocity.y += c.movement->velocityY();
			}

			data->box.left = l;
			data->box.right = r;
			data->box.top = t;
			data->box.bottom = b;

		}
	} return(true);

	case Sphere: {
		float l_distance2 = l_pos_b.difference(l_pos_a).magnitude2();
		l_distance2 -= c.radius2() + radius2();
		if (l_distance2 >= 0) return(false);
		
		if (data) { 
			data->velocity.x = -movement->velocityX();
			data->velocity.y = -movement->velocityX();
			if (c.movement != 0) {
				data->velocity.x += c.movement->velocityX();
				data->velocity.y += c.movement->velocityY();
			}

			data->sphere.penetration2 = l_distance2;
		}
	} return(true);

	default: MMWARNING("Unknown collider body type encountered!");
		 break;
	}

	return(false);
}

bool
ColliderComponent::Private::hasFlag(ColliderComponent::Flags flag) const
{
	return((flags & flag) == flag);
}

float
ColliderComponent::Private::radius2(void) const
{
	float l_radius2 = 0;
	if (size) {
		const Math::Size2f &l_size = size->size();
		l_radius2 = powf(l_size.width  / 2.f, 2)
		          + powf(l_size.height / 2.f, 2);
	}
	else MMWARNING("Collider component found no size component!");
	return(l_radius2);
}

void
ColliderComponent::Private::update(float d)
{
	if (!init) {
		if (!movement) {
			movement = static_cast<MovementComponent *>
			    (component.entity()->getComponentType(MovementComponent::Type()));
		}

		if (!position) {
			position = static_cast<PositionComponent *>
			    (component.entity()->getComponentType(PositionComponent::Type()));
		}

		if (!size) {
			size = static_cast<SizeComponent *>
			    (component.entity()->getComponentType(SizeComponent::Type()));
		}

		if (!layer) {
			layer = static_cast<CollisionSceneLayer *>
			    (component.entity()->layer()->scene()->getLayerType(CollisionSceneLayer::Type()));

			if (!layer) {
				MMWARNING("Collider component used with no collision scene layer!");
				return;
			}

			/* register as collider */
			layer->registerCollider(&component);
		}

		/*
		 * Movement component is not required, no movement = static collider
		 */
		init = (layer != 0 && position != 0 && size != 0 );
	}

	if (!init || !hasFlag(Active)) return;

	ColliderList::const_iterator l_i;
	ColliderList::const_iterator l_c = layer->colliders().end();

	for (l_i = layer->colliders().begin(); l_i != l_c; ++l_i) {
		if (*l_i == &component) continue;

		ColliderComponent *l_collider = *l_i;
		CollisionData data[2];
		memset(&data, 0, sizeof(data));

		if (hasFlag(Bullet)) {
			int l_steps = bullet_resolution;
			const float l_delta_step = d / float(l_steps);
			float l_bullet_delta = 0;

			for(int i = 1; i < l_steps; ++i) {
				if (component.isColliding(*l_collider, l_bullet_delta += l_delta_step, &data[0])) {
					l_collider->isColliding(component, l_bullet_delta, &data[1]);
					l_collider->collision(component, l_bullet_delta, data[1]);
					component.collision(*l_collider, l_bullet_delta, data[0]);
					break;
				}
			}
		}
		else {
			if (component.isColliding(*l_collider, d, &data[0])) {
				l_collider->isColliding(component, d, &data[1]);
				l_collider->collision(component, d, data[1]);
				component.collision(*l_collider, d, data[0]);
			}
		}
	}
}

ColliderComponent::ColliderComponent(const Core::Identifier &i,
                                     Game::IEntity *e)
    : Component(i, e)
    , PIMPL_CREATE_X(*this)
{
}

ColliderComponent::~ColliderComponent(void)
{
	PIMPL_DESTROY;
}

ColliderComponent::BodyType
ColliderComponent::body(void) const
{
	return(PIMPL->body);
}

void
ColliderComponent::setBody(BodyType b)
{
	PIMPL->body = b;
}

int
ColliderComponent::flags(void) const
{
	return(PIMPL->flags);
}

bool
ColliderComponent::hasFlag(ColliderComponent::Flags flag) const
{
	return((PIMPL->flags & flag) == flag);
}

void
ColliderComponent::clearFlag(ColliderComponent::Flags flag)
{
	PIMPL->flags &= ~flag;
}

void
ColliderComponent::setFlag(ColliderComponent::Flags flag)
{
	PIMPL->flags |= flag;
}

bool
ColliderComponent::active(void) const
{
	return(PIMPL->hasFlag(Active));
}

void
ColliderComponent::setActive(bool a)
{
	if (a) setFlag(Active);
	else clearFlag(Active);
}

bool
ColliderComponent::bullet(void) const
{
	return(PIMPL->hasFlag(Bullet));
}

void
ColliderComponent::setBullet(bool b)
{
	if (b) setFlag(Bullet);
	else clearFlag(Bullet);
}

int
ColliderComponent::bulletResolution(void) const
{
	return(PIMPL->bullet_resolution);
}

void
ColliderComponent::setBulletResolution(int r)
{
	PIMPL->bullet_resolution = r;
}

float
ColliderComponent::radius2(void) const
{
	return(PIMPL->radius2());
}

void
ColliderComponent::update(float d)
{
	PIMPL->update(d);
}

bool
ColliderComponent::isColliding(ColliderComponent &c, float d, CollisionData *data) const
{
	return(PIMPL->isColliding(c, d, data));
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

/************************************************** SimpleColliderComponent */

SimpleColliderComponent::SimpleColliderComponent(const Core::Identifier &i,
                                                 Game::IEntity *e)
    : ColliderComponent(i, e)
{
}

bool
SimpleColliderComponent::collision(ColliderComponent &c, float d, const CollisionData &data)
{
	MMUNUSED(c);
	MMUNUSED(d);

	/*
	 * We can't bounce if we can't move
	 */
	if (!active() || !movement()) return(false);

	Math::Vector2 l_vel = movement()->velocity();

	switch(body()) {

	case Box: {
		if (l_vel.x == 0 && l_vel.y == 0)
			break;
		else if (l_vel.x > 0 && !hasFlag(LockXAxis)
		                     && data.box.left < data.box.right
		                     && data.box.left < data.box.top
		                     && data.box.left < data.box.bottom) {
			l_vel.x *= -1;
			position()->translateX(-data.box.left);
		}
		else if (l_vel.x < 0 && !hasFlag(LockXAxis)
		                     && data.box.right < data.box.left
		                     && data.box.right < data.box.top
		                     && data.box.right < data.box.bottom) {
			l_vel.x *= -1;
			position()->translateX(data.box.right);
		}
		else if (l_vel.y > 0 && !hasFlag(LockYAxis)
		                     && data.box.bottom < data.box.top
		                     && data.box.bottom < data.box.left
		                     && data.box.bottom < data.box.right) {
			l_vel.y *= -1;
			position()->translateY(-data.box.bottom);
		}
		else if (l_vel.y < 0 && !hasFlag(LockYAxis)
		                     && data.box.top < data.box.bottom
		                     && data.box.top < data.box.left
		                     && data.box.top < data.box.right) {
			l_vel.y *= -1;
			position()->translateY(data.box.top);
		}
		else break;
			
		movement()->setVelocity(l_vel);
	} break;

	/* TODO: FIX */
	case Sphere: {
		const float l_mag = l_vel.magnitude();
		const Math::Vector2 l_normal = l_vel.normalized(l_mag);
		Math::Vector2 l_pvel = (l_normal * (2.f * l_normal.dot(l_vel * -1.f)) + l_vel);
		movement()->setVelocity(l_pvel.normalize(l_pvel.magnitude()) * l_mag);
	} break;

	default:
		MMWARNING("Unknown collider body type encountered!");
		return(false);
	};

	return(true);
}

const Core::Type &
SimpleColliderComponent::Type(void)
{
	static const Core::Type s_type("Game::SimpleColliderComponent");
	return(s_type);
}


/************************************************** BounceColliderComponent */

BounceColliderComponent::BounceColliderComponent(const Core::Identifier &i,
                                                 Game::IEntity *e)
    : ColliderComponent(i, e)
{
}

bool
BounceColliderComponent::collision(ColliderComponent &c, float d, const CollisionData &data)
{
	MMUNUSED(c);
	MMUNUSED(d);

	/*
	 * We can't bounce if we can't move
	 */
	if (!active() || !movement()) return(false);

	Math::Vector2 l_vel = movement()->velocity();

	switch(body()) {

	case Box: {
		/*
		 * We hit something's left while moving right
		 */
		if (l_vel.x > 0 && !hasFlag(LockXAxis)
		                && data.box.left < data.box.right
		                && data.box.left < data.box.top
		                && data.box.left < data.box.bottom) {
			l_vel.x -= data.velocity.x / 2.f;
			l_vel.x /= -2.f;
			position()->translateX(-data.box.left);
		}

		/*
		 * We hit something's right while moving left
		 */
		else if (l_vel.x < 0 && !hasFlag(LockXAxis)
		                     && data.box.right < data.box.left
		                     && data.box.right < data.box.top
		                     && data.box.right < data.box.bottom) {
			l_vel.x -= data.velocity.x / 2.f;
			l_vel.x /= -2.f;
			position()->translateX(data.box.right);
		}

		/*
		 * We hit something's bottom while moving up
		 */
		else if (l_vel.y > 0 && !hasFlag(LockYAxis)
		                     && data.box.bottom < data.box.top
		                     && data.box.bottom < data.box.left
		                     && data.box.bottom < data.box.right) {
			l_vel.y -= data.velocity.y / 2.f;
			l_vel.y /= -2.f;
			position()->translateY(-data.box.bottom);
		}

		/*
		 * We hit somethings top while moving down
		 */
		else if (l_vel.y < 0 && !hasFlag(LockYAxis)
		                     && data.box.top < data.box.bottom
		                     && data.box.top < data.box.left
		                     && data.box.top < data.box.right) {
			l_vel.y -= data.velocity.y / 2.f;
			l_vel.y /= -2.f;
			position()->translateY(data.box.top);
		}

		/*
		 * Something's right hit us while moving right (pushed)
		 */
		else if (l_vel.x >= 0 && !hasFlag(LockXAxis)
		                      && data.box.right < data.box.left
		                      && data.box.right < data.box.top
		                      && data.box.right < data.box.bottom) {
			l_vel.x += data.velocity.x / 2.f;
			position()->translateX(data.box.right);
		}

		/*
		 * Something's left hit us while moving left (pushed)
		 */
		else if (l_vel.x <= 0 && !hasFlag(LockXAxis)
		                      && data.box.left < data.box.right
		                      && data.box.left < data.box.top
		                      && data.box.left < data.box.bottom) {
			l_vel.x += data.velocity.x / 2.f;
			position()->translateX(-data.box.left);
		}

		/*
		 * Something's top hit us while moving up (pushed)
		 */
		else if (l_vel.y >= 0 && !hasFlag(LockYAxis)
		                      && data.box.top < data.box.bottom
		                      && data.box.top < data.box.left
		                      && data.box.top < data.box.right) {
			l_vel.y += data.velocity.y / 2.f;
			position()->translateY(data.box.top);
		}

		/*
		 * Something's bottom hit us while moving down (pushed)
		 */
		else if (l_vel.y <= 0 && !hasFlag(LockYAxis)
		                      && data.box.bottom < data.box.top
		                      && data.box.bottom < data.box.left
		                      && data.box.bottom < data.box.right) {
			l_vel.y += data.velocity.y / 2.f;
			position()->translateY(-data.box.bottom);
		}
		else break;
			
		movement()->setVelocity(l_vel);
	} break;

	/* TODO: FIX */
	case Sphere: {
		const float l_mag = l_vel.magnitude();
		const Math::Vector2 l_normal = l_vel.normalized(l_mag);
		Math::Vector2 l_pvel = (l_normal * (2.f * l_normal.dot(l_vel * -1.f)) + l_vel);
		movement()->setVelocity(l_pvel.normalize(l_pvel.magnitude()) * l_mag);
	} break;

	default:
		MMWARNING("Unknown collider body type encountered!");
		return(false);
	};

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

