/*
 * Copyright 2012 Marshmallow Engine. All rights reserved.
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

#include "event/eventmanager.h"
#include "event/keyboardevent.h"
#include "event/quitevent.h"
#include "event/viewportevent.h"

#ifdef MARSHMALLOW_INPUT_MISC_SDL
#  include "input/misc/sdl.h"
#endif

#include "graphics/camera.h"
#include "graphics/color.h"
#include "graphics/display.h"
#include "graphics/painter_p.h"

#include <SDL.h>

#include "headers.h"
#include "extensions.h"

/*
 * SDL Viewport Notes
 *
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */
namespace OpenGL { /****************************** Graphics::OpenGL Namespace */
namespace { /************************ Graphics::OpenGL::<anonymous> Namespace */

namespace SDLViewport {

	class Widget;

	inline bool Initialize(void);
	inline void Finalize(void);

	inline void Reset(int state);
	inline void SwapBuffer(void);

	inline bool Create(const Display &display);
	inline void Destroy(void);
	inline void ProcessEvents(void);

	enum StateFlag
	{
		sfUninitialized = 0,
		sfReady         = (1 << 0),

		sfSDLInit       = (1 << 1),
		sfSDLSurface    = (1 << 2),

		sfExposed       = (1 << 3),

		sfFocused       = (1 << 4),
		sfKeyboardFocus = (1 << 5),
		sfMouseFocus    = (1 << 6),

		sfTerminated    = (1 << 7),
		sfValid         = sfSDLInit|sfSDLSurface,
		sfActive        = sfReady|sfValid|sfExposed|sfFocused
	};
	/******************* MARSHMALLOW */
	Display           dpy;
	Math::Size2i      wsize;
	Math::Size2f      vsize;
	int               flags;

	/*************************** SDL */
	SDL_Surface      *sdl_surface;

}

bool
SDLViewport::Initialize(void)
{
	/* default display display */
	dpy.depth      = MARSHMALLOW_VIEWPORT_DEPTH;
	dpy.fullscreen = MARSHMALLOW_VIEWPORT_FULLSCREEN;
	dpy.height     = MARSHMALLOW_VIEWPORT_HEIGHT;
	dpy.vsync      = MARSHMALLOW_VIEWPORT_VSYNC;
	dpy.width      = MARSHMALLOW_VIEWPORT_WIDTH;

	Reset(sfUninitialized);

	/*
	 * Initial Camera Reset (IMPORTANT)
	 */
	Camera::Reset();

	/*
	 * Initial Background Color (IMPORTANT)
	 */
	Painter::SetBackgroundColor(Color::Black());

	return(true);
}

void
SDLViewport::Finalize(void)
{
	/* set termination flag */
	flags |= sfTerminated;

	if (sfValid == (flags & sfValid))
		Destroy();

	/* turn off SDL if last subsystem */
	if (0 == SDL_WasInit(SDL_INIT_EVERYTHING))
		SDL_Quit();

	/* sanity check */
	assert(flags == sfTerminated && "We seem to have some stray flags!");

	flags = sfUninitialized;
}

void
SDLViewport::Reset(int state)
{
	flags = state;
	vsize.zero();
	wsize.zero();

	if (state == sfUninitialized)
		sdl_surface = 0;

	/* sanity check */
	else {
		assert(0 == sdl_surface
		    && "SDL display didn't get cleanly destroyed!");
	}
}

bool
SDLViewport::Create(const Display &display)
{
	/*
	 * Initialize SDL video subsystem
	 */
	SDL_putenv(const_cast<char *>("SDL_VIDEO_CENTERED=1"));
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
		MMERROR("SDL: Viewport initialization failed.");
		return(false);
	}
	flags |= sfSDLInit;

	/*
	 * Check if already valid (no no)
	 */
	if (sfValid == (flags & sfValid))
		return(false);

	/* assign new display display */
	dpy = display;

	/*
	 * Create SDL Display
	 */
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,  dpy.depth);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     0);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   0);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    0);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,   0);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,   0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#if SDL_VERSION_ATLEAST(1,3,0)
	if (!SDL_GL_SetSwapInterval(dpy.vsync ? 1 : 0))
		MMERROR("SDL: Swap interval request was ignored! "
		        "SDLERROR=" << SDL_GetError());
#else
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, (dpy.vsync ? 1 : 0));
#endif
	
	sdl_surface =
	    SDL_SetVideoMode(dpy.fullscreen ? 0 : dpy.width,
	                     dpy.fullscreen ? 0 : dpy.height,
	                     dpy.depth,
	                     SDL_HWSURFACE
	                         | SDL_GL_DOUBLEBUFFER
	                         | SDL_OPENGL
	                         | (dpy.fullscreen ? SDL_FULLSCREEN : 0));

	if (!sdl_surface) {
		MMERROR("SDL: Failed to create an valid SDL display surface. "
		    "SDLERROR=" << SDL_GetError());
		return(false);
	}
	else flags |= sfSDLSurface;

	SDL_WM_SetCaption(MARSHMALLOW_VIEWPORT_TITLE, MARSHMALLOW_VIEWPORT_TITLE);
	wsize.set(sdl_surface->w, sdl_surface->h);

	/* move cursor */

	SDL_WarpMouse(static_cast<Uint16>(sdl_surface->w),
	              static_cast<Uint16>(sdl_surface->h));

	/* set initial state (SDL doesn't trigger ACTIVE events on start) */
	flags |= sfExposed|sfFocused|sfKeyboardFocus|sfMouseFocus;

	/* sanity check */
	assert(sfValid == (flags & sfValid)
	    && "Valid viewport was expected!");

	/* extensions */

	OpenGL::Extensions::Initialize();

	/* initialize context */

	glViewport(0, 0, wsize.width, wsize.height);

	if (glGetError() != GL_NO_ERROR) {
		MMERROR("GL: Failed during initialization.");
		return(false);
	}

	/* viewport size */

	if (dpy.fullscreen) {
#if MARSHMALLOW_VIEWPORT_LOCK_WIDTH
		vsize.width = static_cast<float>(dpy.width);
		vsize.height = (vsize.width * static_cast<float>(wsize.height)) /
		    static_cast<float>(wsize.width);
#else
		vsize.height = static_cast<float>(dpy.height);
		vsize.width = (vsize.height * static_cast<float>(wsize.width)) /
		    static_cast<float>(wsize.height);
#endif
	}
	else vsize.set(dpy.width, dpy.height);

	/* sub-systems */

	Camera::Update();

	Painter::Initialize();

	/* activate */

	flags |= sfReady;

	/* broadcast */

	Event::ViewportEvent l_event(Event::ViewportEvent::Created);
	Event::EventManager::Instance()->dispatch(l_event);

	return(true);
}

