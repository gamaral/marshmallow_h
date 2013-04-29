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

#ifndef MARSHMALLOW_GRAPHICS_MESHBASE_H
#define MARSHMALLOW_GRAPHICS_MESHBASE_H 1

#include <math/vector2.h>

#include <graphics/color.h>
#include <graphics/imesh.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */

	/*! @brief Graphics Mesh Base Class */
	class MARSHMALLOW_GRAPHICS_EXPORT
	MeshBase : public IMesh
	{
		PRIVATE_IMPLEMENTATION
		NO_ASSIGN_COPY(MeshBase);
	public:

		MeshBase(Graphics::ITextureCoordinateData *tc,
		         Graphics::ITextureData *t,
		         Graphics::IVertexData *v);
		virtual ~MeshBase(void);

		void setColor(const Graphics::Color &color);

		void setRotation(float angle);

		void setScale(float x, float y);

		/*! @brief Replace texture coordinate data */
		void setTextureCoordinateData(Graphics::ITextureCoordinateData *tc);

		/*! @brief Replace texture data */
		void setTextureData(Graphics::ITextureData *td);

		/*! @brief Replace vertex data */
		void setVertexData(Graphics::IVertexData *vd);

	public: /* operators */

		Math::Vector2 operator[](uint16_t index) const
		    { return(vertex(index)); }

	public: /* virtual */

		virtual void setVertex(uint16_t index, const Math::Vector2 &vertex);
		virtual void setTextureCoordinate(uint16_t index, float u, float v);

		VIRTUAL Graphics::ITextureCoordinateData * textureCoordinateData(void) const;
		VIRTUAL Graphics::ITextureData * textureData(void) const;
		VIRTUAL Graphics::IVertexData * vertexData(void) const;

		VIRTUAL const Graphics::Color & color(void) const;
		VIRTUAL float rotation(void) const;
		VIRTUAL void scale(float &x, float &y) const;

		VIRTUAL Math::Vector2 vertex(uint16_t index) const;
		VIRTUAL void textureCoordinate(uint16_t index, float &u, float &v) const;
	};

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
