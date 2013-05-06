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

#include "graphics/backend_p.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/logger.h"
#include "core/platform.h"

#include "graphics/camera.h"
#include "graphics/color.h"
#include "graphics/display.h"
#include "graphics/painter_p.h"

#include "game/config.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */
namespace Dummy { /******************************** Graphics::Dummy Namespace */
namespace { /************************* Graphics::Dummy::<anonymous> Namespace */

	static Graphics::Display s_dpy;
	static bool              s_active;

} /*********************************** Graphics::Dummy::<anonymous> Namespace */
} /************************************************ Graphics::Dummy Namespace */

bool
Backend::Active(void)
{
	using namespace Dummy;
	return(s_active);
}

bool
Backend::Initialize(void)
{
	using namespace Dummy;

	/* default display display */
	s_dpy.depth      = MARSHMALLOW_GRAPHICS_DEPTH;
	s_dpy.fullscreen = MARSHMALLOW_GRAPHICS_FULLSCREEN;
	s_dpy.height     = MARSHMALLOW_GRAPHICS_HEIGHT;
	s_dpy.vsync      = MARSHMALLOW_GRAPHICS_VSYNC;
	s_dpy.width      = MARSHMALLOW_GRAPHICS_WIDTH;

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
Backend::Finalize(void)
{
	MMDEBUG("Dummy backend finalized.");
}

bool
Backend::Setup(const Graphics::Display &display)
{
	using namespace Dummy;

	MMDEBUG("Dummy backend setup.");

	if (s_active) {
		Painter::Finalize();
		s_active = false;
	}

	s_dpy = display;

	Camera::Update();

	Painter::Initialize();

#ifdef MARSHMALLOW_DUMMY_FAIL
	return(s_active = false);
#else
	return(s_active = true);
#endif
}

void
Backend::Tick(float)
{
}

void
Backend::Finish(void)
{
	using namespace Dummy;

	/* simulated slow swap */
	if (s_dpy.vsync > 0)
		Core::Platform::Sleep((1000 * s_dpy.vsync)/MARSHMALLOW_ENGINE_FRAMERATE);
	else 
		Core::Platform::Sleep(500/MARSHMALLOW_ENGINE_FRAMERATE);

	Painter::Reset();
}

const Graphics::Display &
Backend::Display(void)
{
	return(Dummy::s_dpy);
}

const Math::Size2f &
Backend::Size(void)
{
	const static Math::Size2f s_size(MARSHMALLOW_GRAPHICS_WIDTH,
	                                 MARSHMALLOW_GRAPHICS_HEIGHT);
	return(s_size);
}

const Math::Size2i &
Backend::WindowSize(void)
{
	using namespace Dummy;

	static Math::Size2i s_size;
	s_size.set(s_dpy.width, s_dpy.height);
	return(s_size);
}

const Core::Identifier &
Backend::Implementation(void)
{
	static const Core::Identifier s_implementation("DUMMY");
	return(s_implementation);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