void
SDLViewport::Destroy(void)
{
	/* check for valid state */
	if (sfValid != (flags & sfValid))
		return;

	/* deactivate */

	flags &= ~(sfReady);

	/* broadcast */

	if (0 == (flags & sfTerminated)) {
		Event::ViewportEvent l_event(Event::ViewportEvent::Destroyed);
		Event::EventManager::Instance()->dispatch(l_event);
	}

	Painter::Finalize();

	flags &= ~(sfSDLSurface|sfExposed|sfFocused|sfKeyboardFocus|sfMouseFocus);
	sdl_surface = 0;

	/*
	 * Quit SDL video subsystem
	 */
	if (sfSDLInit == (flags & sfSDLInit)) {
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		flags ^= sfSDLInit;
	}

	/* sanity check */
	assert(0 == (flags & ~(sfTerminated))
	    && "We seem to have some stray flags!");

	Reset(flags & (sfTerminated));
}

void
SDLViewport::SwapBuffer(void)
{
	assert(sfValid == (flags & sfValid)
	    && "Attempted to swap buffer with an invalid viewport");
	SDL_GL_SwapBuffers();
}

void
SDLViewport::ProcessEvents(void)
{
	SDL_Event e;

	while(SDL_PollEvent(&e)) {
		switch(e.type) {
		case SDL_QUIT: {
			Event::QuitEvent l_event(-1);
			Event::EventManager::Instance()->dispatch(l_event);
		} break;

		case SDL_ACTIVEEVENT:
			switch (e.active.state) {
			case SDL_APPACTIVE:
				if (0 == e.active.gain)
					flags &= ~(sfExposed);
				else
					flags |= sfExposed;
				break;
			case SDL_APPINPUTFOCUS:
				if (0 == e.active.gain)
					flags &= ~(sfKeyboardFocus);
				else
					flags |= sfKeyboardFocus;
				break;
			case SDL_APPMOUSEFOCUS:
				if (0 == e.active.gain)
					flags &= ~(sfMouseFocus);
				else
					flags |= sfMouseFocus;
				break;
			}

			if (0 == (flags & (sfKeyboardFocus|sfMouseFocus)))
				flags &= ~(sfFocused);
			else
				flags |= sfFocused;
			
		break;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
#ifdef MARSHMALLOW_INPUT_MISC_SDL
			Input::Misc::SDL::HandleKeyboardEvent(e.key);
#endif
			break;

		default: MMINFO("Unknown viewport event received."); break;
		}
	}
}

} /********************************** Graphics::OpenGL::<anonymous> Namespace */

PFNPROC
glGetProcAddress(const char *f)
{
	union {
		PFNPROC fptr;
		void    *ptr;
	} conv;

	conv.ptr = SDL_GL_GetProcAddress(f);
	return(conv.fptr);
}

} /*********************************************** Graphics::OpenGL Namespace */

bool
Viewport::Active(void)
{
	using namespace OpenGL::SDLViewport;
	return(sfActive == (flags & sfActive));
}

bool
Viewport::Initialize(void)
{
	using namespace OpenGL;
	return(SDLViewport::Initialize());
}

void
Viewport::Finalize(void)
{
	using namespace OpenGL;
	SDLViewport::Finalize();
}

bool
Viewport::Setup(const Graphics::Display &display)
{
	using namespace OpenGL;
	SDLViewport::Destroy();

	if (!SDLViewport::Create(display)) {
		SDLViewport::Destroy();
		return(false);
	}

	return(true);
}

void
Viewport::Tick(float)
{
	using namespace OpenGL;
	SDLViewport::ProcessEvents();
}

void
Viewport::SwapBuffer(void)
{
	using namespace OpenGL;
	SDLViewport::SwapBuffer();
	Painter::Reset();
}

const Graphics::Display &
Viewport::Display(void)
{
	using namespace OpenGL;
	return(SDLViewport::dpy);
}

const Math::Size2f &
Viewport::Size(void)
{
	using namespace OpenGL;
	return(SDLViewport::vsize);
}

const Math::Size2i &
Viewport::WindowSize(void)
{
	using namespace OpenGL;
	return(SDLViewport::wsize);
}

const Core::Type &
Viewport::Type(void)
{
	static const Core::Type s_type("SDL");
	return(s_type);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

