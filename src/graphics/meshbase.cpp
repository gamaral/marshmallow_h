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

#include "graphics/textureasset.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Graphics;

MeshBase::MeshBase(void)
    : m_color(),
      m_texture(),
      m_rotation(0)
{
	m_scale[0] = m_scale[1] = 1.f;
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
	if (m_texture) {
		TinyXML::TiXmlElement l_texture("texture");
		l_texture.SetAttribute("file", m_texture->filename().c_str());
		n.InsertEndChild(l_texture);
	}

	return(true);
}

bool
MeshBase::deserialize(TinyXML::TiXmlElement &n)
{
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

	return(true);
}

