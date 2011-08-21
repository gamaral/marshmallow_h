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

#include "graphics/meshbase.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"
#include "core/type.h"
#include "graphics/itexturecoordinatedata.h"
#include "graphics/itexturedata.h"
#include "graphics/ivertexdata.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Graphics;

MeshBase::MeshBase(SharedTextureCoordinateData tc, SharedTextureData t, SharedVertexData v)
    : m_tcdata(tc),
      m_tdata(t),
      m_vdata(v),
      m_color(),
      m_rotation(0)
{
	m_scale[0] = m_scale[1] = 1.f;
}

MeshBase::~MeshBase(void)
{
}

void
MeshBase::setColor(const Graphics::Color &c)
{
	m_color = c;
}

void
MeshBase::setRotation(float a)
{
	m_rotation = a;
}

Math::Vector2
MeshBase::vertex(int i) const
{
	Math::Vector2 l_vector;
	if (!m_vdata->get(i, l_vector.rx(), l_vector.ry()))
		MMWARNING("Failed to retrieve values for vertex %d", i);
	return(l_vector);
}

void
MeshBase::setVertex(int i, const Math::Vector2 &v)
{
	if (!m_vdata->set(i, v.rx(), v.ry()))
		MMWARNING("Failed to assign values (%f, %f) to vertex %d",
		    v.rx(), v.ry(), i);
}

void
MeshBase::textureCoordinate(int i, float &u, float &v) const
{
	if (!m_tcdata->get(i, u, v))
		MMWARNING("Failed to retrieve values for vertex %d", i);
}

void
MeshBase::setTextureCoordinate(int i, float u, float v)
{
	if (!m_tcdata->set(i, u, v))
		MMWARNING("Failed to assign values (%f, %f) to texture coordinate %d",
		    u, v, i);
}

bool
MeshBase::serialize(TinyXML::TiXmlElement &n) const
{
	n.SetAttribute("type", type().str().c_str());
	n.SetDoubleAttribute("rotation", m_rotation);

	/* color */
	TinyXML::TiXmlElement l_color("color");
	l_color.SetDoubleAttribute("r", m_color[0]);
	l_color.SetDoubleAttribute("g", m_color[1]);
	l_color.SetDoubleAttribute("b", m_color[2]);
	l_color.SetDoubleAttribute("a", m_color[3]);
	n.InsertEndChild(l_color);

	/* texture */
	if (m_tdata->isLoaded()) {
		TinyXML::TiXmlElement l_texture("texture");
		l_texture.SetAttribute("id", m_tdata->id().str().c_str());
		n.InsertEndChild(l_texture);
	}

	/* texture coordinates */
	for (int i = 0; i < m_tcdata->count(); ++i) {
		float l_u, l_v;
		if (m_tcdata->get(i, l_u, l_v)) {
			TinyXML::TiXmlElement l_vector("tcoord");
			l_vector.SetDoubleAttribute("u", l_u);
			l_vector.SetDoubleAttribute("v", l_v);
			n.InsertEndChild(l_vector);
		} else MMWARNING("Failed to serialize text coord %d", i);
	}

	/* vertexes */
	for (int i = 0; i < m_vdata->count(); ++i) {
		float l_x, l_y;
		if (m_vdata->get(i, l_x, l_y)) {
			TinyXML::TiXmlElement l_vector("vector");
			l_vector.SetDoubleAttribute("x", l_x);
			l_vector.SetDoubleAttribute("y", l_y);
			n.InsertEndChild(l_vector);
		} else MMWARNING("Failed to serialize vertex %d", i);
	}

	return(true);
}

bool
MeshBase::deserialize(TinyXML::TiXmlElement &n)
{
	TinyXML::TiXmlElement *l_child;
	int l_i;

	n.QueryFloatAttribute("rotation", &m_rotation);

	/* color */
	l_child = n.FirstChildElement("color");
	if (l_child) {
		l_child->QueryFloatAttribute("r", &m_color[0]);
		l_child->QueryFloatAttribute("g", &m_color[1]);
		l_child->QueryFloatAttribute("b", &m_color[2]);
		l_child->QueryFloatAttribute("a", &m_color[3]);
	}

	/* texture */
	l_child = n.FirstChildElement("texture");
	if (l_child) {
		const char *l_file = l_child->Attribute("id");
		if (l_file) m_tdata->load(l_file);
	}

	/* texture coordinates */
	for (l_child = n.FirstChildElement("tcoord"), l_i = 0;
	     l_child;
	     l_child = l_child->NextSiblingElement("tcoord")) {
		float l_u, l_v;
		l_child->QueryFloatAttribute("u", &l_u);
		l_child->QueryFloatAttribute("v", &l_v);
		if (!m_tcdata->set(l_i++, l_u, l_v)) {
			MMWARNING1("Failed to assign texture coordinate data.");
			break;
		}
	}

	/* vertexes */
	for (l_child = n.FirstChildElement("vector"), l_i = 0;
	     l_child;
	     l_child = l_child->NextSiblingElement("vector")) {
		float l_x, l_y;
		l_child->QueryFloatAttribute("x", &l_x);
		l_child->QueryFloatAttribute("y", &l_y);
		if (!m_vdata->set(l_i++, l_x, l_y)) {
			MMWARNING1("Failed to assign vertex data.");
			break;
		}
	}

	return(true);
}

