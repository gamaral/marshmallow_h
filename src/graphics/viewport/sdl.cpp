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

#include "core/platform.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Graphics;

struct Viewport::Internal
{
	    SDL_Surface *screen;
	    SDL_Event event;
} MPI;

bool
Viewport::Initialize(int w, int h, int d, bool f)
{
	SDL_Init(SDL_INIT_VIDEO);

	MPI.screen = SDL_SetVideoMode(w, h, d, SDL_DOUBLEBUF|SDL_HWSURFACE|(f ? SDL_FULLSCREEN:0));
	if (!MPI.screen) {
		ERROR("SDL Error: %s", SDL_GetError());
		return(false);
	}
	SDL_WM_SetCaption("Marshmallow", "Marshmallow"); // TODO: Set window caption
	SDL_FillRect(MPI.screen, &MPI.screen->clip_rect, SDL_MapRGB(MPI.screen->format, 0, 0, 0));
	SwapBuffer();

	return(true);
}

void
Viewport::Finalize(void)
{
	SDL_Quit();
}

void
Viewport::Tick(TIME &t)
{
	TIMEOUT_INIT;

	SDL_Event e;
	while(TIMEOUT_DEC(t) > 0 && SDL_PollEvent(&e))
		switch(e.type) {
		case SDL_QUIT:
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
	SDL_Flip(MPI.screen);
}

