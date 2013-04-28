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

#ifndef MARSHMALLOW_GAME_COLLISIONSCENELAYER_H
#define MARSHMALLOW_GAME_COLLISIONSCENELAYER_H 1

#include <game/collidercomponent.h>
#include <game/scenelayerbase.h>

#include <list>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Math { /******************************************** Math Namespace */
	struct Vector2;
} /*********************************************************** Math Namespace */

namespace Game { /******************************************** Game Namespace */

	class ColliderComponent;
	typedef std::list<ColliderComponent *> ColliderList;

	/*! @brief Game Collision Scene Layer Class */
	class MARSHMALLOW_GAME_EXPORT
	CollisionSceneLayer : public SceneLayerBase
	{
		struct Private;
		Private *m_p;

		NO_ASSIGN_COPY(CollisionSceneLayer);
	public:

		CollisionSceneLayer(const Core::Identifier &identifier,
		    IScene &scene);
		virtual ~CollisionSceneLayer(void);

		void registerCollider(ColliderComponent &collider);
		void deregisterCollider(ColliderComponent &collider);

		const ColliderList & colliders(void) const;

	public: /* virtual */

		VIRTUAL const Core::Type & type(void) const
		    { return(Type()); }

		VIRTUAL void render(void) {}
		VIRTUAL void update(float delta);

		VIRTUAL bool serialize(XMLElement &node) const;
		VIRTUAL bool deserialize(XMLElement &node);

	public: /* static */

		static const Core::Type & Type(void);
	};

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
