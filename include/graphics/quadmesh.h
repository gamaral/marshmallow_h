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

#ifndef MARSHMALLOW_GRAPHICS_QUADMESH_H
#define MARSHMALLOW_GRAPHICS_QUADMESH_H 1

#include <graphics/meshbase.h>

#include <math/rect2.h>

MARSHMALLOW_NAMESPACE_BEGIN

namespace Graphics
{
	/*! @brief Graphics Quad Mesh Class */
	class MARSHMALLOW_GRAPHICS_EXPORT
	QuadMesh : public MeshBase
#define MARSHMALLOW_QUAD_VERTEXES 4
	{
		NO_ASSIGN_COPY(QuadMesh);
	public:

		QuadMesh(const Math::Vector2 &tl,
		         const Math::Vector2 &bl,
		         const Math::Vector2 &br,
		         const Math::Vector2 &tr);
		QuadMesh(const Math::Rect2 &rect);
		QuadMesh(float width, float height);
		QuadMesh(void);
		QuadMesh(SharedTextureCoordinateData tc, SharedTextureData t, SharedVertexData v);
		virtual ~QuadMesh(void);

	public: /* virtual */

		VIRTUAL const Core::Type & type(void) const
		    { return(Type()); }

		VIRTUAL int count(void) const
		    { return(MARSHMALLOW_QUAD_VERTEXES); }

	public: /* static */

		static const Core::Type & Type(void);

	private: /* static */

		static const Core::Type sType;
	};
	typedef Core::Shared<QuadMesh> SharedQuadMesh;
	typedef Core::Weak<QuadMesh> WeakQuadMesh;
}

MARSHMALLOW_NAMESPACE_END

#endif
