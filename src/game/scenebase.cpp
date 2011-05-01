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

#include "game/scenebase.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"
#include "game/iscenelayer.h"
#include "game/factorybase.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Game;

SceneBase::SceneBase(const Core::Identifier &i)
    : m_layers(),
      m_id(i)
{
}

SceneBase::~SceneBase(void)
{
	m_layers.clear();
}

void
SceneBase::pushLayer(SharedSceneLayer l)
{
	m_layers.push_front(l);
}

void
SceneBase::popLayer(void)
{
	m_layers.pop_front();
}

void
SceneBase::removeLayer(const Core::Identifier &i)
{
	SceneLayerList::const_iterator l_i;
	SceneLayerList::const_iterator l_c = m_layers.end();

	/* maybe replace later with a map if required */
	for (l_i = m_layers.begin(); l_i != l_c; ++l_i)
		if ((*l_i)->id() == i) {
			m_layers.remove(*l_i);
			return;
		}
}

SharedSceneLayer
SceneBase::getLayer(const Core::Identifier &i) const
{
	SceneLayerList::const_iterator l_i;
	SceneLayerList::const_iterator l_c = m_layers.end();

	/* maybe replace later with a map if required */
	for (l_i = m_layers.begin(); l_i != l_c; ++l_i) {
		if ((*l_i)->id() == i)
			return(*l_i);
	}

	return(SharedSceneLayer());
}

SharedSceneLayer
SceneBase::getLayerType(const Core::Type &t) const
{
	SceneLayerList::const_iterator l_i;
	SceneLayerList::const_iterator l_c = m_layers.end();

	/* maybe replace later with a map if required */
	for (l_i = m_layers.begin(); l_i != l_c; ++l_i) {
		if ((*l_i)->type() == t)
			return(*l_i);
	}

	return(SharedSceneLayer());
}

void
SceneBase::render(void)
{
	if (m_layers.empty()) return;

	SceneLayerList::const_iterator l_i;
	SceneLayerList::const_iterator l_b = m_layers.begin();
	SceneLayerList::const_iterator l_c = --m_layers.end();

	for (l_i = l_b; l_i != l_c; ++l_i)
		if ((*l_i)->flags() & slfRenderBlock)
			break;

	do { (*l_i)->render(); } while(l_i-- != l_b);
}

void
SceneBase::update(TIME d)
{
	if (m_layers.empty()) return;

	SceneLayerList::const_iterator l_i;
	SceneLayerList::const_iterator l_b = m_layers.begin();
	SceneLayerList::const_iterator l_c = --m_layers.end();

	for (l_i = l_b; l_i != l_c; ++l_i)
		if ((*l_i)->flags() & slfUpdateBlock)
			break;

	do { (*l_i)->update(d); } while(l_i-- != l_b);
}

bool
SceneBase::serialize(TinyXML::TiXmlElement &n) const
{
	n.SetAttribute("id", id().str().c_str());
	n.SetAttribute("type", type().str().c_str());

	SceneLayerList::const_reverse_iterator l_i;
	SceneLayerList::const_reverse_iterator l_c = m_layers.rend();
	for (l_i = m_layers.rbegin(); l_i != l_c; ++l_i) {
		TinyXML::TiXmlElement l_element("layer");
		if ((*l_i)->serialize(l_element))
			n.InsertEndChild(l_element);
	}
	
	return(true);
}

bool
SceneBase::deserialize(TinyXML::TiXmlElement &n)
{
	TinyXML::TiXmlElement *l_child;
	for (l_child = n.FirstChildElement("layer") ;
	     l_child;
	     l_child = l_child->NextSiblingElement("layer")) {

		const char *l_id   = l_child->Attribute("id");
		const char *l_type = l_child->Attribute("type");

		SharedSceneLayer l_layer =
		    FactoryBase::Instance()->createSceneLayer(l_type, l_id, *this);

		if (!l_layer) {
			WARNING("SceneLayer '%s' of type '%s' creation failed", l_id, l_type);
			continue;
		}

		if (!l_layer->deserialize(*l_child)) {
			WARNING("SceneLayer '%s' of type '%s' failed deserialization", l_id, l_type);
			continue;
		}

		pushLayer(l_layer);
	}
	
	return(true);
}

