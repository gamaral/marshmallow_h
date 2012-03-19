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

#ifndef MARSHMALLOW_GRAPHICS_MESHBASE_H
#define MARSHMALLOW_GRAPHICS_MESHBASE_H 1

#include <graphics/imesh.h>

#include <core/shared.h>
#include <math/vector2.h>
#include <graphics/color.h>

MARSHMALLOW_NAMESPACE_BEGIN

namespace Graphics
{

	/*! @brief Graphics Mesh Base Class */
	class MARSHMALLOW_GRAPHICS_EXPORT
	MeshBase : public IMesh
	{
		SharedTextureCoordinateData m_tcdata;
		SharedTextureData m_tdata;
		SharedVertexData m_vdata;
		Color m_color;
		float m_scale[2];
		float m_rotation;

		NO_ASSIGN_COPY(MeshBase);
	public:

		MeshBase(SharedTextureCoordinateData tc, SharedTextureData t, SharedVertexData v);
		virtual ~MeshBase(void);

		void setColor(const Graphics::Color &color);

		void setRotation(float angle);

		void setScale(float x, float y)
		    { m_scale[0] = x; m_scale[1] = y; }

		/*! @brief Replace texture coordinate data */
		void setTextureCoordinateData(SharedTextureCoordinateData tc);

		/*! @brief Replace texture data */
		void setTextureData(SharedTextureData td);

		/*! @brief Replace vertex data */
		void setVertexData(Graphics::SharedVertexData vd);

	public: /* operators */

		Math::Vector2 operator[](UINT16 index) const
		    { return(vertex(index)); }

	public: /* virtual */

		virtual void setVertex(UINT16 index, const Math::Vector2 &vertex);
		virtual void setTextureCoordinate(UINT16 index, float u, float v);

		VIRTUAL const Graphics::SharedTextureCoordinateData & textureCoordinateData(void) const
		    { return(m_tcdata); }

		VIRTUAL const Graphics::SharedTextureData & textureData(void) const
		    { return(m_tdata); }

		VIRTUAL const Graphics::SharedVertexData & vertexData(void) const
		    { return(m_vdata); }

		VIRTUAL const Graphics::Color & color(void) const
		    { return(m_color); }

		VIRTUAL float rotation(void) const
		    { return(m_rotation); }

		VIRTUAL void scale(float &x, float &y) const
		    { x = m_scale[0]; y = m_scale[1]; }

		VIRTUAL Math::Vector2 vertex(UINT16 index) const;
		VIRTUAL void textureCoordinate(UINT16 index, float &u, float &v) const;

		VIRTUAL bool serialize(TinyXML::TiXmlElement &node) const;
		VIRTUAL bool deserialize(TinyXML::TiXmlElement &node);
	};
	typedef Core::Shared<MeshBase> SharedMeshBase;
	typedef Core::Weak<MeshBase> WeakMeshBase;

}

MARSHMALLOW_NAMESPACE_END

#endif
