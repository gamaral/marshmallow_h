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

#include "game/splashscenelayer.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/logger.h"
#include "core/type.h"

#include "math/point2.h"

#include "event/eventmanager.h"
#include "event/keyboardevent.h"
#include "event/touchevent.h"

#include "graphics/backend.h"
#include "graphics/itexturedata.h"
#include "graphics/painter.h"
#include "graphics/quadmesh.h"

#include "game/engine.h"
#include "game/iengine.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

struct SplashSceneLayer::Private
{
	enum State
	{
		Init     = 0,
		FadeIn   = 1,
		Exposure = 2,
		FadeOut  = 3,
		Finished = 4
	};

	Private(SplashSceneLayer &i)
	    : layer(i)
	    , mesh(new Graphics::QuadMesh(2.f, 2.f))
	    , exposure(1.5f)
	    , fade(1.f)
	    , timer(0.f)
	    , state(Init)
	    , autoKill(true)
	{
		mesh->setColor(Graphics::Color(0.f, 0.f, 0.f, 0.f));
	}

	inline void setState(State state);

	inline void calculateQuadScale(void);

	inline void update(float d);

	SplashSceneLayer &layer;

	Graphics::QuadMesh *mesh;
	float exposure;
	float fade;
	float timer;
	State state;
	bool autoBegin;
	bool autoKill;
};

void
SplashSceneLayer::Private::setState(State s)
{
	if (state == s)
		return;

	switch (s) {
	case FadeIn:
		if (state == Init || state == Finished) break;
	case Init:
		timer = 0.;
		mesh->setColor(Graphics::Color(0.f, 0.f, 0.f, 0.f));
		break;
	case Finished:
		if (autoKill)
			layer.kill();
		break;
	case FadeOut:
	case Exposure:
		timer = 0.;
		mesh->setColor(Graphics::Color(1.f, 1.f, 1.f, 1.f));
		break;
	}

	state = s;
}

void
SplashSceneLayer::Private::calculateQuadScale(void)
{
	if (!mesh->textureData())
		return;

	const Math::Size2i &l_texture_size = mesh->textureData()->size();
	const Math::Size2i &l_window_size = Graphics::Backend::WindowSize();

	float l_pixel_scale_x =
	    static_cast<float>(l_texture_size.width) /
	        static_cast<float>(l_window_size.width);
	float l_pixel_scale_y =
	    static_cast<float>(l_texture_size.height) /
	        static_cast<float>(l_window_size.height);

	mesh->setScale(l_pixel_scale_x, l_pixel_scale_y);
}

void
SplashSceneLayer::Private::update(float d)
{
	float l_fiv;

	if (state == Finished)
		return;

	/* update timer */
	timer += d;

	switch (state) {
	case Init:
		setState(FadeIn);

		/* adjust quad scale based on texture data */
		calculateQuadScale();

		break;
	case FadeIn:
		if (timer < fade) {
			l_fiv = timer / fade;
			mesh->setColor(Graphics::Color(l_fiv, l_fiv, l_fiv, l_fiv));
		} else setState(Exposure);
		break;
	case FadeOut:
		if (timer < fade) {
			l_fiv = 1.f - (timer / fade);
			mesh->setColor(Graphics::Color(l_fiv, l_fiv, l_fiv, l_fiv));
		} else setState(Finished);
		break;
	case Exposure:
		if (timer >= exposure)
			setState(FadeOut);
		break;
	case Finished: break;
	}
}

SplashSceneLayer::SplashSceneLayer(const Core::Identifier &i, Game::IScene *s)
    : SceneLayer(i, s, UpdateBlock)
    , PIMPL_CREATE_X(*this)
{
	Game::Engine::Instance()->eventManager()->connect(this, Event::KeyboardEvent::Type());
	Game::Engine::Instance()->eventManager()->connect(this, Event::TouchEvent::Type());
}

SplashSceneLayer::~SplashSceneLayer(void)
{
	Game::Engine::Instance()->eventManager()->disconnect(this, Event::TouchEvent::Type());
	Game::Engine::Instance()->eventManager()->disconnect(this, Event::KeyboardEvent::Type());

	PIMPL_DESTROY;
}

Graphics::QuadMesh *
SplashSceneLayer::mesh(void) const
{
	return(PIMPL->mesh);
}

float
SplashSceneLayer::exposure(void) const
{
	return(PIMPL->exposure);
}

void
SplashSceneLayer::setExposure(float t)
{
	PIMPL->exposure = t;
}

float
SplashSceneLayer::fade(void) const
{
	return(PIMPL->fade);
}

void
SplashSceneLayer::setFade(float t)
{
	PIMPL->fade = t;
}

bool
SplashSceneLayer::autoKill(void) const
{
	return(PIMPL->autoKill);
}

void
SplashSceneLayer::setAutoKill(bool ak)
{
	PIMPL->autoKill = ak;
}

void
SplashSceneLayer::reset(void)
{
	if (PIMPL->state != Private::Finished)
		return;

	PIMPL->setState(Private::FadeIn);
}

bool
SplashSceneLayer::skip(void)
{
	if (PIMPL->state != Private::Exposure)
		return(false);

	PIMPL->setState(Private::FadeOut);
	return(true);
}

void
SplashSceneLayer::render(void)
{
	Graphics::Painter::PushMatrix();
	Graphics::Painter::LoadIdentity();

	Graphics::Painter::Draw(*PIMPL->mesh, Math::Point2(0, 0));

	Graphics::Painter::PopMatrix();
}

void
SplashSceneLayer::update(float d)
{
	PIMPL->update(d);
}

bool
SplashSceneLayer::handleEvent(const Event::IEvent &e)
{
	if (e.type() == Event::KeyboardEvent::Type()
	    || e.type() == Event::KeyboardEvent::Type())
		return(skip());
	return(false);
}

const Core::Type &
SplashSceneLayer::Type(void)
{
	static const Core::Type s_type("Game::SplashSceneLayer");
	return(s_type);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

