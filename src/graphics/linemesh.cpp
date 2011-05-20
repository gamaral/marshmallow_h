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

#include "graphics/linemesh.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

MARSHMALLOW_NAMESPACE_USE;
using namespace Graphics;

const Core::Type LineMesh::Type("Graphics::LineMesh");

LineMesh::LineMesh(const Math::Vector2 &p1,
                         const Math::Vector2 &p2)
    : MeshBase()
{
	m_vertex[0] = p1.rx(); m_vertex[1] = p1.ry();
	m_vertex[2] = p2.rx(); m_vertex[3] = p2.ry();
}

LineMesh::LineMesh(void)
    : MeshBase()
{
	memset(m_vertex, 0, LINE_VERTEXES * 2);
}

LineMesh::~LineMesh(void)
{
}

Math::Vector2
LineMesh::vertex(int i) const
{
	const int l_offset = (i % LINE_VERTEXES) * 2;
	return(Math::Vector2(m_vertex[l_offset], m_vertex[l_offset + 1]));
}

bool
LineMesh::serialize(TinyXML::TiXmlElement &n) const
{
	/* TODO: FILL IN */
	UNUSED(n);
	return(false);
}

bool
LineMesh::deserialize(TinyXML::TiXmlElement &n)
{
	/* TODO: FILL IN */
	UNUSED(n);
	return(false);
}

void
LineMesh::setVertex(int i, Math::Vector2 &v)
{
	const int l_offset = (i % LINE_VERTEXES) * 2;
	m_vertex[l_offset] = v.rx();
	m_vertex[l_offset + 1] = v.ry();
}

