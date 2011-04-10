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

#ifndef GAME_SCENEBASE_H
#define GAME_SCENEBASE_H 1

#include "game/iscene.h"

#include "EASTL/list.h"
using namespace eastl;

MARSHMALLOW_NAMESPACE_BEGIN

namespace Game
{
	class IEntity;
	typedef Core::Shared<IEntity> SharedEntity;

	typedef list<SharedEntity> EntityList;

	/*! @brief Game Scene Base Class */
	class GAME_EXPORT SceneBase : public IScene
	{
		EntityList m_entities;
		Core::Identifier m_id;

	public:

		SceneBase(const Core::Identifier &identifier);
		virtual ~SceneBase(void);

		void addEntity(SharedEntity &entity);
		void removeEntity(const SharedEntity &entity);
		SharedEntity entity(const Core::Identifier &identifier) const;
		const EntityList & entities(void) const;

	public: /* virtual */

		VIRTUAL const Core::Identifier & id(void) const
		    { return(m_id); }

		VIRTUAL const Core::Type & type(void) const
		    { return(Type); }

		VIRTUAL void activate(void);
		VIRTUAL void deactivate(void);
		VIRTUAL void update(void);

	public: /* static */

		static Core::Type Type;
	};

}

MARSHMALLOW_NAMESPACE_END

#endif
