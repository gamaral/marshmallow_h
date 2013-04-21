/*
 * Copyright (c) 2011-2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
 * All rights reserved.
 *
 * This file is part of Marshmallow Game Engine.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the project as a whole.
 */

#include "game/scenebase.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/logger.h"
#include "core/shared.h"

#include "graphics/color.h"
#include "graphics/painter.h"

#include "game/factorybase.h"
#include "game/iscenelayer.h"

#include <tinyxml2.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

struct SceneBase::Private
{
	SceneLayerList layers;
	Core::Identifier id;
	Graphics::Color bgcolor;
	bool active;
};

SceneBase::SceneBase(const Core::Identifier &i)
    : m_p(new Private)
{
	m_p->id = i;
	m_p->bgcolor = Graphics::Color::Black();
	m_p->active = false;
}

SceneBase::~SceneBase(void)
{
	m_p->layers.clear();

	delete m_p, m_p = 0;
}

const Core::Identifier &
SceneBase::id(void) const
{
	return(m_p->id);
}

bool
SceneBase::isActive(void) const
{
	return(m_p->active);
}

void
SceneBase::pushLayer(SharedSceneLayer l)
{
	m_p->layers.push_front(l);
}

void
SceneBase::popLayer(void)
{
	m_p->layers.pop_front();
}

void
SceneBase::removeLayer(const Core::Identifier &i)
{
	SceneLayerList::const_iterator l_i;
	SceneLayerList::const_iterator l_c = m_p->layers.end();

	/* maybe replace later with a map if required */
	for (l_i = m_p->layers.begin(); l_i != l_c; ++l_i)
		if ((*l_i)->id() == i) {
			SharedSceneLayer l_slayer = *l_i;
			m_p->layers.remove(l_slayer);
			return;
		}
}

SharedSceneLayer
SceneBase::getLayer(const Core::Identifier &i) const
{
	SceneLayerList::const_iterator l_i;
	SceneLayerList::const_iterator l_c = m_p->layers.end();

	/* maybe replace later with a map if required */
	for (l_i = m_p->layers.begin(); l_i != l_c; ++l_i) {
		if ((*l_i)->id() == i)
			return(*l_i);
	}

	return(SharedSceneLayer());
}

SharedSceneLayer
SceneBase::getLayerType(const Core::Type &t) const
{
	SceneLayerList::const_iterator l_i;
	SceneLayerList::const_iterator l_c = m_p->layers.end();

	/* maybe replace later with a map if required */
	for (l_i = m_p->layers.begin(); l_i != l_c; ++l_i) {
		if ((*l_i)->type() == t)
			return(*l_i);
	}

	return(SharedSceneLayer());
}

const SceneLayerList &
SceneBase::getLayers(void) const
{
	return(m_p->layers);
}

const Graphics::Color &
SceneBase::background(void) const
{
	return(m_p->bgcolor);
}

void
SceneBase::setBackground(const Graphics::Color &color)
{
	using namespace Graphics;

	m_p->bgcolor = color;

	if (isActive())
		Painter::SetBackgroundColor(m_p->bgcolor);
}

void
SceneBase::activate(void)
{
	using namespace Graphics;

	Painter::SetBackgroundColor(m_p->bgcolor);

	m_p->active = true;
}

void
SceneBase::deactivate(void)
{
	m_p->active = false;
}

void
SceneBase::render(void)
{
	if (m_p->layers.empty()) return;

	SceneLayerList::const_iterator l_i;
	SceneLayerList::const_iterator l_b = m_p->layers.begin();
	SceneLayerList::const_iterator l_c = --m_p->layers.end();

	for (l_i = l_b; l_i != l_c; ++l_i)
		if ((*l_i)->flags() & slfRenderBlock)
			break;

	bool l_finished = false;
	do {
		(*l_i)->render();

		if (l_i == l_b) l_finished = true;
		else l_i--;
	} while(!l_finished);
}

void
SceneBase::update(float d)
{
	if (m_p->layers.empty()) return;

	SceneLayerList::const_iterator l_i;
	SceneLayerList::const_iterator l_b = m_p->layers.begin();
	SceneLayerList::const_iterator l_c = --m_p->layers.end();

	for (l_i = l_b; l_i != l_c; ++l_i)
		if ((*l_i)->flags() & slfUpdateBlock)
			break;

	/* TODO: polish later if possible */
	bool l_finished = false;
	do {
		SharedSceneLayer l_slayer = (*l_i);

		if (l_i == l_b) l_finished = true;
		else l_i--;

		if (l_slayer->isZombie())
			m_p->layers.remove(l_slayer);
		else
			l_slayer->update(d);
	} while(!l_finished);
}

bool
SceneBase::serialize(XMLElement &n) const
{
	n.SetAttribute("id", id().str().c_str());
	n.SetAttribute("type", type().str().c_str());

	SceneLayerList::const_reverse_iterator l_i;
	SceneLayerList::const_reverse_iterator l_c = m_p->layers.rend();
	for (l_i = m_p->layers.rbegin(); l_i != l_c; ++l_i) {
		XMLElement *l_element = n.GetDocument()->NewElement("layer");
		if ((*l_i)->serialize(*l_element))
			n.InsertEndChild(l_element);
	}
	
	return(true);
}

bool
SceneBase::deserialize(XMLElement &n)
{
	XMLElement *l_child;
	for (l_child = n.FirstChildElement("layer") ;
	     l_child;
	     l_child = l_child->NextSiblingElement("layer")) {

		const char *l_id   = l_child->Attribute("id");
		const char *l_type = l_child->Attribute("type");

		SharedSceneLayer l_layer =
		    FactoryBase::Instance()->createSceneLayer(l_type, l_id, *this);

		if (!l_layer) {
			MMWARNING("SceneLayer '" << l_id << "' of type '" << l_type << "' creation failed");
			continue;
		}

		if (!l_layer->deserialize(*l_child)) {
			MMWARNING("SceneLayer '" << l_id << "' of type '" << l_type << "' failed deserialization");
			continue;
		}

		pushLayer(l_layer);
	}
	
	return(true);
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

