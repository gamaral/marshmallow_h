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

#include "game/textcomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "math/point2.h"

#include "graphics/factory.h"
#include "graphics/itexturecoordinatedata.h"
#include "graphics/itexturedata.h"
#include "graphics/itileset.h"
#include "graphics/ivertexdata.h"
#include "graphics/painter.h"
#include "graphics/quadmesh.h"

#define MIN_CHAR 33
#define MAX_CHAR 126

MARSHMALLOW_NAMESPACE_USE;
using namespace Game;

TextComponent::TextComponent(const Core::Identifier &i, IEntity &e)
    : ComponentBase(i, e)
    , m_font_size(2.f, 4.f)
    , m_tileset()
    , m_tile_offset(0)
{
}

TextComponent::~TextComponent(void)
{
}

bool
TextComponent::serialize(TinyXML::TiXmlElement &n) const
{
	MMUNUSED(n);
	/* TODO: IMPLEMENT */
	return(false);
}

bool
TextComponent::deserialize(TinyXML::TiXmlElement &n)
{
	MMUNUSED(n);
	/* TODO: IMPLEMENT */
	return(false);
}

void
TextComponent::update(TIME)
{
}

void
TextComponent::render(void)
{
	/* create vertex data */

	/* TODO: this needs to be kept around, only replaced when font size
	 *       changes.
	 */
	Graphics::SharedVertexData l_vdata =
	    Graphics::Factory::CreateVertexData(QUAD_VERTEXES);
	{
		float l_hwidth  =
		    static_cast<float>(m_font_size.width())  / 2.f;
		float l_hheight =
		    static_cast<float>(m_font_size.height()) / 2.f;

		l_vdata->set(0, -l_hwidth,  l_hheight);
		l_vdata->set(1, -l_hwidth, -l_hheight);
		l_vdata->set(2,  l_hwidth,  l_hheight);
		l_vdata->set(3,  l_hwidth, -l_hheight);
	}

	/* render characters */

	/* TODO: find line-breaks to determine line length for center
	 *       alignment, also add right alignment.
	 */
	Math::Point2 l_point(0, 0);
	char l_char;
	const int l_text_count = m_text.size();
	for (int i = 0; i < l_text_count; ++i) {
		l_char = m_text[i];
		if (MIN_CHAR <= l_char && MAX_CHAR >= l_char) {
			Graphics::SharedTextureCoordinateData l_tdata =
				m_tileset->getTextureCoordinateData(m_tile_offset + (l_char - MIN_CHAR));

			Graphics::SharedQuadMesh l_mesh =
			    new Graphics::QuadMesh(l_tdata, m_tileset->textureData(), l_vdata);

			Graphics::Painter::Draw(*l_mesh, l_point);
			l_point[0] += m_font_size.width();
		}
		else if ('\n' == l_char) {
			l_point[0] = 0;
			l_point[1] -= m_font_size.height();
		} else {
			l_point[0] += m_font_size.width();
		}
	}
}

const Core::Type &
TextComponent::Type(void)
{
	static const Core::Type s_type("Game::TextComponent");
	return(s_type);
}

