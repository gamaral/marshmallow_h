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

#include "graphics/painter.h"
#include "graphics/transform.h"

#include "headers.h"
#include "extensions.h"

#include <SDL.h>

#include <cmath>
#include <list>

MARSHMALLOW_NAMESPACE_USE
using namespace Graphics::OpenGL;
using namespace Graphics;

/******************************************************************************/

namespace
{
	struct ViewportData {
		SDL_Surface  *display;
		Math::Size2i  wsize;
		int           screen;
		Transform     camera;
		float         radius2;
		Math::Size2f  size;
		Math::Size2f  scaled_size;
		bool          fullscreen;
		bool          loaded;
	} s_data;

	void UpdateCamera(void);

	void InitializeViewport(void)
	{
		s_data.camera.setRotation(.0f);
		s_data.camera.setScale(Math::Size2f::Identity());
		s_data.camera.setTranslation(Math::Point2::Zero());

		s_data.display = 0;
		s_data.fullscreen = false;
		s_data.loaded = false;
		s_data.size.zero();
		s_data.wsize[0] = s_data.wsize[1] = 0;
	}

	bool
	CreateWindow(int w, int h, int d, bool f, bool v)
	{
		s_data.loaded  = false;

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); 

#if SDL_VERSION_ATLEAST(1,3,0)
		if (!SDL_GL_SetSwapInterval(v ? 1 : 0))
			MMERROR("SDL: Failed to set swap interval. SDLERROR=" << SDL_GetError());
#else
		SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, (v ? 1 : 0));
#endif
		s_data.display = SDL_SetVideoMode(f ? 0 : w, f ? 0 : h, d,
		                                  SDL_HWSURFACE |
		                                  SDL_GL_DOUBLEBUFFER |
		                                  SDL_OPENGL |
		                                  (f? SDL_FULLSCREEN : 0));

		if (!s_data.display) {
			MMERROR("Failed to create an SDL surface.");
			return(false);
		}

		SDL_WM_SetCaption(MARSHMALLOW_BUILD_TITLE, MARSHMALLOW_BUILD_TITLE);

		s_data.fullscreen = f;
		s_data.wsize[0] = s_data.display->w;
		s_data.wsize[1] = s_data.display->h;

		/* extensions */

		InitializeExtensions();

		/* initialize context */

		glViewport(0, 0, s_data.wsize[0], s_data.wsize[1]);

		if(glGetError() != GL_NO_ERROR) {
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

		Viewport::SetCamera(s_data.camera);

		return(s_data.loaded = true);
	}

	void
	DestroyWindow(void)
	{
		s_data.display = 0;
		s_data.loaded  = false;
	}

	void
	UpdateCamera(void)
	{
		/* calculate scaled viewport size */
		s_data.scaled_size[0] = s_data.size[0] / s_data.camera.scale().width();
		s_data.scaled_size[1] = s_data.size[1] / s_data.camera.scale().height();

		/* calculate magnitude and pass it off as radius squared */
		s_data.radius2 = powf(s_data.scaled_size[0] / 2.f, 2.f) +
		                 powf(s_data.scaled_size[1] / 2.f, 2.f);
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
} // namespace

/***************************************************************** Extensions */

PFNPROC
OpenGL::glGetProcAddress(const char *f)
{
	union {
		PFNPROC fptr;
		void *ptr;
	} conv;

	conv.ptr = SDL_GL_GetProcAddress(f);
	return(conv.fptr);
}

/******************************************************************************/

bool
Viewport::Initialize(uint16_t w, uint16_t h, uint8_t d, uint8_t r, bool f, bool v)
{
	MMUNUSED(r);

	/* force video center */
	SDL_putenv(const_cast<char *>("SDL_VIDEO_CENTERED=1"));

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		MMERROR("SDL viewport initialization failed.");
		return(false);
	}

	InitializeViewport();

	if (!CreateWindow(w, h, d, f, v)) {
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
Viewport::Redisplay(uint16_t w, uint16_t h, uint8_t d, uint8_t r, bool f, bool v)
{
	MMUNUSED(r);

	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		MMERROR("SDL viewport initialization failed.");
		return(false);
	}

	DestroyWindow();

	if(!CreateWindow(w, h, d, f, v)) {
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

const Graphics::Transform &
Viewport::Camera(void)
{
	return(s_data.camera);
}

void
Viewport::SetCamera(const Graphics::Transform &camera)
{
	s_data.camera = camera;
	UpdateCamera();
}

float
Viewport::Radius2(void)
{
	return(s_data.radius2);
}

const Math::Size2f &
Viewport::ScaledSize(void)
{
	return(s_data.scaled_size);
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

