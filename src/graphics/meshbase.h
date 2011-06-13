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

#ifndef GRAPHICS_MESHBASE_H
#define GRAPHICS_MESHBASE_H 1

#include "graphics/imesh.h"

#include "core/shared.h"
#include "math/vector2.h"
#include "graphics/color.h"
#include "graphics/textureasset.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Graphics
{

	/*! @brief Graphics Mesh Base Class */
	class GRAPHICS_EXPORT MeshBase : public IMesh
	{
		Graphics::SharedMeshData m_data;
		Graphics::Color m_color;
		Graphics::SharedTextureAsset m_texture;
		float m_scale[2];
		float m_rotation;

		NO_COPY(MeshBase);

	public:

		MeshBase(Graphics::SharedMeshData d);
		virtual ~MeshBase(void);

		void setColor(const Graphics::Color &color);

		void setTexture(SharedTextureAsset texture);

		void setRotation(float angle);

		void setScale(float x, float y)
		    { m_scale[0] = x; m_scale[1] = y; }
		
	public: /* operators */

		Math::Vector2 operator[](int index) const
		    { return(vertex(index)); }

	public: /* virtual */

		virtual Math::Vector2 vertex(int index) const;
		virtual void setVertex(int index, const Math::Vector2 &vertex);
		virtual void textureCoord(int index, float &u, float &v) const;
		virtual void setTextureCoord(int index, float u, float v);

		VIRTUAL const Graphics::SharedMeshData &data(void) const
		    { return(m_data); }

		VIRTUAL const Graphics::Color & color(void) const
		    { return(m_color); }

		VIRTUAL const SharedTextureAsset & texture(void) const
		    { return(m_texture); }

		VIRTUAL float rotation(void) const
		    { return(m_rotation); }

		VIRTUAL void scale(float &x, float &y) const
		    { x = m_scale[0]; y = m_scale[1]; }

		VIRTUAL bool serialize(TinyXML::TiXmlElement &node) const;
		VIRTUAL bool deserialize(TinyXML::TiXmlElement &node);
	};
	typedef Core::Shared<MeshBase> SharedMeshBase;
	typedef Core::Weak<MeshBase> WeakMeshBase;

}

MARSHMALLOW_NAMESPACE_END

#endif
