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

#ifndef MARSHMALLOW_GAME_ISCENE_H
#define MARSHMALLOW_GAME_ISCENE_H 1

#include <core/irenderable.h>
#include <core/iserializable.h>
#include <core/iupdateable.h>

#include <list>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Core { /******************************************** Core Namespace */
	class Identifier;
	class Type;
} /*********************************************************** Core Namespace */

namespace Graphics { /************************************ Graphics Namespace */
	class Color;
} /******************************************************* Graphics Namespace */

namespace Game { /******************************************** Game Namespace */

	struct ISceneLayer;
	typedef Core::Shared<ISceneLayer> SharedSceneLayer;

	typedef std::list<SharedSceneLayer> SceneLayerList;

	/*! @brief Game Scene Interface */
	struct MARSHMALLOW_GAME_EXPORT
	IScene : public Core::IRenderable
	       , public Core::IUpdateable
	       , public Core::ISerializable
	{
		virtual ~IScene(void);

		virtual const Core::Identifier & id(void) const = 0;
		virtual const Core::Type & type(void) const = 0;

		virtual void pushLayer(SharedSceneLayer layer) = 0;
		virtual void popLayer(void) = 0;
		virtual void removeLayer(const Core::Identifier &identifier) = 0;
		virtual SharedSceneLayer getLayer(const Core::Identifier &identifier) const = 0;
		virtual SharedSceneLayer getLayerType(const Core::Type &type) const = 0;
		virtual const SceneLayerList & getLayers(void) const = 0;

		virtual const Graphics::Color & background(void) const = 0;

		virtual void activate(void) = 0;
		virtual void deactivate(void) = 0;
	};
	typedef Core::Shared<IScene> SharedScene;
	typedef Core::Weak<IScene> WeakScene;

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
