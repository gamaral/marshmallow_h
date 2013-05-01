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
#include "core/type.h"

#include "graphics/color.h"
#include "graphics/painter.h"

#include "game/factory.h"
#include "game/iscenelayer.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

struct SceneBase::Private
{
	Private(const Core::Identifier &i)
	    : layers()
	    , bgcolor(Graphics::Color::Black())
	    , id(i)
	    , active(false)
	{}

	~Private(void);

	inline void
	pushLayer(Game::ISceneLayer *layer);

	inline Game::ISceneLayer *
	popLayer(void);

	inline void
	removeLayer(Game::ISceneLayer *layer);

	inline bool
	removeLayer(const Core::Identifier &identifier);

	inline Game::ISceneLayer *
	getLayer(const Core::Identifier &identifier) const;

	inline Game::ISceneLayer *
	getLayerType(const Core::Type &type) const;

	SceneLayerList layers;
	Graphics::Color bgcolor;
	const Core::Identifier &id;
	bool active;
};

SceneBase::Private::~Private(void)
{
	/* free layers left behind */
	while (!layers.empty()) {
		ISceneLayer *l_layer = layers.back();
		if (l_layer->flags() & slfAutoFree)
			delete l_layer;
		layers.pop_back();
	}
}

void
SceneBase::Private::pushLayer(Game::ISceneLayer *l)
{
	layers.push_front(l);
}


Game::ISceneLayer *
SceneBase::Private::popLayer(void)
{
	if (layers.empty())
		return(0);

	ISceneLayer *l_layer = layers.front();
	layers.pop_front();
	return(l_layer);
}

void
SceneBase::Private::removeLayer(Game::ISceneLayer *l)
{
	layers.remove(l);
}

bool
SceneBase::Private::removeLayer(const Core::Identifier &i)
{
	SceneLayerList::const_iterator l_i;
	const SceneLayerList::const_iterator l_c = layers.end();

	/* maybe replace later with a map if required */
	for (l_i = layers.begin(); l_i != l_c; ++l_i)
		if ((*l_i)->id() == i) {
			ISceneLayer *l_slayer = *l_i;
			layers.remove(l_slayer);
			return(true);
		}
	return(false);
}


Game::ISceneLayer *
SceneBase::Private::getLayer(const Core::Identifier &i) const
{
	SceneLayerList::const_iterator l_i;
	const SceneLayerList::const_iterator l_c = layers.end();

	/* maybe replace later with a map if required */
	for (l_i = layers.begin(); l_i != l_c; ++l_i) {
		if ((*l_i)->id() == i)
			return(*l_i);
	}

	return(0);
}


Game::ISceneLayer *
SceneBase::Private::getLayerType(const Core::Type &t) const
{
	SceneLayerList::const_iterator l_i;
	const SceneLayerList::const_iterator l_c = layers.end();

	/* maybe replace later with a map if required */
	for (l_i = layers.begin(); l_i != l_c; ++l_i) {
		if ((*l_i)->type() == t)
			return(*l_i);
	}

	return(0);
}

SceneBase::SceneBase(const Core::Identifier &i)
    : PIMPL_CREATE_X(i)
{
}

SceneBase::~SceneBase(void)
{
	PIMPL_DESTROY;
}

const Core::Identifier &
SceneBase::id(void) const
{
	return(PIMPL->id);
}

bool
SceneBase::isActive(void) const
{
	return(PIMPL->active);
}

void
SceneBase::pushLayer(ISceneLayer *l)
{
	PIMPL->pushLayer(l);
}

ISceneLayer *
SceneBase::popLayer(void)
{
	return(PIMPL->popLayer());
}

void
SceneBase::removeLayer(ISceneLayer *l)
{
	return(PIMPL->removeLayer(l));
}

bool
SceneBase::removeLayer(const Core::Identifier &i)
{
	return(PIMPL->removeLayer(i));
}

ISceneLayer *
SceneBase::getLayer(const Core::Identifier &i) const
{
	return(PIMPL->getLayer(i));
}

ISceneLayer *
SceneBase::getLayerType(const Core::Type &t) const
{
	return(PIMPL->getLayerType(t));
}

const SceneLayerList &
SceneBase::getLayers(void) const
{
	return(PIMPL->layers);
}

const Graphics::Color &
SceneBase::background(void) const
{
	return(PIMPL->bgcolor);
}

void
SceneBase::setBackground(const Graphics::Color &color)
{
	using namespace Graphics;

	PIMPL->bgcolor = color;

	if (isActive())
		Painter::SetBackgroundColor(PIMPL->bgcolor);
}

void
SceneBase::activate(void)
{
	using namespace Graphics;

	Painter::SetBackgroundColor(PIMPL->bgcolor);

	PIMPL->active = true;
}

void
SceneBase::deactivate(void)
{
	PIMPL->active = false;
}

void
SceneBase::render(void)
{
	if (PIMPL->layers.empty()) return;

	SceneLayerList::const_iterator l_i;
	SceneLayerList::const_iterator l_b = PIMPL->layers.begin();
	SceneLayerList::const_iterator l_c = --PIMPL->layers.end();

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
	if (PIMPL->layers.empty()) return;

	SceneLayerList::const_iterator l_i;
	SceneLayerList::const_iterator l_b = PIMPL->layers.begin();
	const SceneLayerList::const_iterator l_c = --PIMPL->layers.end();

	for (l_i = l_b; l_i != l_c; ++l_i)
		if ((*l_i)->flags() & slfUpdateBlock)
			break;

	/* TODO: polish later if possible */
	bool l_finished = false;
	do {
		ISceneLayer *l_slayer = (*l_i);

		if (l_i == l_b) l_finished = true;
		else l_i--;

		if (l_slayer->isZombie()) {
			PIMPL->layers.remove(l_slayer);
			if (l_slayer->flags() & slfAutoFree)
				delete l_slayer;
		} else l_slayer->update(d);
	} while(!l_finished);
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

