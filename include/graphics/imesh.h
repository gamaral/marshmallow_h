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

#ifndef MARSHMALLOW_GRAPHICS_IMESH_H
#define MARSHMALLOW_GRAPHICS_IMESH_H 1

#include <core/iserializable.h>

#include <core/fd.h>

#include <graphics/config.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Math { class Vector2; }

namespace Graphics { /************************************ Graphics Namespace */

	struct ITextureCoordinateData;
	typedef Core::Shared<ITextureCoordinateData> SharedTextureCoordinateData;

	struct ITextureData;
	typedef Core::Shared<ITextureData> SharedTextureData;

	struct IVertexData;
	typedef Core::Shared<IVertexData> SharedVertexData;

	class Color;

	/*! @brief Graphics Mesh Interface */
	struct MARSHMALLOW_GRAPHICS_EXPORT
	IMesh : public Core::ISerializable
	{
		virtual ~IMesh(void);

		virtual const SharedTextureCoordinateData & textureCoordinateData(void) const = 0;
		virtual const SharedTextureData & textureData(void) const = 0;
		virtual const SharedVertexData & vertexData(void) const = 0;

		virtual const Graphics::Color & color(void) const = 0;
		virtual float rotation(void) const = 0;
		virtual void scale(float &x, float &y) const = 0;

		virtual Math::Vector2 vertex(uint16_t index) const = 0;
		virtual void textureCoordinate(uint16_t index, float &u, float &v) const = 0;
		virtual int count(void) const = 0;

		virtual const Core::Type & type(void) const = 0;
	};
	typedef Core::Shared<IMesh> SharedMesh;
	typedef Core::Weak<IMesh> WeakMesh;

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
