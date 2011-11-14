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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef GAME_COLLIDERCOMPONENT_H
#define GAME_COLLIDERCOMPONENT_H 1

#include "game/componentbase.h"

#include "core/weak.h"

#include "math/size2.h"
#include "math/vector2.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Game
{
	class CollisionSceneLayer;
	typedef Core::Weak<CollisionSceneLayer> WeakCollisionSceneLayer;

	class MovementComponent;
	typedef Core::Weak<MovementComponent> WeakMovementComponent;

	class PositionComponent;
	typedef Core::Weak<PositionComponent> WeakPositionComponent;

	class SizeComponent;
	typedef Core::Weak<SizeComponent> WeakSizeComponent;

	/*! @brief Game Collider Component Class */
	class GAME_EXPORT ColliderComponent : public ComponentBase
	{
		WeakCollisionSceneLayer m_layer;
		WeakMovementComponent m_movement;
		WeakPositionComponent m_position;
		WeakSizeComponent m_size;
		int  m_body;
		bool m_active;
		bool m_bullet;
		bool m_init;

		NO_COPY(ColliderComponent);

	public:

		enum BodyType {
			SphereType,
			BoxType,
			CapsuleType
		};

	public:

		ColliderComponent(const Core::Identifier &identifier, IEntity &entity);
		virtual ~ColliderComponent(void);

		int &body(void)
		    { return(m_body); }

		bool &active(void)
		    { return(m_active); }

		bool &bullet(void)
		    { return(m_bullet); }

		float radius2(void) const;

		bool isColliding(ColliderComponent& collider, float *penetration = 0) const;

	public: /* virtual */

		VIRTUAL const Core::Type & type(void) const
		    { return(Type()); }

		VIRTUAL void update(float delta);

		VIRTUAL bool serialize(TinyXML::TiXmlElement &node) const;
		VIRTUAL bool deserialize(TinyXML::TiXmlElement &node);

	public: /* static */

		static const Core::Type & Type(void);

	protected:
	
		virtual bool collision(ColliderComponent& collider, float penetration, float delta);

		WeakCollisionSceneLayer &layer(void)
		    { return(m_layer); }
		WeakMovementComponent &movement(void)
		    { return(m_movement); }
		WeakPositionComponent &position(void)
		    { return(m_position); }
		WeakSizeComponent &size(void)
		    { return(m_size); }

	private: /* static */

		static const Core::Type sType;
	};
	typedef Core::Shared<ColliderComponent> SharedColliderComponent;
	typedef Core::Weak<ColliderComponent> WeakColliderComponent;

	/*! @brief Game Bounce Collider Component Class */
	class GAME_EXPORT BounceColliderComponent : public ColliderComponent
	{
		Math::Vector2 m_factor;
		NO_COPY(BounceColliderComponent);

	public:

		BounceColliderComponent(const Core::Identifier &identifier, IEntity &entity);
		virtual ~BounceColliderComponent(void) {};

		Math::Vector2 &factor(void)
		    { return(m_factor); }

	protected:
	
		VIRTUAL bool collision(ColliderComponent& collider, float penetration, float delta);
	};
	typedef Core::Shared<BounceColliderComponent> SharedBounceColliderComponent;
	typedef Core::Weak<BounceColliderComponent> WeakBounceColliderComponent;
}

MARSHMALLOW_NAMESPACE_END

#endif
