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

#include "graphics/quadmesh.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/type.h"

#include "graphics/factory.h"
#include "graphics/itexturecoordinatedata.h"
#include "graphics/itexturedata.h"
#include "graphics/ivertexdata.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */

QuadMesh::QuadMesh(const Math::Vector2 &tl,
                   const Math::Vector2 &bl,
                   const Math::Vector2 &br,
                   const Math::Vector2 &tr)
    : Mesh(Backend::Factory::CreateTextureCoordinateData(MARSHMALLOW_QUAD_VERTEXES),
           Backend::Factory::CreateTextureData(),
           Backend::Factory::CreateVertexData(MARSHMALLOW_QUAD_VERTEXES))
{
	setVertex(0, tl);
	setTextureCoordinate(0, 0, 0);
	setVertex(1, bl);
	setTextureCoordinate(1, 0, 1);
	setVertex(2, tr);
	setTextureCoordinate(2, 1, 0);
	setVertex(3, br);
	setTextureCoordinate(3, 1, 1);
}

QuadMesh::QuadMesh(const Math::Rect2  &r)
    : Mesh(Backend::Factory::CreateTextureCoordinateData(MARSHMALLOW_QUAD_VERTEXES),
           Backend::Factory::CreateTextureData(),
           Backend::Factory::CreateVertexData(MARSHMALLOW_QUAD_VERTEXES),
           TextureCoordinateFree|TextureDataFree|VertexDataFree)
{
	setVertex(0, r.topLeft());
	setTextureCoordinate(0, 0, 0);
	setVertex(1, r.bottomLeft());
	setTextureCoordinate(1, 0, 1);
	setVertex(2, r.topRight());
	setTextureCoordinate(2, 1, 0);
	setVertex(3, r.bottomRight());
	setTextureCoordinate(3, 1, 1);
}

QuadMesh::QuadMesh(const Math::Size2f &s)
    : Mesh(Backend::Factory::CreateTextureCoordinateData(MARSHMALLOW_QUAD_VERTEXES),
           Backend::Factory::CreateTextureData(),
           Backend::Factory::CreateVertexData(MARSHMALLOW_QUAD_VERTEXES),
           TextureCoordinateFree|TextureDataFree|VertexDataFree)
{
	/* half size */
	const float l_hwidth  = s.width  / 2.f;
	const float l_hheight = s.height / 2.f;

	setVertex(0, Math::Vector2(-l_hwidth,  l_hheight));
	setTextureCoordinate(0, 0, 0);
	setVertex(1, Math::Vector2(-l_hwidth, -l_hheight));
	setTextureCoordinate(1, 0, 1);
	setVertex(2, Math::Vector2( l_hwidth,  l_hheight));
	setTextureCoordinate(2, 1, 0);
	setVertex(3, Math::Vector2( l_hwidth, -l_hheight));
	setTextureCoordinate(3, 1, 1);
}

QuadMesh::QuadMesh(float width, float height)
    : Mesh(Backend::Factory::CreateTextureCoordinateData(MARSHMALLOW_QUAD_VERTEXES),
           Backend::Factory::CreateTextureData(),
           Backend::Factory::CreateVertexData(MARSHMALLOW_QUAD_VERTEXES),
           TextureCoordinateFree|TextureDataFree|VertexDataFree)
{
	/* half size */
	const float l_hwidth  = width  / 2.f;
	const float l_hheight = height / 2.f;

	setVertex(0, Math::Vector2(-l_hwidth,  l_hheight));
	setTextureCoordinate(0, 0, 0);
	setVertex(1, Math::Vector2(-l_hwidth, -l_hheight));
	setTextureCoordinate(1, 0, 1);
	setVertex(2, Math::Vector2( l_hwidth,  l_hheight));
	setTextureCoordinate(2, 1, 0);
	setVertex(3, Math::Vector2( l_hwidth, -l_hheight));
	setTextureCoordinate(3, 1, 1);
}

QuadMesh::QuadMesh(void)
    : Mesh(Backend::Factory::CreateTextureCoordinateData(MARSHMALLOW_QUAD_VERTEXES),
           Backend::Factory::CreateTextureData(),
           Backend::Factory::CreateVertexData(MARSHMALLOW_QUAD_VERTEXES),
           TextureCoordinateFree|TextureDataFree|VertexDataFree)
{
	setTextureCoordinate(0, 0, 0);
	setTextureCoordinate(1, 0, 1);
	setTextureCoordinate(2, 1, 0);
	setTextureCoordinate(3, 1, 1);
}

QuadMesh::QuadMesh(ITextureCoordinateData *tc, ITextureData *t, IVertexData *v, int f)
    : Mesh(tc, t, v, f)
{
}

QuadMesh::~QuadMesh(void)
{
}

const Core::Type &
QuadMesh::Type(void)
{
	static const Core::Type s_type("Graphics::QuadMesh");
	return(s_type);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

