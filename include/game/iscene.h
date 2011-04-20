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

#ifndef GAME_ISCENE_H
#define GAME_ISCENE_H 1

#include "EASTL/list.h"
using namespace eastl;

#include "core/identifier.h"
#include "core/irenderable.h"
#include "core/iupdateable.h"
#include "core/shared.h"
#include "core/type.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Game
{

	struct IEntity;
	typedef Core::Shared<IEntity> SharedEntity;

	typedef list<SharedEntity> EntityList;

	/*! @brief Game Scene Interface */
	struct GAME_EXPORT IScene : public Core::IRenderable,
	                            public Core::IUpdateable
	{
		virtual ~IScene(void) {};

		virtual const Core::Identifier & id(void) const = 0;
		virtual const Core::Type & type(void) const = 0;

		virtual void addEntity(SharedEntity &entity) = 0;
		virtual void removeEntity(const SharedEntity &entity) = 0;
		virtual SharedEntity entity(const Core::Identifier &identifier) const = 0;
		virtual const EntityList & entities(void) const = 0;

		virtual void activate(void) = 0;
		virtual void deactivate(void) = 0;
	};
	typedef Core::Shared<IScene> SharedScene;

}

MARSHMALLOW_NAMESPACE_END

#endif
