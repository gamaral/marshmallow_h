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

#include "core/platform.h"
#include "event/eventmanager.h"
#include "event/keyboardevent.h"
#include "event/quitevent.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Graphics;

const char *Viewport::Name("X11_GLX");

struct Viewport::Internal
{
	GLXContext  context;
	Window      window;
	Atom        wm_delete;
	Display    *display;
	XSizeHints *size_hints;
	bool        loaded;

	Internal(void)
	    : display(0),
	      size_hints(0),
	      loaded(false)
	{
	}

	bool
	createXWindow(int w, int h, int d, bool f)
	{
		loaded = false;

		/* open display */
		if (!(display = XOpenDisplay(0))) {
			ERROR1("Unable to open X Display.");
			return(false);
		}

		/* get visual info */
		GLint gattr[] = {GLX_RGBA, GLX_DEPTH_SIZE, d, GLX_DOUBLEBUFFER, None};
		XVisualInfo *l_vinfo;
		if(!(l_vinfo = glXChooseVisual(display, 0, gattr))) {
			ERROR1("Unable to choose X Visual Info.");
			return(false);
		}

		/* get root window */
		Window rwindow = RootWindow(display, l_vinfo->screen);
		
		/* create window */
		XSetWindowAttributes l_swattr;
		l_swattr.colormap = XCreateColormap(display, rwindow, l_vinfo->visual, AllocNone);
		l_swattr.background_pixel = BlackPixel(display, l_vinfo->screen);
		l_swattr.border_pixel = BlackPixel(display, l_vinfo->screen);
		l_swattr.event_mask =
			ButtonPressMask |
			ButtonReleaseMask |
			ExposureMask |
			KeyPressMask |
			KeyReleaseMask |
			PointerMotionMask |
			StructureNotifyMask;
		window = XCreateWindow(display,
					   rwindow,
					   (DisplayWidth(display, l_vinfo->screen) - w) / 2,
					   (DisplayHeight(display, l_vinfo->screen) - h) / 2,
					   w, h,
					   1,
					   d,
					   InputOutput,
					   l_vinfo->visual,
					   CWBackPixel|CWBorderPixel|CWColormap|CWEventMask,
					   &l_swattr);

		XMapWindow(display, window);
		XStoreName(display, window, "Marshmallow"); // TODO: Set window caption

		/* catch window manager delete event */
		wm_delete = XInternAtom(display, "WM_DELETE_WINDOW", false);
		XSetWMProtocols(display, window, &wm_delete, 1);

		/* set size hints */
		if (f) {
			size_hints = 0;
		} else {
			if(!(size_hints = XAllocSizeHints())) {
				ERROR1("Unable to allocate window size hints.");
				return(false);
			}
			size_hints->flags = PMinSize|PMaxSize;
			size_hints->min_width = w;
			size_hints->min_height = h;
			size_hints->max_width = w;
			size_hints->max_height = h;
			XSetWMNormalHints(display, window, size_hints);
		}

		/* create context */
		if (!(context = glXCreateContext(display, l_vinfo, 0, GL_TRUE))) {
			ERROR1("Failed to create context!");
			return(false);
		}
		if (!glXMakeCurrent(display, window, context)) {
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

		return(loaded = true);
	}

	void
	cleanupXWindow(void)
	{
		if (display) {
			glXMakeCurrent(display, None, 0);
			glXDestroyContext(display, context);
			XDestroyWindow(display, window);
			XCloseDisplay(display);
		}
		if (size_hints)
			XFree(size_hints);

		display = 0;
		size_hints = 0;
		loaded = false;
	}
} MPI;

bool
Viewport::Initialize(int w, int h, int d, bool f)
{
	return(MPI.createXWindow(w, h, d, f));
}

void
Viewport::Finalize(void)
{
	MPI.cleanupXWindow();
}

bool
Viewport::Redisplay(int w, int h, int d, bool f)
{
	MPI.cleanupXWindow();
	return(MPI.createXWindow(w, h, d, f));
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
			/* TODO: Send Events */
		break;
		case KeyPress:
		case KeyRelease: {
			XKeyEvent &key = e.xkey;
			UINT16 l_key = 0; // TODO: create keymap and map virtual keys
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

