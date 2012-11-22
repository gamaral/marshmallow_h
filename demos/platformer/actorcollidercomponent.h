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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef DEMO_ACTORCOLLIDERCOMPONENT_H
#define DEMO_ACTORCOLLIDERCOMPONENT_H 1

#include <game/collidercomponent.h>

MARSHMALLOW_NAMESPACE_USE

namespace Common { class ActorEntity; }

/*! @brief Demo Collider Component Class */
class ActorColliderComponent : public Game::ColliderComponent
{
	bool m_enabled;
	bool m_platform;

	NO_ASSIGN_COPY(ActorColliderComponent);
public:

	ActorColliderComponent(const Core::Identifier &identifier, Common::ActorEntity &entity);
	virtual ~ActorColliderComponent(void) {};

	bool onPlatform(void) const
	    { return(m_platform); }

	inline void enable()
	    { m_enabled = true; }
	inline void disable()
	    { m_enabled = false; }
	inline bool isEnabled() const
	    { return(m_enabled); }

public: /* virtual */

	VIRTUAL void update(float delta);

protected: /* virtual */

	VIRTUAL bool collision(ColliderComponent &collider, float delta, const Game::CollisionData &data);
};
typedef Core::Shared<ActorColliderComponent> SharedActorColliderComponent;
typedef Core::Weak<ActorColliderComponent> WeakActorColliderComponent;

#endif
