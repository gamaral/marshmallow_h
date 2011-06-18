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

#include "game/splashscenelayer.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "math/point2.h"
#include "math/size2.h"
#include "graphics/painter.h"
#include "graphics/quadmesh.h"
#include "graphics/viewport.h"
#include "game/iscene.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Game;

const Core::Type SplashSceneLayer::Type("Game::SplashSceneLayer");

SplashSceneLayer::SplashSceneLayer(const Core::Identifier &i, IScene &s)
    : SceneLayerBase(i, s, slfUpdateBlock),
      m_exposure(1.),
      m_fade(1.),
      m_timer(0.),
      m_state(ssStopped),
      m_autoRemove(true)
{
	m_mesh = new Graphics::QuadMesh(Math::Rect2(Graphics::Viewport::Size()));;
	m_mesh->setColor(Graphics::Color(0.f, 0.f, 0.f, 0.f));
}

SplashSceneLayer::~SplashSceneLayer(void)
{
}

Graphics::SharedQuadMesh
SplashSceneLayer::mesh(void) const
{
	return(m_mesh);
}

void
SplashSceneLayer::begin(void)
{
	if (m_state != ssStopped)
		return;

	setState(ssFadeIn);
}

void
SplashSceneLayer::skip(void)
{
	if (m_state == ssStopped)
		return;

	setState(ssFadeOut);
}

void
SplashSceneLayer::render(void)
{
	Graphics::Painter::Draw(*m_mesh, Math::Point2(0,0));
}

void
SplashSceneLayer::update(TIME d)
{
	float l_fiv;

	if (m_state == ssStopped)
		return;

	/* update timer */
	m_timer += d;

	switch (m_state) {
	case ssFadeIn:
		if (m_timer < m_fade) {
			l_fiv = m_timer / m_fade;
			m_mesh->setColor(Graphics::Color(l_fiv, l_fiv, l_fiv, l_fiv));
		} else setState(ssExposure);
		break;
	case ssFadeOut:
		if (m_timer < m_fade) {
			l_fiv = 1.f - (m_timer / m_fade);
			m_mesh->setColor(Graphics::Color(l_fiv, l_fiv, l_fiv, l_fiv));
		} else setState(ssStopped);
		break;
	case ssExposure:
		if (m_timer >= m_exposure)
			setState(ssFadeOut);
		break;
	case ssStopped: break;
	}
}

void
SplashSceneLayer::setState(SplashState s)
{
	if (m_state == s)
		return;

	switch (s) {
	case ssFadeIn:
		if (m_state == ssStopped) break;
	case ssStopped:
		if (m_autoRemove) {
			kill();
		} else {
			m_timer = 0.;
			m_mesh->setColor(Graphics::Color(0.f, 0.f, 0.f, 0.f));
		}
		break;
	case ssFadeOut:
		if (m_state == ssFadeIn) break;
	case ssExposure:
		m_timer = 0.;
		m_mesh->setColor(Graphics::Color(1.f, 1.f, 1.f, 1.f));
		break;
	}

	m_state = s;
}

