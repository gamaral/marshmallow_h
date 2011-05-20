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

#include "graphics/quadmesh.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "graphics/textureasset.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Graphics;

const Core::Type QuadMesh::Type("Graphics::QuadMesh");

QuadMesh::QuadMesh(const Math::Vector2 &tl,
                   const Math::Vector2 &bl,
                   const Math::Vector2 &br,
                   const Math::Vector2 &tr)
    : MeshBase()
{
	m_vertex[0] = tl.rx(); m_vertex[1] = tl.ry();
	m_vertex[2] = tr.rx(); m_vertex[3] = tr.ry();
	m_vertex[4] = br.rx(); m_vertex[5] = br.ry();
	m_vertex[6] = bl.rx(); m_vertex[7] = bl.ry();

	m_tcoord[0] = m_tcoord[1] = m_tcoord[3] = m_tcoord[6] = 0;
	m_tcoord[2] = m_tcoord[4] = m_tcoord[5] = m_tcoord[7] = 1;
}

QuadMesh::QuadMesh(const Math::Rect2  &r)
    : MeshBase()
{
	const Math::Vector2 &tr = r.topRight();
	m_vertex[0] = tr.rx(); m_vertex[1] = tr.ry();
	const Math::Vector2 &tl = r.topLeft();
	m_vertex[2] = tl.rx(); m_vertex[3] = tl.ry();
	const Math::Vector2 &br = r.bottomRight();
	m_vertex[4] = br.rx(); m_vertex[5] = br.ry();
	const Math::Vector2 &bl = r.bottomLeft();
	m_vertex[6] = bl.rx(); m_vertex[7] = bl.ry();

	m_tcoord[1] = m_tcoord[2] = m_tcoord[3] = m_tcoord[6] = 0;
	m_tcoord[0] = m_tcoord[4] = m_tcoord[5] = m_tcoord[7] = 1;
}

QuadMesh::QuadMesh(void)
    : MeshBase()
{
	memset(m_vertex, 0, QUAD_VERTEXES * 2);

	m_tcoord[1] = m_tcoord[2] = m_tcoord[3] = m_tcoord[6] = 0;
	m_tcoord[0] = m_tcoord[4] = m_tcoord[5] = m_tcoord[7] = 1;
}

QuadMesh::~QuadMesh(void)
{
}

Math::Vector2
QuadMesh::vertex(int index) const
{
	const int l_offset = (index % QUAD_VERTEXES) * 2;
	return(Math::Vector2(m_vertex[l_offset], m_vertex[l_offset + 1]));
}

void
QuadMesh::textureCoord(int i, float &u, float &v) const
{
	const int l_offset = (i % QUAD_VERTEXES) * 2;
	u = m_tcoord[l_offset];
	v = m_tcoord[l_offset + 1];
}

bool
QuadMesh::serialize(TinyXML::TiXmlElement &n) const
{
	if (!MeshBase::serialize(n))
		return(false);

	for (int i = 0; i < QUAD_VERTEXES; ++i) {
		const int l_offset = i * 2;
		TinyXML::TiXmlElement l_vector("vector");
		l_vector.SetDoubleAttribute("x", m_vertex[l_offset]);
		l_vector.SetDoubleAttribute("y", m_vertex[l_offset + 1]);
		n.InsertEndChild(l_vector);
	}

	for (int i = 0; i < QUAD_VERTEXES; ++i) {
		const int l_offset = i * 2;
		TinyXML::TiXmlElement l_vector("tcoord");
		l_vector.SetDoubleAttribute("u", m_tcoord[l_offset]);
		l_vector.SetDoubleAttribute("v", m_tcoord[l_offset + 1]);
		n.InsertEndChild(l_vector);
	}

	return(true);
}

bool
QuadMesh::deserialize(TinyXML::TiXmlElement &n)
{
	if (!MeshBase::deserialize(n))
		return(false);

	int l_i;
	TinyXML::TiXmlElement *l_child;
	for (l_i = 0, l_child = n.FirstChildElement("vector");
	     l_i < QUAD_VERTEXES && l_child;
	     ++l_i, l_child = l_child->NextSiblingElement("vector")) {
		const int l_offset = l_i * 2;
		l_child->QueryFloatAttribute("x", &m_vertex[l_offset]);
		l_child->QueryFloatAttribute("y", &m_vertex[l_offset + 1]);
	}

	for (l_i = 0, l_child = n.FirstChildElement("tcoord");
	     l_i < QUAD_VERTEXES && l_child;
	     ++l_i, l_child = l_child->NextSiblingElement("tcoord")) {
		const int l_offset = l_i * 2;
		l_child->QueryFloatAttribute("u", &m_tcoord[l_offset]);
		l_child->QueryFloatAttribute("v", &m_tcoord[l_offset + 1]);
	}

	return(true);
}

void
QuadMesh::setVertex(int i, Math::Vector2 &v)
{
	const int l_offset = (i % QUAD_VERTEXES) * 2;
	v.rx() = m_vertex[l_offset];
	v.ry() = m_vertex[l_offset + 1];
}

void
QuadMesh::setTextureCoord(int i, float u, float v)
{
	const int l_offset = (i % QUAD_VERTEXES) * 2;
	u = m_tcoord[l_offset];
	v = m_tcoord[l_offset + 1];
}

