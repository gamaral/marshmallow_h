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

#include <core/logger.h>
#include <math/vector2.h>
#include <math/vector3.h>
#include <graphics/viewport.h>
#include <game/entityfactory.h>
#include <game/entityscenelayer.h>
#include <game/graphicfactory.h>
#include <game/scene.h>
#include <game/scenefactory.h>
#include <game/scenelayerfactory.h>

#include "componentfactory.h"

Demo::Demo(const char *f)
    : EngineBase(),
      m_filename(STRDUP(f)),
      m_stop_timer(0)
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
	if (!EngineBase::initialize())
		return(false);

	ComponentFactory l_cf;
	Game::EntityFactory l_ef;
	Game::GraphicFactory l_gf;
	Game::SceneFactory l_sf;
	Game::SceneLayerFactory l_slf;

	{	/* derialization test */
		TinyXML::TiXmlDocument l_document;
		if (!l_document.LoadFile(m_filename)) {
			ERROR("Failed to load '%s'", m_filename);
			return(false);
		}
		TinyXML::TiXmlElement *l_root = l_document.FirstChildElement("marshmallow");
		if (l_root) deserialize(*l_root);
	}

	{	/* pre-run serialization test */
		TinyXML::TiXmlDocument l_document;
		TinyXML::TiXmlElement  l_root("marshmallow");
		serialize(l_root);
		l_document.InsertEndChild(l_root);
		l_document.SaveFile("mainscene.xml");
	}

	return(true);
}

void
Demo::second(void)
{
	EngineBase::second();

	if (++m_stop_timer == 8) {
		stop();
	} else if (m_stop_timer == 4) {
		TinyXML::TiXmlDocument l_document;
		TinyXML::TiXmlElement l_root("marshmallow");
		serialize(l_root);
		l_document.InsertEndChild(l_root);
		l_document.SaveFile("midscene.xml");
	}
}

