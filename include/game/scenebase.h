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

#ifndef MARSHMALLOW_GAME_SCENEBASE_H
#define MARSHMALLOW_GAME_SCENEBASE_H 1

#include <game/iscene.h>

#include <core/identifier.h>
#include <core/shared.h>
#include <core/type.h>

MARSHMALLOW_NAMESPACE_BEGIN

namespace Game
{
	/*! @brief Game Scene Base Class */
	class MARSHMALLOW_GAME_EXPORT
	SceneBase : public IScene
	{
		struct Private;
		Private *m_p;

		NO_ASSIGN_COPY(SceneBase);
	public:

		SceneBase(const Core::Identifier &identifier);
		virtual ~SceneBase(void);

	public: /* virtual */

		VIRTUAL const Core::Identifier & id(void) const;

		VIRTUAL void pushLayer(SharedSceneLayer layer);
		VIRTUAL void popLayer(void);
		VIRTUAL void removeLayer(const Core::Identifier &identifier);
		VIRTUAL SharedSceneLayer getLayer(const Core::Identifier &identifier) const;
		VIRTUAL SharedSceneLayer getLayerType(const Core::Type &type) const;
		VIRTUAL const SceneLayerList & getLayers(void) const;

		VIRTUAL void activate(void) {}
		VIRTUAL void deactivate(void) {}

		VIRTUAL void render(void);
		VIRTUAL void update(float delta);

		VIRTUAL bool serialize(TinyXML::TiXmlElement &node) const;
		VIRTUAL bool deserialize(TinyXML::TiXmlElement &node);
	};
	typedef Core::Shared<SceneBase> SharedSceneBase;
	typedef Core::Weak<SceneBase> WeakSceneBase;
}

MARSHMALLOW_NAMESPACE_END

#endif
