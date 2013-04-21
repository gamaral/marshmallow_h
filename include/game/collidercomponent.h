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

#include <game/componentbase.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

	class CollisionSceneLayer;
	typedef Core::Weak<CollisionSceneLayer> WeakCollisionSceneLayer;

	class MovementComponent;
	typedef Core::Weak<MovementComponent> WeakMovementComponent;

	class PositionComponent;
	typedef Core::Weak<PositionComponent> WeakPositionComponent;

	class SizeComponent;
	typedef Core::Weak<SizeComponent> WeakSizeComponent;

	union CollisionData;

	/*! @brief Game Collider Component Class */
	class MARSHMALLOW_GAME_EXPORT
	ColliderComponent : public ComponentBase
	{
		struct Private;
		Private *m_p;

		NO_ASSIGN_COPY(ColliderComponent);
	public:

		enum BodyType {
			btBox,
			btCapsule,
			btSphere
		};

	public:

		ColliderComponent(const Core::Identifier &identifier, IEntity &entity);
		virtual ~ColliderComponent(void);

		int & body(void);
		bool & active(void);
		bool & bullet(void);
		int & bulletResolution(void);

		float radius2(void) const;

		bool isColliding(ColliderComponent& collider, float delta, CollisionData *data = 0) const;

	public: /* virtual */

		VIRTUAL const Core::Type & type(void) const
		    { return(Type()); }

		VIRTUAL void update(float delta);

		VIRTUAL bool serialize(XMLElement &node) const;
		VIRTUAL bool deserialize(XMLElement &node);

		WeakCollisionSceneLayer & layer(void);
		WeakMovementComponent & movement(void);
		WeakPositionComponent & position(void);
		WeakSizeComponent & size(void);

	public: /* static */

		static const Core::Type & Type(void);

	protected:
	
		virtual bool collision(ColliderComponent &collider, float delta, const CollisionData &data);
	};
	typedef Core::Shared<ColliderComponent> SharedColliderComponent;
	typedef Core::Weak<ColliderComponent> WeakColliderComponent;

	/*! @brief Game Bounce Collider Component Class */
	class MARSHMALLOW_GAME_EXPORT
	BounceColliderComponent : public ColliderComponent
	{
		NO_ASSIGN_COPY(BounceColliderComponent);
	public:

		BounceColliderComponent(const Core::Identifier &identifier, IEntity &entity);
		virtual ~BounceColliderComponent(void) {};

	public: /* virtual */

		VIRTUAL const Core::Type & type(void) const
		    { return(Type()); }

	protected:
	
		VIRTUAL bool collision(ColliderComponent &collider, float delta, const CollisionData &data);

	public: /* static */

		static const Core::Type & Type(void);
	};
	typedef Core::Shared<BounceColliderComponent> SharedBounceColliderComponent;
	typedef Core::Weak<BounceColliderComponent> WeakBounceColliderComponent;

	/*! @brief Game Collision Data */
	union CollisionData
	{
		struct {
			float penetration2;
		} sphere;

		struct {
			float left;
			float right;
			float top;
			float bottom;
		} rect;
	};

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
