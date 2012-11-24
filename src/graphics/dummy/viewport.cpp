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
#include "core/type.h"

#include "graphics/camera.h"
#include "graphics/color.h"
#include "graphics/display.h"
#include "graphics/painter_p.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */
namespace Dummy { /******************************** Graphics::Dummy Namespace */
namespace { /************************* Graphics::Dummy::<anonymous> Namespace */

Graphics::Display s_dpy;
bool              s_active;

} /*********************************** Graphics::Dummy::<anonymous> Namespace */
} /************************************************ Graphics::Dummy Namespace */

bool
Viewport::Active(void)
{
	using namespace Dummy;
	return(s_active);
}

bool
Viewport::Initialize(void)
{
	using namespace Dummy;

	/* default display display */
	s_dpy.depth      = MARSHMALLOW_VIEWPORT_DEPTH;
	s_dpy.fullscreen = MARSHMALLOW_VIEWPORT_FULLSCREEN;
	s_dpy.height     = MARSHMALLOW_VIEWPORT_HEIGHT;
	s_dpy.vsync      = MARSHMALLOW_VIEWPORT_VSYNC;
	s_dpy.width      = MARSHMALLOW_VIEWPORT_WIDTH;

	/*
	 * Initial Camera Reset (IMPORTANT)
	 */
	Camera::Reset();

	/*
	 * Initial Background Color (IMPORTANT)
	 */
	Painter::SetBackgroundColor(Color::Black());

	s_active = false;

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
	using namespace Dummy;

	MMDEBUG("Dummy viewport setup.");

	if (s_active) {
		Painter::Finalize();
		s_active = false;
	}

	s_dpy = display;

	Camera::Update();

	Painter::Initialize();

#if MARSHMALLOW_DUMMY_FAIL
	return(s_active = false);
#else
	return(s_active = true);
#endif
}

void
Viewport::Tick(float)
{
}

void
Viewport::SwapBuffer(void)
{
}

const Graphics::Display &
Viewport::Display(void)
{
	using namespace Dummy;
	return(s_dpy);
}

const Math::Size2f &
Viewport::Size(void)
{
	using namespace Dummy;

	const static Math::Size2f s_size(MARSHMALLOW_VIEWPORT_WIDTH,
	                                 MARSHMALLOW_VIEWPORT_HEIGHT);
	return(s_size);
}

const Math::Size2i &
Viewport::WindowSize(void)
{
	using namespace Dummy;

	static Math::Size2i s_size;
	s_size.set(s_dpy.width, s_dpy.height);
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

