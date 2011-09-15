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

#include "graphics/viewport.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Graphics;

/******************************************************************************/

namespace
{
	static Math::Triplet s_camera(0.f, 0.f, 1.f);
} // namespace

/******************************************************************************/

bool
Viewport::Initialize(int w, int h, int d, bool f)
{
	MMUNUSED(w);
	MMUNUSED(h);
	MMUNUSED(d);
	MMUNUSED(f);
	MMINFO("Dummy viewport initialized a %d bit %dx%d display (%s)", d, w, h, f ? "FULLSCREEN" : "WINDOWED");
	return(true);
}

void
Viewport::Finalize(void)
{
	MMINFO1("Dummy viewport finalized");
}

bool
Viewport::Redisplay(int w, int h, int d, bool f)
{
	MMUNUSED(w);
	MMUNUSED(h);
	MMUNUSED(d);
	MMUNUSED(f);
	MMINFO("Dummy viewport redisplayed using a %d bit %dx%d display (%s)", d, w, h, f ? "FULLSCREEN" : "WINDOWED");
	return(true);
}

void
Viewport::Tick(TIME t)
{
	MMUNUSED(t);
}

void
Viewport::SwapBuffer(void)
{
}

const Math::Triplet &
Viewport::Camera(void)
{
	return(s_camera);
}

void
Viewport::MoveCamera(const Math::Triplet &c)
{
	s_camera = c;
}

void
Viewport::VisibleArea(Math::Point2 *tl, Math::Point2 *br)
{
	if (tl) *tl = Math::Point2(-DEFAULT_VIEWPORT_VWIDTH / 2.f,  DEFAULT_VIEWPORT_VHEIGHT / 2.f);
	if (br) *br = Math::Point2( DEFAULT_VIEWPORT_VWIDTH / 2.f, -DEFAULT_VIEWPORT_VHEIGHT / 2.f);
}

const Math::Size2f &
Viewport::Size(void)
{
	const static Math::Size2f s_size(DEFAULT_VIEWPORT_VWIDTH, DEFAULT_VIEWPORT_VHEIGHT);
	return(s_size);
}

const Math::Size2i &
Viewport::WindowSize(void)
{
	const static Math::Size2i s_wsize(DEFAULT_VIEWPORT_WIDTH, DEFAULT_VIEWPORT_HEIGHT);
	return(s_wsize);
}

const Core::Type &
Viewport::Type(void)
{
	static const Core::Type s_type("DUMMY");
	return(s_type);
}

