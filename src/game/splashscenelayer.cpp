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

#include "game/splashscenelayer.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"
#include "core/weak.h"

#include "math/point2.h"
#include "math/size2.h"

#include "event/eventmanager.h"
#include "event/ievent.h"
#include "event/keyboardevent.h"

#include "graphics/painter.h"
#include "graphics/quadmesh.h"

#include "game/engine.h"
#include "game/iscene.h"

#include <tinyxml2.h>

MARSHMALLOW_NAMESPACE_USE
using namespace Game;

/******************************************************************************/

namespace {
	enum SplashState
	{
		ssInit     = 0,
		ssFadeIn   = 1,
		ssExposure = 2,
		ssFadeOut  = 3,
		ssFinished = 4
	};
} // namespace

/******************************************************************************/

struct SplashSceneLayer::Private
{
	Private(SplashSceneLayer &i)
	    : _interface(i)
	    , mesh(new Graphics::QuadMesh(2.f, 2.f))
	    , exposure(1.5f)
	    , fade(1.f)
	    , timer(0.f)
	    , state(ssInit)
	    , autoKill(true)
	{
		mesh->setColor(Graphics::Color(0.f, 0.f, 0.f, 0.f));
	}

	void setState(int state);

	SplashSceneLayer &_interface;

	Graphics::SharedQuadMesh mesh;
	float exposure;
	float fade;
	float timer;
	SplashState state;
	bool autoBegin;
	bool autoKill;
};

void
SplashSceneLayer::Private::setState(int s)
{
	if (state == s)
		return;

	switch (static_cast<SplashState>(s)) {
	case ssFadeIn:
		if (state == ssInit || state == ssFinished) break;
	case ssInit:
		timer = 0.;
		mesh->setColor(Graphics::Color(0.f, 0.f, 0.f, 0.f));
		break;
	case ssFinished:
		if (autoKill)
			_interface.kill();
		break;
	case ssFadeOut:
	case ssExposure:
		timer = 0.;
		mesh->setColor(Graphics::Color(1.f, 1.f, 1.f, 1.f));
		break;
	}

	state = static_cast<SplashState>(s);
}

/******************************************************************************/

SplashSceneLayer::SplashSceneLayer(const Core::Identifier &i, IScene &s)
    : SceneLayerBase(i, s, slfUpdateBlock)
    , m_p(new Private(*this))
{
	Game::Engine::Instance()->eventManager()->connect(this, Event::KeyboardEvent::Type());
}

SplashSceneLayer::~SplashSceneLayer(void)
{
	Game::Engine::Instance()->eventManager()->disconnect(this, Event::KeyboardEvent::Type());

	delete m_p;
	m_p = 0;
}

Graphics::SharedQuadMesh
SplashSceneLayer::mesh(void) const
{
	return(m_p->mesh);
}

float
SplashSceneLayer::exposure(void) const
{
	return(m_p->exposure);
}

void
SplashSceneLayer::setExposure(float t)
{
	m_p->exposure = t;
}

float
SplashSceneLayer::fade(void) const
{
	return(m_p->fade);
}

void
SplashSceneLayer::setFade(float t)
{
	m_p->fade = t;
}

bool
SplashSceneLayer::autoKill(void) const
{
	return(m_p->autoKill);
}

void
SplashSceneLayer::setAutoKill(bool ak)
{
	m_p->autoKill = ak;
}

void
SplashSceneLayer::reset(void)
{
	if (m_p->state != ssFinished)
		return;

	m_p->setState(ssFadeIn);
}

bool
SplashSceneLayer::skip(void)
{
	if (m_p->state != ssExposure)
		return(false);

	m_p->setState(ssFadeOut);
	return(true);
}

void
SplashSceneLayer::render(void)
{
	Graphics::Painter::PushMatrix();
	Graphics::Painter::LoadIdentity();

	Graphics::Painter::Draw(*m_p->mesh, Math::Point2(0,0));

	Graphics::Painter::PopMatrix();
}

void
SplashSceneLayer::update(float d)
{
	float l_fiv;

	if (m_p->state == ssFinished)
		return;

	/* update timer */
	m_p->timer += d;

	switch (m_p->state) {
	case ssInit:
		m_p->setState(ssFadeIn);
		break;
	case ssFadeIn:
		if (m_p->timer < m_p->fade) {
			l_fiv = m_p->timer / m_p->fade;
			m_p->mesh->setColor(Graphics::Color(l_fiv, l_fiv, l_fiv, l_fiv));
		} else m_p->setState(ssExposure);
		break;
	case ssFadeOut:
		if (m_p->timer < m_p->fade) {
			l_fiv = 1.f - (m_p->timer / m_p->fade);
			m_p->mesh->setColor(Graphics::Color(l_fiv, l_fiv, l_fiv, l_fiv));
		} else m_p->setState(ssFinished);
		break;
	case ssExposure:
		if (m_p->timer >= m_p->exposure)
			m_p->setState(ssFadeOut);
		break;
	case ssFinished: break;
	}
}

bool
SplashSceneLayer::serialize(XMLElement &n) const
{
	if (!SceneLayerBase::serialize(n))
		return(false);

	n.SetAttribute("fade", m_p->fade);
	n.SetAttribute("exposure", m_p->exposure);

	n.SetAttribute("autokill", m_p->autoKill ? "true" : "false");

	XMLElement *l_mesh = n.GetDocument()->NewElement("mesh");
	if (m_p->mesh && !m_p->mesh->serialize(*l_mesh)) {
		MMWARNING("Splash scene layer '" << id().str() << "' serialization failed to serialize mesh!");
		return(false);
	}
	n.InsertEndChild(l_mesh);

	return(true);
}

bool
SplashSceneLayer::deserialize(XMLElement &n)
{
	if (!SceneLayerBase::deserialize(n))
		return(false);

	n.QueryFloatAttribute("fade", &m_p->fade);
	n.QueryFloatAttribute("exposure", &m_p->exposure);

	const char *l_autokill = n.Attribute("autokill");
	m_p->autoKill = (l_autokill && l_autokill[0] == 't');

	XMLElement *l_child = n.FirstChildElement("mesh");
	if (!l_child) {
		MMWARNING("Splash scene layer '" << id().str() << "' deserialized without a mesh!");
		return(false);
	}
	m_p->mesh->deserialize(*l_child);

	return(true);
}

bool
SplashSceneLayer::handleEvent(const Event::IEvent &e)
{
	if (e.type() == Event::KeyboardEvent::Type())
		return(skip());
	return(false);
}

const Core::Type &
SplashSceneLayer::Type(void)
{
	static const Core::Type s_type("Game::SplashSceneLayer");
	return(s_type);
}

