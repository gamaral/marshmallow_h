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

#include "graphics/graphicbase.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <tinyxml.h>

#include "graphics/textureasset.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Graphics;

GraphicBase::GraphicBase(void)
    : m_texture(),
      m_rotation(0)
{
}

void
GraphicBase::setTexture(Graphics::SharedTextureAsset t)
{
	m_texture = t;
}

void
GraphicBase::setRotation(float a)
{
	m_rotation = a;
}

bool
GraphicBase::serialize(TinyXML::TiXmlElement &n) const
{
	n.SetAttribute("type", type().str());
	n.SetDoubleAttribute("rotation", m_rotation);

	/* TODO: Save Texture */

	return(true);
}

bool
GraphicBase::deserialize(TinyXML::TiXmlElement &n)
{
	TinyXML::TiXmlElement *l_child;

	n.QueryFloatAttribute("rotation", &m_rotation);

	l_child = n.FirstChildElement("texture");
	if (l_child) {
		const char *l_file = l_child->Attribute("file");
		TextureAsset *l_texture = new TextureAsset;
		l_texture->load(l_file);
		setTexture(l_texture);
	}

	return(true);
}

