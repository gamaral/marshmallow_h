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

#include "game/rendercomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <tinyxml.h>

#include "core/logger.h"
#include "graphics/igraphic.h"
#include "graphics/painter.h"
#include "game/graphicfactorybase.h"
#include "game/ientity.h"
#include "game/positioncomponent.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Game;

const Core::Type RenderComponent::Type("Game::RenderComponent");

RenderComponent::RenderComponent(const Core::Identifier &i, IEntity &e)
    : ComponentBase(i, e),
      m_position(),
      m_graphic()
{
}

RenderComponent::~RenderComponent(void)
{
}

void
RenderComponent::update(TIME)
{
	if (!m_position)
		m_position = entity().componentType("Game::PositionComponent").
		    staticCast<PositionComponent>();
}

void
RenderComponent::render(void)
{
	if (m_position && m_graphic)
		Graphics::Painter::Draw(*m_graphic, m_position->position());
}

bool
RenderComponent::serialize(TinyXML::TiXmlElement &n) const
{
	n.SetAttribute("id", id().str());
	n.SetAttribute("type", type().str());

	TinyXML::TiXmlElement l_graphic("graphic");
	if (m_graphic && !m_graphic->serialize(l_graphic)) {
		WARNING("Render component '%s' serialization failed to serialize graphic!",
		    id().str());
		return(false);
	}
	n.InsertEndChild(l_graphic);

	return(true);
}

bool
RenderComponent::deserialize(TinyXML::TiXmlElement &n)
{
	TinyXML::TiXmlElement *l_child = n.FirstChildElement("graphic");
	if (!l_child) {
		WARNING("Render component '%s' deserialized without a graphic!",
		    id().str());
		return(false);
	}

	const char *l_graphic_type = l_child->Attribute("type");
	Graphics::SharedGraphic l_graphic =
	    Game::GraphicFactoryBase::Instance()->createGraphic(l_graphic_type);
	if (!l_graphic) {
		WARNING("Render component '%s' has an unknown graphic type",
		    id().str());
		return(false);
	}

	if (!l_graphic->deserialize(*l_child)) {
		WARNING("Render component '%s' deserialization of graphic failed",
		    id().str());
		return(false);
	}

	m_graphic = l_graphic;

	return(true);
}

