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
#include <X11/extensions/Xrandr.h>
#include <X11/keysymdef.h>

#include "core/platform.h"
#include "event/eventmanager.h"
#include "event/keyboardevent.h"
#include "event/quitevent.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Graphics;

const char *Viewport::Name("X11_GLX");

struct Viewport::Internal
{
	Display    *display;
	GLXContext  context;
	Window      window;
	Atom        wm_delete;
	XSizeHints *size_hints;
	bool        loaded;
} MPI;

bool
Viewport::Initialize(int w, int h, int d, bool f)
{
	MPI.display = 0;
	MPI.size_hints = 0;
	MPI.loaded = false;
	return(Redisplay(w, h, d, f));
}

void
Viewport::Finalize(void)
{
	if (MPI.display) {
		glXMakeCurrent(MPI.display, None, 0);
		glXDestroyContext(MPI.display, MPI.context);
		XDestroyWindow(MPI.display, MPI.window);
		XCloseDisplay(MPI.display);
	}
	if (MPI.size_hints)
		XFree(MPI.size_hints);
}

bool
Viewport::Redisplay(int w, int h, int d, bool f)
{
	Colormap cmap;
	XSetWindowAttributes swattr;
	XVisualInfo *vinfo;
	XWindowAttributes rwattrs;

	if (MPI.loaded) {
		glXMakeCurrent(MPI.display, None, 0);
		glXDestroyContext(MPI.display, MPI.context);
		XDestroyWindow(MPI.display, MPI.window);
		XCloseDisplay(MPI.display);
	}

	MPI.loaded = false;

	/* open display */
	if (!(MPI.display = XOpenDisplay(0))) {
		ERROR1("Unable to open X Display.");
		return(false);
	}

	/* get visual info */
	GLint gattr[] = {GLX_RGBA, GLX_DEPTH_SIZE, d, GLX_DOUBLEBUFFER, None};
	if(!(vinfo = glXChooseVisual(MPI.display, 0, gattr))) {
		ERROR1("Unable to choose X Visual Info.");
		return(false);
	}

	/* get root window */
	Window rwindow = RootWindow(MPI.display, vinfo->screen);
	XGetWindowAttributes(MPI.display, rwindow, &rwattrs);

	/* create colormap */
	cmap = XCreateColormap(MPI.display, rwindow, vinfo->visual, AllocNone);

	/* create window */
	swattr.colormap = cmap;
	swattr.background_pixel = BlackPixel(MPI.display, vinfo->screen);
	swattr.border_pixel = BlackPixel(MPI.display, vinfo->screen);
	swattr.event_mask =
		ButtonPressMask |
		ButtonReleaseMask |
		ExposureMask |
		KeyPressMask |
		KeyReleaseMask |
		PointerMotionMask |
		StructureNotifyMask;
	MPI.window = XCreateWindow(MPI.display,
	                           rwindow,
	                           (DisplayWidth(MPI.display, vinfo->screen) - w) / 2,
	                           (DisplayHeight(MPI.display, vinfo->screen) - h) / 2,
	                           w, h,
	                           1,
	                           d,
	                           InputOutput,
	                           vinfo->visual,
	                           CWBackPixel|CWBorderPixel|CWColormap|CWEventMask,
	                           &swattr);

	XMapWindow(MPI.display, MPI.window);
	XStoreName(MPI.display, MPI.window, "Marshmallow"); // TODO: Set window caption

	/* catch window manager delete event */
	MPI.wm_delete = XInternAtom(MPI.display, "WM_DELETE_WINDOW", false);
	XSetWMProtocols(MPI.display, MPI.window, &MPI.wm_delete, 1);

	/* set size hints */
	if (f) {
		MPI.size_hints = 0;
	} else {
		if(!(MPI.size_hints = XAllocSizeHints())) {
			ERROR1("Unable to allocate window size hints.");
			return(false);
		}
		MPI.size_hints->flags = PMinSize|PMaxSize;
		MPI.size_hints->min_width = w;
		MPI.size_hints->min_height = h;
		MPI.size_hints->max_width = w;
		MPI.size_hints->max_height = h;
		XSetWMNormalHints(MPI.display, MPI.window, MPI.size_hints);
	}

	/* create context */
	if (!(MPI.context = glXCreateContext(MPI.display, vinfo, 0, GL_TRUE))) {
		ERROR1("Failed to create context!");
		return(false);
	}
	if (!glXMakeCurrent(MPI.display, MPI.window, MPI.context)) {
		WARNING1("Failed to make context current!");
		return(false);
	}
	glEnable(GL_DEPTH_TEST);

	/* initialize context */
	glViewport(0, 0, w, h);
	glClearColor(.0, .0, .0, .0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	SwapBuffer();

	return(MPI.loaded = true);
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
			glClearColor(.0, .0, .0, .0);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			SwapBuffer();
			} break;
		case ClientMessage: {
			XClientMessageEvent &client = e.xclient;
			if (static_cast<Atom>(client.data.l[0]) == MPI.wm_delete) {
				Event::QuitEvent event;
				Event::EventManager::Instance()->dispatch(event);
			}
		} break;
		case ButtonPress:
		case ButtonRelease:
		case KeyPress:
		case KeyRelease: {
			XKeyEvent &key = e.xkey;
			UINT16 l_key = 0;
			Event::KBAction l_action =
			    (key.type == KeyPress ? Event::KeyPressed : Event::KeyReleased);
			Event::KBModifiers l_modifiers = Event::NoModifiers;
			
			Event::SharedEvent event(new Event::KeyboardEvent(l_key, l_action, l_modifiers));
			Event::EventManager::Instance()->queue(event);
		} break;
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

