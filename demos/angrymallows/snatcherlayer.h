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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef SNATCHERLAYER_H
#define SNATCHERLAYER_H 1

#include <game/scenelayerbase.h>
#include <event/ieventlistener.h>

#include <vector>

#include <core/shared.h>
#include <game/ientity.h>

MARSHMALLOW_NAMESPACE_USE

class SnatcherLayer : public Game::SceneLayerBase
                    , public Event::IEventListener
{
	enum SnatchState {
		ssIdle = 0,
		ssNext = 1,
		ssTimeout = 2
	};

	typedef std::vector<Core::Identifier> UIDList;
	UIDList m_entities;
	SnatchState m_state;
	float m_timeout;
	int m_active;

	NO_ASSIGN_COPY(SnatcherLayer);
public:

	SnatcherLayer(const Core::Identifier &identifier,
	    Game::IScene &scene);
	virtual ~SnatcherLayer(void);

	void registerEntity(Game::IEntity &entity);
	void deregisterEntity(Game::IEntity &entity);

public: /* virtual */

	VIRTUAL const Core::Type & type(void) const
	    { return(Type()); }

	VIRTUAL void render(void) {};
	VIRTUAL void update(float delta);

	VIRTUAL bool handleEvent(const Event::IEvent &event);

public: /* static */

	static const Core::Type & Type(void);

protected:

	void setState(SnatchState state);
};
typedef Core::Shared<SnatcherLayer> SharedSnatcherLayer;
typedef Core::Weak<SnatcherLayer> WeakSnatcherLayer;

#endif
