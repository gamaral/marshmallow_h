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

#ifndef MARSHMALLOW_GAME_ENTITYBASE_H
#define MARSHMALLOW_GAME_ENTITYBASE_H 1

#include <game/ientity.h>

#include <core/global.h>
#include <core/identifier.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

	class EntitySceneLayer;

	/*! @brief Game Entity Base Class */
	class MARSHMALLOW_GAME_EXPORT
	EntityBase : public IEntity
	{
		struct Private;
		Private *m_p;

		NO_ASSIGN_COPY(EntityBase);
	public:

		EntityBase(const Core::Identifier &identifier, EntitySceneLayer &layer);
		virtual ~EntityBase(void);

	public: /* virtual */

		VIRTUAL const Core::Identifier & id(void) const;
		VIRTUAL EntitySceneLayer & layer(void);

		VIRTUAL void pushComponent(const SharedComponent &component);
		VIRTUAL void popComponent(void);
		VIRTUAL void removeComponent(const Core::Identifier &identifier);
		VIRTUAL void removeComponent(const SharedComponent &component);
		VIRTUAL SharedComponent getComponent(const Core::Identifier &identifier) const;
		VIRTUAL SharedComponent getComponentType(const Core::Type &type) const;

		VIRTUAL void render(void);
		VIRTUAL void update(float delta);

		VIRTUAL void kill(void);
		VIRTUAL bool isZombie(void) const;

		VIRTUAL bool serialize(XMLElement &node) const;
		VIRTUAL bool deserialize(XMLElement &node);
	};

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
