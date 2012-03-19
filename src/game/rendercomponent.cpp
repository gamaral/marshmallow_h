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

MARSHMALLOW_NAMESPACE_USE
using namespace Game;

const Core::Type RenderComponent::sType("Game::RenderComponent");

struct RenderComponent::Private
{
	WeakPositionComponent position;
	Graphics::SharedMesh  mesh;
};

RenderComponent::RenderComponent(const Core::Identifier &i, IEntity &e)
    : ComponentBase(i, e)
    , m_p(new Private)
{
}

RenderComponent::~RenderComponent(void)
{
	delete m_p;
	m_p = 0;
}

Graphics::SharedMesh &
RenderComponent::mesh(void)
{
	return(m_p->mesh);
}

void
RenderComponent::update(float)
{
	if (!m_p->position)
		m_p->position = entity().getComponentType("Game::PositionComponent").
		    staticCast<PositionComponent>();
}

void
RenderComponent::render(void)
{
	if (m_p->position && m_p->mesh)
		Graphics::Painter::Draw(*m_p->mesh, m_p->position->position());
}

bool
RenderComponent::serialize(TinyXML::TiXmlElement &n) const
{
	if (!ComponentBase::serialize(n))
	    return(false);

	TinyXML::TiXmlElement l_mesh("mesh");
	if (m_p->mesh && !m_p->mesh->serialize(l_mesh)) {
		MMWARNING("Render component '" << id().str() << "' serialization failed to serialize mesh!");
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
		MMWARNING("Render component '" << id().str() << "' deserialized without a mesh!");
		return(false);
	}

	const char *l_mesh_type = l_child->Attribute("type");
	Graphics::SharedMesh l_mesh =
	    Game::FactoryBase::Instance()->createMesh(l_mesh_type);
	if (!l_mesh) {
		MMWARNING("Render component '" << id().str() << "' has an unknown mesh type");
		return(false);
	}

	if (!l_mesh->deserialize(*l_child)) {
		MMWARNING("Render component '" << id().str() << "' deserialization of mesh failed");
		return(false);
	}

	m_p->mesh = l_mesh;

	return(true);
}

const Core::Type &
RenderComponent::Type(void)
{
	return(sType);
}

