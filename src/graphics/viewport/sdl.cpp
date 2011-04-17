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

#include <SDL.h>
#include <SDL_opengl.h>

#include "core/platform.h"
#include "event/eventmanager.h"
#include "event/quitevent.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Graphics;

const char *Viewport::Name("SDL");

struct Viewport::Internal
{
	SDL_Surface *screen;
	float       world[4];

	Internal(void)
	: screen(0)
	{}

	bool
	createSDLWindow(int w, int h, int d, bool f)
	{
		int l_flags =
		    SDL_HWSURFACE |
		    SDL_DOUBLEBUF |
		    SDL_OPENGL |
		    (f ? SDL_FULLSCREEN : 0);

		screen = SDL_SetVideoMode(w, h, d, l_flags);
		if (!screen) {
			ERROR("SDL Error: %s", SDL_GetError());
			return(false);
		}

		/* set world coordinates */
		float l_aratio = static_cast<float>(h) / static_cast<float>(w);
		world[0] = -100.f;
		world[1] =  100.f;
		world[2] = world[0] * l_aratio;
		world[3] = world[1] * l_aratio;

		/* initialize context */
		glViewport(0, 0, w, h);
		glClearColor(0., 0., 0., 0.);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(world[0], world[1], world[2], world[3], -1.f, 1.f);
		glMatrixMode(GL_MODELVIEW);
		SwapBuffer();

		if( glGetError() != GL_NO_ERROR )
			return(false);

		return(true);
	}

} MVI;

bool
Viewport::Initialize(int w, int h, int d, bool f)
{
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		ERROR1("Failed to initialize SDL");
		return(false);
	}

	return(MVI.createSDLWindow(w, h, d, f));
}

void
Viewport::Finalize(void)
{
	SDL_Quit();
}

bool
Viewport::Redisplay(int w, int h, int d, bool f)
{
	return(MVI.createSDLWindow(w, h, d, f));
}

void
Viewport::Tick(TIME t)
{
	TIMEOUT_INIT;

	SDL_Event e;
	while(TIMEOUT_DEC(t) > 0 && SDL_PollEvent(&e))
		switch(e.type) {
		case SDL_QUIT: {
			Event::QuitEvent event(-1);
			Event::EventManager::Instance()->dispatch(event);
		} break;
		case SDL_KEYUP:
		case SDL_KEYDOWN:
		case SDL_MOUSEMOTION:
			/* TODO: Send Events */
		break;
		default: INFO1("Unknown viewport event received."); break;
		}
}

void
Viewport::SwapBuffer(void)
{
	SDL_GL_SwapBuffers();
	glClearColor(.0, .0, .0, .0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void
Viewport::World(float &lx, float &hx, float &ly, float &hy)
{
	lx = MVI.world[0];
	hx = MVI.world[1];
	ly = MVI.world[2];
	hy = MVI.world[3];
}

