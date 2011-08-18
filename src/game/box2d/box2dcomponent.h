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

#ifndef GAME_BOX2DCOMPONENT_H
#define GAME_BOX2DCOMPONENT_H 1

#include "game/componentbase.h"

#include "math/size2.h"

struct b2Body;

MARSHMALLOW_NAMESPACE_BEGIN

namespace Game
{
	class Box2DSceneLayer;
	typedef Core::Weak<Box2DSceneLayer> WeakBox2DSceneLayer;

	class PositionComponent;
	typedef Core::Weak<PositionComponent> WeakPositionComponent;

	class RenderComponent;
	typedef Core::Weak<RenderComponent> WeakRenderComponent;

	/*! @brief Game Box2D Component Class */
	class GAME_EXPORT Box2DComponent : public ComponentBase
	{
		WeakBox2DSceneLayer m_b2layer;
		WeakPositionComponent m_position;
		WeakRenderComponent m_render;
		Math::Size2f m_size;
		b2Body* m_body;
		int   m_body_type;
		float m_density;
		float m_friction;
		bool  m_init;

		NO_COPY(Box2DComponent);

	public:

		Box2DComponent(const Core::Identifier &identifier, IEntity &entity);
		virtual ~Box2DComponent(void);

		b2Body * body(void)
		    { return(m_body); }

		int & bodyType(void)
		    { return(m_body_type); }

		float & density(void)
		    { return(m_density); }

		float & friction(void)
		    { return(m_friction); }

		Math::Size2f &size(void)
		    { return(m_size); }

	public: /* virtual */

		VIRTUAL const Core::Type & type(void) const
		    { return(Type()); }

		VIRTUAL void update(TIME delta);

		VIRTUAL bool serialize(TinyXML::TiXmlElement &node) const;
		VIRTUAL bool deserialize(TinyXML::TiXmlElement &node);

	public: /* static */

		static const Core::Type & Type(void);

	private: /* static */

		static const Core::Type sType;
	};
	typedef Core::Shared<Box2DComponent> SharedBox2DComponent;
	typedef Core::Weak<Box2DComponent> WeakBox2DComponent;

}

MARSHMALLOW_NAMESPACE_END

#endif
