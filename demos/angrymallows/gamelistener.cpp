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

#include "gamelistener.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <event/keyboardevent.h>
#include <graphics/quadmesh.h>
#include <graphics/textureasset.h>
#include <game/iscene.h>
#include <game/pausescenelayer.h>
#include <game/splashscenelayer.h>

#include "game.h"

GameListener::GameListener(Game::IEngine &e)
    : m_engine(e)
{
}

GameListener::~GameListener(void)
{
}

bool
GameListener::handleEvent(const Event::IEvent &e)
{
	if (e.type() != Event::KeyboardEvent::Type)
		return(false);

	const Event::KeyboardEvent &l_kevent =
	    static_cast<const Event::KeyboardEvent &>(e);

	if (l_kevent.action() != Event::KeyPressed)
		return(false);

	if (l_kevent.key() == Event::KEY_RETURN) {
		Game::SharedScene l_scene = m_engine.sceneManager()->activeScene();
		if (l_scene->getLayer("pause"))
			l_scene->removeLayer("pause");
		else
			l_scene->pushLayer(new Game::PauseSceneLayer("pause", *l_scene));
	} else if (l_kevent.key() == Event::KEY_ESCAPE)
		m_engine.stop();

	return(true);
}

