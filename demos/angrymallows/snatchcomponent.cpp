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

#include "snatchcomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <Box2D/Box2D.h>

#include <core/identifier.h>
#include <core/logger.h>

#include <math/size2.h>

#include <event/eventmanager.h>
#include <event/keyboardevent.h>

#include <graphics/viewport.h>

#include <game/enginebase.h>
#include <game/entityscenelayer.h>
#include <game/ientity.h>
#include <game/iscene.h>

SnatchComponent::SnatchComponent(const Core::Identifier &i, Game::IEntity &e)
    : ComponentBase(i, e)
    , m_registered(false)
{
}

SnatchComponent::~SnatchComponent(void)
{
	if (m_layer && m_registered)
		m_layer->deregisterEntity(entity());
}

void
SnatchComponent::update(float)
{
	if (m_registered)
		return;

	if (!m_layer) {
		Game::WeakSceneLayer l_layer = entity().layer().scene().getLayerType("SnatcherLayer");
		m_layer = l_layer.cast<SnatcherLayer>();
	}

	if (m_layer) {
		m_layer->registerEntity(entity());
		m_registered = true;
	}
}

const Core::Type &
SnatchComponent::Type(void)
{
	static const Core::Type s_type("SnatchComponent");
	return(s_type);
}

