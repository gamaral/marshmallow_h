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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef MARSHMALLOW_GAME_COLLIDERCOMPONENT_H
#define MARSHMALLOW_GAME_COLLIDERCOMPONENT_H 1

#include <game/component.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

	class CollisionSceneLayer;
	class MovementComponent;
	class PositionComponent;
	class SizeComponent;

	union CollisionData;

	/*! @brief Game Collider Component Class */
	class MARSHMALLOW_GAME_EXPORT
	ColliderComponent : public Component
	{
		PRIVATE_IMPLEMENTATION
		NO_ASSIGN_COPY(ColliderComponent);
	public:

		enum BodyType {
			Box = 0,
			Sphere
		};

		enum Flags {
			Static    = 0,
			Active    = (1 << 0),
			Bullet    = (1 << 1),
			LockXAxis = (1 << 2),
			LockYAxis = (1 << 3)
		};

		/*! @brief Game Collision Data */
		union CollisionData
		{
			struct {
				float x;
				float y;
			} velocity;

			struct {
				float velocity[2];
				float left;
				float right;
				float top;
				float bottom;
			} box;

			struct {
				float velocity[2];
				float penetration2;
			} sphere;
		};

	public:

		ColliderComponent(const Core::Identifier &identifier,
		                  Game::IEntity *entity);
		virtual ~ColliderComponent(void);

		BodyType body(void) const;
		void setBody(BodyType body);

		int flags(void) const;
		bool hasFlag(ColliderComponent::Flags flag) const;
		void clearFlag(ColliderComponent::Flags flag);
		void setFlag(ColliderComponent::Flags flag);

		bool active(void) const;
		void setActive(bool active);

		bool bullet(void) const;
		void setBullet(bool bullet);

		int bulletResolution(void) const;
		void setBulletResolution(int resolution);

		float radius2(void) const;

	public: /* reimp */

		VIRTUAL const Core::Type & type(void) const
		    { return(Type()); }

		VIRTUAL void update(float delta);

	protected:

		bool isColliding(ColliderComponent &collider,
		                 float delta,
		                 CollisionData *data = 0) const;

		Game::CollisionSceneLayer * layer(void) const;
		Game::MovementComponent * movement(void) const;
		Game::PositionComponent * position(void) const;
		Game::SizeComponent * size(void) const;

	protected: /* virtual */
	
		virtual bool collision(ColliderComponent &collider,
		                       float delta,
		                       const CollisionData &data);

	public: /* static */

		static const Core::Type & Type(void);
	};

	/*! @brief Game Simple Collider Component Class
	 *  
	 *  This inverts velocity on collision, nothing more, nothing less.
	 */
	class MARSHMALLOW_GAME_EXPORT
	SimpleColliderComponent : public ColliderComponent
	{
		NO_ASSIGN_COPY(SimpleColliderComponent);
	public:

		SimpleColliderComponent(const Core::Identifier &identifier,
		                        Game::IEntity *entity);
		virtual ~SimpleColliderComponent(void) {};

	public: /* reimp */

		VIRTUAL const Core::Type & type(void) const
		    { return(Type()); }

	protected:
	
		VIRTUAL bool collision(ColliderComponent &collider, float delta, const CollisionData &data);

	public: /* static */

		static const Core::Type & Type(void);
	};

	/*! @brief Game Bounce Collider Component Class
	 *
	 *  This is a bit closer to reality compared to simple collider.
	 */
	class MARSHMALLOW_GAME_EXPORT
	BounceColliderComponent : public ColliderComponent
	{
		NO_ASSIGN_COPY(BounceColliderComponent);
	public:

		BounceColliderComponent(const Core::Identifier &identifier,
		                        Game::IEntity *entity);
		virtual ~BounceColliderComponent(void) {};

	public: /* reimp */

		VIRTUAL const Core::Type & type(void) const
		    { return(Type()); }

	protected:
	
		VIRTUAL bool collision(ColliderComponent &collider, float delta, const CollisionData &data);

	public: /* static */

		static const Core::Type & Type(void);
	};

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
