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

#ifndef GAME_IENTITYFACTORY_H
#define GAME_IENTITYFACTORY_H 1

#include "core/global.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Core
{
	class StrHash;
	typedef StrHash Identifier;
	typedef StrHash Type;

	template <class T> class Shared;
	template <class T> class Weak;
}

namespace Game
{

	struct IEntity;
	typedef Core::Shared<IEntity> SharedEntity;
	typedef Core::Weak<IEntity> WeakEntity;

	struct IScene;

	/*! @brief Game EntityFactory Interface */
	struct GAME_EXPORT IEntityFactory
	{
		virtual ~IEntityFactory(void) {};

		virtual SharedEntity createEntity(const Core::Type &type,
		    const Core::Identifier &identifier, IScene &scene) const = 0;
	};
	typedef Core::Shared<IEntityFactory> SharedEntityFactory;
	typedef Core::Weak<IEntityFactory> WeakEntityFactory;

}

MARSHMALLOW_NAMESPACE_END

#endif
