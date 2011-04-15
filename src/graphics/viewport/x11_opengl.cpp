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

#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>
#include <X11/extensions/Xrandr.h>
#include <X11/keysymdef.h>

#include "core/platform.h"


MARSHMALLOW_NAMESPACE_USE;
using namespace Graphics;

struct Viewport::Internal
{
	Display   *display;
	GLXContext context;
	Window     window;
} MPI;

bool
Viewport::Initialize(int w, int h, int d, bool f)
{
	UNUSED(f); // TODO: Implement Fullscreen

	Colormap cmap;
	XSetWindowAttributes swattr;
	XVisualInfo *vinfo;

	if (!(MPI.display = XOpenDisplay(0))) {
		ERROR1("Unable to open X Display.");
		return(false);
	}

	GLint gattr[] = {GLX_RGBA, GLX_DEPTH_SIZE, d, GLX_DOUBLEBUFFER, None};
	if(!(vinfo = glXChooseVisual(MPI.display, 0, gattr))) {
		ERROR1("Unable to choose X Visual Info.");
		return(false);
	}

	Window rootwnd = RootWindow(MPI.display, vinfo->screen);
	cmap = XCreateColormap(MPI.display, rootwnd, vinfo->visual, AllocNone);
	swattr.colormap = cmap;
	swattr.background_pixel = BlackPixel(MPI.display, vinfo->screen);
	swattr.border_pixel = BlackPixel(MPI.display, vinfo->screen);
	swattr.event_mask = ButtonPressMask|ButtonReleaseMask|ExposureMask|KeyPressMask|KeyReleaseMask|PointerMotionMask;

	MPI.window = XCreateWindow(MPI.display,
	                           rootwnd,
	                           0, 0, w, h,
	                           1,
	                           d,
	                           InputOutput,
	                           vinfo->visual,
	                           CWBackPixel|CWBorderPixel|CWColormap|CWEventMask,
	                           &swattr);

	XMapWindow(MPI.display, MPI.window);
	XStoreName(MPI.display, MPI.window, "Marshmallow"); // TODO: Set window caption

	if (!(MPI.context = glXCreateContext(MPI.display, vinfo, 0, GL_TRUE))) {
		ERROR1("Failed to create context!");
		return(false);
	}

	if (!glXMakeCurrent(MPI.display, MPI.window, MPI.context)) {
		WARNING1("Failed to make context current!");
		return(false);
	}
	glEnable(GL_DEPTH_TEST);

	glClearColor(.0, .0, .0, .0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	SwapBuffer();

	return(true);
}

void
Viewport::Finalize(void)
{
	glXMakeCurrent(MPI.display, None, 0);
	glXDestroyContext(MPI.display, MPI.context);
	XDestroyWindow(MPI.display, MPI.window);
	XCloseDisplay(MPI.display);
}

void
Viewport::Tick(TIME &t)
{
	TIMEOUT_INIT;
	XEvent e;

	while(TIMEOUT_DEC(t) > 0 && XPending(MPI.display)) {
		XNextEvent(MPI.display, &e);

		switch(e.type) {
		case Expose: {
			XExposeEvent &expose = e.xexpose;
			glViewport(0, 0, expose.width, expose.height);
			SwapBuffer();
			} break;
		case ButtonPress:
		case ButtonRelease:
		case KeyPress:
		case KeyRelease:
		case MotionNotify:
			/* TODO: Send Events */
			break;
		default: INFO1("Unknown viewport event received."); break;
		}
	}
}

void
Viewport::SwapBuffer(void)
{
	glXSwapBuffers(MPI.display, MPI.window);
}

