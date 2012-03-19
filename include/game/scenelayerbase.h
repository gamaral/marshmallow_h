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

#ifndef MARSHMALLOW_GAME_SCENELAYERBASE_H
#define MARSHMALLOW_GAME_SCENELAYERBASE_H 1

#include <game/iscenelayer.h>

#include <core/fd.h>
#include <core/global.h>
#include <core/identifier.h>

MARSHMALLOW_NAMESPACE_BEGIN

namespace Game
{

	/*! @brief Game Scene Layer Base Class */
	class MARSHMALLOW_GAME_EXPORT
	SceneLayerBase : public ISceneLayer
	{
		Core::Identifier m_id;
		IScene &m_scene;
		int m_flags;
		bool m_killed;

		NO_ASSIGN_COPY(SceneLayerBase);
	public:

		SceneLayerBase(const Core::Identifier &identifier,
		    IScene &scene, int flags = slfNone);
		virtual ~SceneLayerBase(void);

	public: /* virtual */

		VIRTUAL const Core::Identifier & id(void) const
		    { return(m_id); }

		VIRTUAL IScene &scene(void)
		    { return(m_scene); }

		VIRTUAL int flags(void) const
		    { return(m_flags); }

		VIRTUAL void kill(void)
		    { m_killed = true; }
		VIRTUAL bool isZombie(void) const
		    { return(m_killed); }

		VIRTUAL bool serialize(TinyXML::TiXmlElement &node) const;
		VIRTUAL bool deserialize(TinyXML::TiXmlElement &node);
	};
	typedef Core::Shared<SceneLayerBase> SharedSceneLayerBase;
	typedef Core::Weak<SceneLayerBase> WeakSceneLayerBase;

}

MARSHMALLOW_NAMESPACE_END

#endif
