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

#include <core/fd.h>

#include <math/vector2.h>

#include <graphics/color.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */

	/*! @brief Graphics Mesh Base Class */
	class MARSHMALLOW_GRAPHICS_EXPORT
	MeshBase : public IMesh
	{
		struct Private;
		Private *m_p;

		NO_ASSIGN_COPY(MeshBase);
	public:

		MeshBase(SharedTextureCoordinateData tc, SharedTextureData t, SharedVertexData v);
		virtual ~MeshBase(void);

		void setColor(const Graphics::Color &color);

		void setRotation(float angle);

		void setScale(float x, float y);

		/*! @brief Replace texture coordinate data */
		void setTextureCoordinateData(SharedTextureCoordinateData tc);

		/*! @brief Replace texture data */
		void setTextureData(SharedTextureData td);

		/*! @brief Replace vertex data */
		void setVertexData(Graphics::SharedVertexData vd);

	public: /* operators */

		Math::Vector2 operator[](uint16_t index) const
		    { return(vertex(index)); }

	public: /* virtual */

		virtual void setVertex(uint16_t index, const Math::Vector2 &vertex);
		virtual void setTextureCoordinate(uint16_t index, float u, float v);

		VIRTUAL const Graphics::SharedTextureCoordinateData & textureCoordinateData(void) const;
		VIRTUAL const Graphics::SharedTextureData & textureData(void) const;
		VIRTUAL const Graphics::SharedVertexData & vertexData(void) const;

		VIRTUAL const Graphics::Color & color(void) const;
		VIRTUAL float rotation(void) const;
		VIRTUAL void scale(float &x, float &y) const;

		VIRTUAL Math::Vector2 vertex(uint16_t index) const;
		VIRTUAL void textureCoordinate(uint16_t index, float &u, float &v) const;

		VIRTUAL bool serialize(XMLElement &node) const;
		VIRTUAL bool deserialize(XMLElement &node);
	};
	typedef Core::Shared<MeshBase> SharedMeshBase;
	typedef Core::Weak<MeshBase> WeakMeshBase;

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
