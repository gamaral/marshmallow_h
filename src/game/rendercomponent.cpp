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

#include "core/logger.h"
#include "graphics/imesh.h"
#include "graphics/painter.h"
#include "game/factorybase.h"
#include "game/ientity.h"
#include "game/positioncomponent.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Game;

const Core::Type RenderComponent::sType("Game::RenderComponent");

RenderComponent::RenderComponent(const Core::Identifier &i, IEntity &e)
    : ComponentBase(i, e),
      m_position(),
      m_mesh()
{
}

RenderComponent::~RenderComponent(void)
{
}

void
RenderComponent::update(TIME)
{
	if (!m_position)
		m_position = entity().getComponentType("Game::PositionComponent").
		    staticCast<PositionComponent>();
}

void
RenderComponent::render(void)
{
	if (m_position && m_mesh)
		Graphics::Painter::Draw(*m_mesh, m_position->position());
}

bool
RenderComponent::serialize(TinyXML::TiXmlElement &n) const
{
	if (!ComponentBase::serialize(n))
	    return(false);

	TinyXML::TiXmlElement l_mesh("mesh");
	if (m_mesh && !m_mesh->serialize(l_mesh)) {
		WARNING("Render component '%s' serialization failed to serialize mesh!",
		    id().str().c_str());
		return(false);
	}
	n.InsertEndChild(l_mesh);

	return(true);
}

bool
RenderComponent::deserialize(TinyXML::TiXmlElement &n)
{
	if (!ComponentBase::deserialize(n))
	    return(false);

	TinyXML::TiXmlElement *l_child = n.FirstChildElement("mesh");
	if (!l_child) {
		WARNING("Render component '%s' deserialized without a mesh!",
		    id().str().c_str());
		return(false);
	}

	const char *l_mesh_type = l_child->Attribute("type");
	Graphics::SharedMesh l_mesh =
	    Game::FactoryBase::Instance()->createMesh(l_mesh_type);
	if (!l_mesh) {
		WARNING("Render component '%s' has an unknown mesh type",
		    id().str().c_str());
		return(false);
	}

	if (!l_mesh->deserialize(*l_child)) {
		WARNING("Render component '%s' deserialization of mesh failed",
		    id().str().c_str());
		return(false);
	}

	m_mesh = l_mesh;

	return(true);
}

const Core::Type &
RenderComponent::Type(void)
{
	return(sType);
}

