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

#include "game/textcomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include "math/point2.h"

#include "graphics/factory.h"
#include "graphics/itexturecoordinatedata.h"
#include "graphics/itexturedata.h"
#include "graphics/itileset.h"
#include "graphics/ivertexdata.h"
#include "graphics/painter.h"
#include "graphics/quadmesh.h"

#include "game/ientity.h"
#include "game/positioncomponent.h"


#define MIN_CHAR 33
#define MAX_CHAR 126

MARSHMALLOW_NAMESPACE_USE
using namespace Game;

TextComponent::TextComponent(const Core::Identifier &i, IEntity &e)
    : ComponentBase(i, e)
    , m_font_size(2.f, 4.f)
    , m_tileset()
    , m_tile_offset(0)
    , m_invalidated(false)
{
}

TextComponent::~TextComponent(void)
{
}

void
TextComponent::setText(const std::string &t)
{
	m_text = t;
	m_invalidated = true;
	rebuild();
}

void
TextComponent::setColor(const Graphics::Color &c)
{
	m_color = c;
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
TextComponent::update(float delta)
{
	ComponentBase::update(delta);

	if (!m_position)
	    m_position = entity().getComponentType("Game::PositionComponent").
	        staticCast<PositionComponent>();

	if (m_invalidated)
	    rebuild();
}

void
TextComponent::render(void)
{
	ComponentBase::render();

	if (m_invalidated) return;

	/* if no position component, abort! */
	if (!m_position) {
		MMWARNING1("No position component found!");
		return;
	}

	/* render characters */

	/* TODO: find line-breaks to determine line length for center
	 *       alignment, also add right alignment.
	 */
	char l_char;
	Math::Point2 l_point(m_position->position());
	const size_t l_text_count = m_text.size();
	for (unsigned int i = 0; i < l_text_count; ++i) {
		l_char = m_text[i];

		/* render valid characters */
		if (MIN_CHAR <= l_char && MAX_CHAR >= l_char) {
			Graphics::SharedQuadMesh l_mesh = m_mesh[i].staticCast<Graphics::QuadMesh>();
			l_mesh->setColor(m_color);
			Graphics::Painter::Draw(*l_mesh, l_point);
			l_point[0] += m_font_size.width();
		}

		/* handle line break */
		else if ('\n' == l_char) {
			l_point[0] = m_position->position().x();
			l_point[1] -= m_font_size.height();
		}

		/* skip unknown character */
		else l_point[0] += m_font_size.width();
	}
}

void
TextComponent::rebuild(void)
{
	m_mesh.clear();
	m_mesh.resize(m_text.size());

	if (!m_tileset) {
		MMWARNING1("No tileset assigned.");
		return;
	}

	/* create vertex data */

	/* TODO: this needs to be kept around, only replaced when font size
	 *       changes.
	 */
	Graphics::SharedVertexData l_vdata =
	    Graphics::Factory::CreateVertexData(MARSHMALLOW_QUAD_VERTEXES);
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
	char l_char;
	const size_t l_text_count = m_text.size();
	for (UINT16 i = 0; i < l_text_count; ++i) {
		l_char = m_text[i];
		if (MIN_CHAR <= l_char && MAX_CHAR >= l_char) {
			Graphics::SharedTextureCoordinateData l_tdata =
				m_tileset->getTextureCoordinateData(m_tile_offset +
				    static_cast<unsigned char>(l_char - MIN_CHAR));

			m_mesh[i] = new Graphics::QuadMesh(l_tdata, m_tileset->textureData(), l_vdata);
		}
	}

	m_invalidated = false;
}

const Core::Type &
TextComponent::Type(void)
{
	static const Core::Type s_type("Game::TextComponent");
	return(s_type);
}

