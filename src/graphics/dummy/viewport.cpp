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

#include "graphics/viewport_p.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include "graphics/camera.h"
#include "graphics/color.h"
#include "graphics/display.h"
#include "graphics/painter_p.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace { /******************************************** Anonymous Namespace */
	Graphics::Display dpy;
	bool active;
} /****************************************************** Anonymous Namespace */

namespace Graphics { /************************************ Graphics Namespace */

bool
Viewport::Active(void)
{
	return(active);
}

bool
Viewport::Initialize(void)
{
	/* default display display */
	dpy.depth      = MARSHMALLOW_VIEWPORT_DEPTH;
	dpy.fullscreen = MARSHMALLOW_VIEWPORT_FULLSCREEN;
	dpy.height     = MARSHMALLOW_VIEWPORT_HEIGHT;
	dpy.vsync      = MARSHMALLOW_VIEWPORT_VSYNC;
	dpy.width      = MARSHMALLOW_VIEWPORT_WIDTH;

	/*
	 * Initial Camera Reset (IMPORTANT)
	 */
	Camera::Reset();

	/*
	 * Initial Background Color (IMPORTANT)
	 */
	Painter::SetBackgroundColor(Color::Black());

	active = false;

	return(true);
}

void
Viewport::Finalize(void)
{
	MMDEBUG("Dummy viewport finalized.");
}

bool
Viewport::Setup(const Graphics::Display &display)
{
	MMDEBUG("Dummy viewport setup.");

	if (active) {
		Painter::Finalize();
		active = false;
	}

	dpy = display;

	Camera::Update();

	Painter::Initialize();

#if MARSHMALLOW_DUMMY_FAIL
	return(active = false);
#else
	return(active = true);
#endif
}

void
Viewport::Tick(void)
{
}

void
Viewport::SwapBuffer(void)
{
}

const Graphics::Display &
Viewport::Display(void)
{
	return(dpy);
}

const Math::Size2f &
Viewport::Size(void)
{
	const static Math::Size2f s_size(MARSHMALLOW_VIEWPORT_WIDTH,
	                                 MARSHMALLOW_VIEWPORT_HEIGHT);
	return(s_size);
}

const Math::Size2i &
Viewport::WindowSize(void)
{
	static Math::Size2i s_size;
	s_size.set(dpy.width, dpy.height);
	return(s_size);
}

const Core::Type &
Viewport::Type(void)
{
	static const Core::Type s_type("DUMMY");
	return(s_type);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

