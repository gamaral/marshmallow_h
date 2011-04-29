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

#include "game.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

MARSHMALLOW_NAMESPACE_USE;

#include <tinyxml.h>

#include <math/vector2.h>
#include <math/vector3.h>
#include <graphics/viewport.h>
#include <game/box2dscenelayer.h>
#include <game/entityfactory.h>
#include <game/entityscenelayer.h>
#include <game/graphicfactory.h>
#include <game/scene.h>
#include <game/scenebuilder.h>
#include <game/scenelayerfactory.h>

#include "componentfactory.h"

Demo::Demo(void)
    : EngineBase(),
      m_stop_timer(0)
{
}

void
Demo::initialize(void)
{
	EngineBase::initialize();

	ComponentFactory l_cf;
	Game::EntityFactory l_ef;
	Game::GraphicFactory l_gf;
	Game::SceneLayerFactory l_slf;

	Game::SharedScene l_scene(new Game::Scene("main"));
	sceneManager()->pushScene(l_scene);

	Game::SceneBuilder builder;
	builder.load("demos/marshmallow/assets/mainscene.xml", *l_scene);
}

void
Demo::finalize(void)
{
	EngineBase::finalize();
}

void
Demo::second(void)
{
	EngineBase::second();

	if (++m_stop_timer == 8) {
		TinyXML::TiXmlDocument l_document;
		TinyXML::TiXmlElement l_root("scene");
		sceneManager()->activeScene()->serialize(l_root);
		l_document.InsertEndChild(l_root);
		l_document.SaveFile("scene.xml");
		stop();
	}
}

