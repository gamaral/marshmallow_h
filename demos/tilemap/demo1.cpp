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

#include <core/identifier.h>

#include <math/vector2.h>

#include <graphics/factory.h>
#include <graphics/tileset.h>
#include <graphics/viewport.h>

#include <game/box2d/box2dscenelayer.h>
#include <game/enginebase.h>
#include <game/scene.h>
#include <game/scenemanager.h>
#include <game/tilemapscenelayer.h>

#include <extra/tmxloader.h>

#include "customfactory.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Core;

class Demo : public Game::EngineBase
{
	int m_stop_timer;

public:

	Demo(void)
	: EngineBase(),
	  m_stop_timer(0)
	{
	}

	VIRTUAL bool initialize(void)
	{
		setFactory(new CustomFactory);

		if (!EngineBase::initialize())
			return(false);
		Graphics::Viewport::Redisplay(640.f, 480.f);

		Game::SharedScene l_scene(new Game::Scene("main"));

		/* box2d layer */
		Game::SharedBox2DSceneLayer l_box2d_layer
		    (new Game::Box2DSceneLayer("box2d", *l_scene));
		l_box2d_layer->setGravity(Math::Vector2(0., 0.));
		l_scene->pushLayer(l_box2d_layer.staticCast<Game::ISceneLayer>());

		/* load tmx tilemap */
		Extra::TMXLoader m_tmxloader(*l_scene);
		m_tmxloader.load("assets/sewers.tmx");
		assert(m_tmxloader.isLoaded() && "TMX tilemap failed to load.");

		sceneManager()->pushScene(l_scene);

		Graphics::Viewport::SetCamera(Math::Vector3(0, 50, 0.25));

		return(true);
	}

	VIRTUAL void second(void)
	{
		EngineBase::second();

		if (++m_stop_timer == 60)
			stop();
	}
};

int
MMain(int argc, char *argv[])
{
	UNUSED(argc);
	UNUSED(argv);
	CHDIR(DEMO_CWD);

	return(Demo().run());
}

