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

#ifndef GRAPHICS_TRIANGLEMESH_H
#define GRAPHICS_TRIANGLEMESH_H 1

#include "graphics/meshbase.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Graphics
{

	/*! @brief Graphics Triangle Mesh Class */
	class GRAPHICS_EXPORT TriangleMesh : public MeshBase
	{
#define TRIANGLE_VERTEXES 3
		float m_vertex[TRIANGLE_VERTEXES * 2];
		float m_tcoord[TRIANGLE_VERTEXES * 2];

		NO_COPY(TriangleMesh);

	public:

		TriangleMesh(const Math::Vector2 &p1,
		             const Math::Vector2 &p2,
		             const Math::Vector2 &p3);
		TriangleMesh(void);
		virtual ~TriangleMesh(void);

	public: /* virtual */

		VIRTUAL const Core::Type & type(void) const
		    { return(Type); }

		VIRTUAL Math::Vector2 vertex(int index) const;
		VIRTUAL void textureCoord(int index, float &u, float &v) const;
		VIRTUAL int size(void) const
		    { return(TRIANGLE_VERTEXES); }

		VIRTUAL const float * vertexDataArray(void) const
		    { return(m_vertex); }
		VIRTUAL const float * textureCoordArray(void) const
		    { return(m_tcoord); }

		VIRTUAL bool serialize(TinyXML::TiXmlElement &node) const;
		VIRTUAL bool deserialize(TinyXML::TiXmlElement &node);

		VIRTUAL void setVertex(int index, Math::Vector2 &vertex);
		VIRTUAL void setTextureCoord(int index, float u, float v);

	public: /* static */

		static const Core::Type Type;
	};

}

MARSHMALLOW_NAMESPACE_END

#endif
