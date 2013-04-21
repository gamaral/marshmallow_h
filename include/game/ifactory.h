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

#ifndef MARSHMALLOW_GAME_IFACTORY_H
#define MARSHMALLOW_GAME_IFACTORY_H 1

#include <core/environment.h>
#include <core/fd.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */
	struct ITexture;
	typedef Core::Shared<ITexture> SharedTexture;

	struct IMesh;
	typedef Core::Shared<IMesh> SharedMesh;
} /******************************************************* Graphics Namespace */

namespace Game { /******************************************** Game Namespace */

	class EntitySceneLayer;

	struct IComponent;
	typedef Core::Shared<IComponent> SharedComponent;

	struct IEntity;
	typedef Core::Shared<IEntity> SharedEntity;

	struct IScene;
	typedef Core::Shared<IScene> SharedScene;

	struct ISceneLayer;
	typedef Core::Shared<ISceneLayer> SharedSceneLayer;

	/*! @brief Game Factory Interface */
	struct MARSHMALLOW_GAME_EXPORT
	IFactory
	{
		virtual ~IFactory(void);

		virtual SharedScene createScene(const Core::Type &type,
		    const Core::Identifier &identifier) const = 0;

		virtual SharedSceneLayer createSceneLayer(const Core::Type &type,
		    const Core::Identifier &identifier, IScene &scene) const = 0;

		virtual SharedEntity createEntity(const Core::Type &type,
		    const Core::Identifier &identifier, EntitySceneLayer &layer) const = 0;

		virtual SharedComponent createComponent(const Core::Type &type,
		    const Core::Identifier &identifier, IEntity &entity) const = 0;

		virtual Graphics::SharedMesh createMesh(const Core::Type &type) const = 0;
	};
	typedef Core::Shared<IFactory> SharedFactory;
	typedef Core::Weak<IFactory> WeakFactory;

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
