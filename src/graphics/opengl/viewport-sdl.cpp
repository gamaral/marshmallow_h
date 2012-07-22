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

#include "graphics/viewport.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include "event/eventmanager.h"
#include "event/keyboardevent.h"
#include "event/quitevent.h"

#include "graphics/camera.h"
#include "graphics/painter.h"

#include <SDL.h>

#include "headers.h"
#include "extensions.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace { /******************************************** Anonymous Namespace */

struct ViewportData
{
	SDL_Surface        *display;
	Math::Size2i        wsize;
	int                 screen;
	Math::Size2f        size;
	bool                fullscreen;
	bool                loaded;
} s_data;

void
ResetViewportData(void)
{
	s_data.display = 0;
	s_data.fullscreen = false;
	s_data.size.zero();
	s_data.wsize[0] = s_data.wsize[1] = 0;

	s_data.loaded = false;
}

bool
CreateWindow(int w, int h, int depth, bool fullscreen, bool vsync)
{
	using namespace Graphics;

	ResetViewportData();

	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,  depth);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     0);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   0);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    0);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,   0);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,   0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#if SDL_VERSION_ATLEAST(1,3,0)
	if (!SDL_GL_SetSwapInterval(vsync ? 1 : 0))
		MMERROR("SDL: Failed to set swap interval. "
		        "SDLERROR=" << SDL_GetError());
#else
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, (vsync ? 1 : 0));
#endif
	
	s_data.display =
	    SDL_SetVideoMode(fullscreen ? 0 : w, fullscreen ? 0 : h, depth,
	                     SDL_HWSURFACE |
	                     SDL_GL_DOUBLEBUFFER |
	                     SDL_OPENGL |
	                     (fullscreen? SDL_FULLSCREEN : 0));

	if (!s_data.display) {
		MMERROR("Failed to create an SDL surface.");
		return(false);
	}

	SDL_WM_SetCaption(MARSHMALLOW_BUILD_TITLE, MARSHMALLOW_BUILD_TITLE);

	s_data.fullscreen = fullscreen;
	s_data.wsize[0] = s_data.display->w;
	s_data.wsize[1] = s_data.display->h;

	/* extensions */

	OpenGL::InitializeExtensions();

	/* initialize context */

	glViewport(0, 0, s_data.wsize[0], s_data.wsize[1]);

	if (glGetError() != GL_NO_ERROR) {
		MMERROR("GL failed during initialization.");
		return(false);
	}

	/* set viewport size */

#if MARSHMALLOW_VIEWPORT_LOCK_WIDTH
	s_data.size[0] = MARSHMALLOW_VIEWPORT_WIDTH;
	s_data.size[1] = static_cast<float>(s_data.wsize[1]) *
	    (MARSHMALLOW_VIEWPORT_WIDTH / static_cast<float>(s_data.wsize[0]));
#else
	s_data.size[0] = static_cast<float>(s_data.wsize[0]) *
	    (MARSHMALLOW_VIEWPORT_HEIGHT / static_cast<float>(s_data.wsize[1]));
	s_data.size[1] = MARSHMALLOW_VIEWPORT_HEIGHT;
#endif

	Camera::Update();

	return(s_data.loaded = true);
}

void
DestroyWindow(void)
{
	s_data.display = 0;
	s_data.loaded  = false;
}

void
HandleKeyEvent(SDL_KeyboardEvent &key)
{
	typedef std::list<Event::KBKeys> KeyList;
	static KeyList s_keys_pressed;

	Event::KBKeys l_key = Event::KEY_NONE;
	Event::KBActions l_action =
	    (key.state == SDL_PRESSED ? Event::KeyPressed : Event::KeyReleased);

	l_key = static_cast<Event::KBKeys>(key.keysym.sym);

	bool l_key_pressed = false;
	KeyList::const_iterator l_pressed_key_i;
	for (l_pressed_key_i = s_keys_pressed.begin();
	     l_pressed_key_i != s_keys_pressed.end();
	     ++l_pressed_key_i)
		if (*l_pressed_key_i == l_key) {
			l_key_pressed = true;
			break;
		}
	
	if (( l_key_pressed && l_action != Event::KeyPressed)
	 || (!l_key_pressed && l_action == Event::KeyPressed)) {
		Event::SharedEvent event(new Event::KeyboardEvent(l_key, l_action));
		Event::EventManager::Instance()->queue(event);

		if (l_key_pressed) s_keys_pressed.remove(l_key);
		else s_keys_pressed.push_front(l_key);
	}
}

} /****************************************************** Anonymous Namespace */

namespace Graphics { /************************************ Graphics Namespace */

OpenGL::PFNPROC
OpenGL::glGetProcAddress(const char *f)
{
	union {
		PFNPROC fptr;
		void    *ptr;
	} conv;

	conv.ptr = SDL_GL_GetProcAddress(f);
	return(conv.fptr);
}

/********************************************************* Graphics::Viewport */

bool
Viewport::Initialize(uint16_t w, uint16_t h, uint8_t depth, uint8_t,
                     bool fullscreen, bool vsync)
{
	/* force video center */
	SDL_putenv(const_cast<char *>("SDL_VIDEO_CENTERED=1"));

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		MMERROR("SDL viewport initialization failed.");
		return(false);
	}

	Camera::Reset();

	if (!CreateWindow(w, h, depth, fullscreen, vsync)) {
		DestroyWindow();
		return(false);
	}

	Painter::Initialize();
	return(true);
}

void
Viewport::Finalize(void)
{
	Painter::Finalize();

	DestroyWindow();

	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

bool
Viewport::Redisplay(uint16_t w, uint16_t h, uint8_t depth, uint8_t,
                    bool fullscreen, bool vsync)
{
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		MMERROR("SDL viewport initialization failed.");
		return(false);
	}

	DestroyWindow();

	if (!CreateWindow(w, h, depth, fullscreen, vsync)) {
		DestroyWindow();
		return(false);
	}
	return(true);
}

void
Viewport::Tick(void)
{
	SDL_Event e;

	while(SDL_PollEvent(&e)) {
		switch(e.type) {
		case SDL_QUIT: {
			Event::QuitEvent l_event(-1);
			Event::EventManager::Instance()->dispatch(l_event);
		} break;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			HandleKeyEvent(e.key);
			break;

		default: MMINFO("Unknown viewport event received."); break;
		}
	}
}

void
Viewport::SwapBuffer(void)
{
	SDL_GL_SwapBuffers();
	Painter::Reset();
}

const Math::Size2f &
Viewport::Size(void)
{
	return(s_data.size);
}

const Math::Size2i &
Viewport::WindowSize(void)
{
	return(s_data.wsize);
}

const Core::Type &
Viewport::Type(void)
{
	static const Core::Type s_type("SDL");
	return(s_type);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

