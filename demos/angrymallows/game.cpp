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

#include "game.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

MARSHMALLOW_NAMESPACE_USE

#include <core/logger.h>
#include <core/shared.h>
#include <core/type.h>

#include <event/keyboardevent.h>

#include <graphics/camera.h>

#include <game/iscene.h>
#include <game/pausescenelayer.h>

#include "customfactory.h"

#include <tinyxml2.h>

#include <cstdlib>
#include <cstring>

#define STATE_FILENAME "savedstate.xml"

Demo::Demo(const char *f)
    : EngineBase()
    , m_filename(MMSTRDUP(f))
{
}

Demo::~Demo(void)
{
	free(m_filename);
	m_filename = 0;
}

bool
Demo::initialize(void)
{
	setFactory(new CustomFactory);

	if (!EngineBase::initialize())
		return(false);

	eventManager()->connect(this, Event::KeyboardEvent::Type());

	Graphics::Camera::SetPosition(0.f, -0.2f);
	Graphics::Camera::SetZoom(300.f);

	return(reset());
}

void
Demo::finalize(void)
{
	eventManager()->disconnect(this, Event::KeyboardEvent::Type());

	EngineBase::finalize();
}

bool
Demo::handleEvent(const Event::IEvent &e)
{
	using namespace Input;

	if (EngineBase::handleEvent(e))
		return(true);

	if (e.type() != Event::KeyboardEvent::Type())
		return(false);

	const Event::KeyboardEvent &l_kevent =
	    static_cast<const Event::KeyboardEvent &>(e);

	if (l_kevent.action() != Keyboard::KeyPressed)
		return(false);

	if (l_kevent.key() == Keyboard::KBK_RETURN) {
		Game::SharedScene l_scene = sceneManager()->activeScene();
		if (l_scene->getLayer("pause"))
			l_scene->removeLayer("pause");
		else
			l_scene->pushLayer(new Game::PauseSceneLayer("pause", *l_scene));
	} else if (l_kevent.key() == Keyboard::KBK_ESCAPE) {
		stop();
	} else if (l_kevent.key() == Keyboard::KBK_F1) {
		reset();
	} else if (l_kevent.key() == Keyboard::KBK_F2) {
		loadState();
	} else if (l_kevent.key() == Keyboard::KBK_F3) {
		saveState();
	} else return(false);

	return(true);
}

bool
Demo::reset()
{
	sceneManager()->popScene();
	TinyXML::XMLDocument l_document;
	if (l_document.LoadFile(m_filename) != XML_NO_ERROR) {
		MMERROR("Failed to load \'" << m_filename << "\'");
		return(false);
	}
	XMLElement *l_root = l_document.FirstChildElement("marshmallow");
	if (l_root) deserialize(*l_root);
	return(true);
}

bool
Demo::loadState()
{
	sceneManager()->popScene();
	TinyXML::XMLDocument l_document;
	if (l_document.LoadFile(STATE_FILENAME) != XML_NO_ERROR) {
		MMERROR("Failed to load \'" << STATE_FILENAME << "\'");
		return(false);
	}
	XMLElement *l_root = l_document.FirstChildElement("marshmallow");
	if (l_root) deserialize(*l_root);
	return(true);
}

bool
Demo::saveState()
{
	TinyXML::XMLDocument l_document;
	XMLElement  *l_root = l_document.NewElement("marshmallow");
	serialize(*l_root);
	l_document.InsertEndChild(l_root);
	l_document.SaveFile(STATE_FILENAME);
	return(true);
}

