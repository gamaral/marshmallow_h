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
#include "graphics/imeshdata.h"
#include "graphics/textureasset.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Graphics;

MeshBase::MeshBase(Graphics::SharedMeshData d)
    : m_data(d),
      m_color(),
      m_texture(),
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
MeshBase::setTexture(Graphics::SharedTextureAsset t)
{
	m_texture = t;
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
	if (!m_data->vertex(i, l_vector.rx(), l_vector.ry()))
		WARNING("Failed to retreave values for vertex %d", i);
	return(l_vector);
}

void
MeshBase::setVertex(int i, const Math::Vector2 &v)
{
	if (!m_data->setVertex(i, v.rx(), v.ry()))
		WARNING("Failed to assign values (%f, %f) to vertex %d",
		    v.rx(), v.ry(), i);
}

void
MeshBase::textureCoord(int i, float &u, float &v) const
{
	if (!m_data->textureCoord(i, u, v))
		WARNING("Failed to retreave values for vertex %d", i);
}

void
MeshBase::setTextureCoord(int i, float u, float v)
{
	if (!m_data->setTextureCoord(i, u, v))
		WARNING("Failed to assign values (%f, %f) to texture coord %d",
		    u, v, i);
}

bool
MeshBase::serialize(TinyXML::TiXmlElement &n) const
{
	const int l_size = size();

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
	if (m_texture) {
		TinyXML::TiXmlElement l_texture("texture");
		l_texture.SetAttribute("file", m_texture->filename().c_str());
		n.InsertEndChild(l_texture);
	}

	/* vertexes */
	for (int i = 0; i < l_size; ++i) {
		float l_x, l_y;
		if (m_data->vertex(i, l_x, l_y)) {
			TinyXML::TiXmlElement l_vector("vector");
			l_vector.SetDoubleAttribute("x", l_x);
			l_vector.SetDoubleAttribute("y", l_y);
			n.InsertEndChild(l_vector);
		} else WARNING("Failed to serialize vertex %d", i);
	}

	/* texture coordinates */
	for (int i = 0; i < l_size; ++i) {
		float l_u, l_v;
		if (m_data->vertex(i, l_u, l_v)) {
			TinyXML::TiXmlElement l_vector("tcoord");
			l_vector.SetDoubleAttribute("u", l_u);
			l_vector.SetDoubleAttribute("v", l_v);
			n.InsertEndChild(l_vector);
		} else WARNING("Failed to serialize text coord %d", i);
	}
	return(true);
}

bool
MeshBase::deserialize(TinyXML::TiXmlElement &n)
{
	const int l_size = size();


	TinyXML::TiXmlElement *l_child;

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
		const char *l_file = l_child->Attribute("file");
		TextureAsset *l_texture = new TextureAsset;
		l_texture->load(l_file);
		setTexture(l_texture);
	}

	/* vertexes */
	int l_i;
	for (l_child = n.FirstChildElement("vector"), l_i = 0;
	     l_child;
	     l_child = l_child->NextSiblingElement("vector")) {
		float l_x, l_y;
		l_child->QueryFloatAttribute("x", &l_x);
		l_child->QueryFloatAttribute("y", &l_y);
		if (!m_data->setVertex(l_i++, l_x, l_y)) {
			WARNING1("Failed to assign vertex to mesh data.");
			break;
		}
	}

	/* texture coordinates */
	for (l_child = n.FirstChildElement("tcoord");
	     l_child;
	     l_child = l_child->NextSiblingElement("tcoord")) {
		float l_u, l_v;
		l_child->QueryFloatAttribute("u", &l_u);
		l_child->QueryFloatAttribute("v", &l_v);
		if (!m_data->setTextureCoord(l_i++, l_u, l_v)) {
			WARNING1("Failed to assign texture coord to mesh data.");
			break;
		}
	}

	return(true);
}

