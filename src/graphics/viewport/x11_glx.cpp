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
#include "graphics/painter.h"

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
	{}

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

		//glCullFace(GL_BACK);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);

		/* initialize context */
		glViewport(0, 0, w, h);
		glClearColor(0., 0., 0., 0.);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, w, h, 0, -1.f, 1.f);
		glMatrixMode(GL_MODELVIEW);
		SwapBuffer();

		if( glGetError() != GL_NO_ERROR )
			return(false);

		return(loaded = true);
	}

	void
	destroyXWindow(void)
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

	void
	handleKeyEvent(XKeyEvent &key)
	{
		Event::KBKeys l_key = Event::KEY_NONE;
		Event::KBActions l_action =
		    (key.type == KeyPress ? Event::KeyPressed : Event::KeyReleased);

		switch (XLookupKeysym(&key, 0)) {
		case XK_0:            l_key = Event::KEY_0; break;
		case XK_1:            l_key = Event::KEY_1; break;
		case XK_2:            l_key = Event::KEY_2; break;
		case XK_3:            l_key = Event::KEY_3; break;
		case XK_4:            l_key = Event::KEY_4; break;
		case XK_5:            l_key = Event::KEY_5; break;
		case XK_6:            l_key = Event::KEY_6; break;
		case XK_7:            l_key = Event::KEY_7; break;
		case XK_8:            l_key = Event::KEY_8; break;
		case XK_9:            l_key = Event::KEY_9; break;
		case XK_a:            l_key = Event::KEY_A; break;
		case XK_b:            l_key = Event::KEY_B; break;
		case XK_c:            l_key = Event::KEY_C; break;
		case XK_d:            l_key = Event::KEY_D; break;
		case XK_e:            l_key = Event::KEY_E; break;
		case XK_f:            l_key = Event::KEY_F; break;
		case XK_g:            l_key = Event::KEY_G; break;
		case XK_h:            l_key = Event::KEY_H; break;
		case XK_i:            l_key = Event::KEY_I; break;
		case XK_j:            l_key = Event::KEY_J; break;
		case XK_k:            l_key = Event::KEY_K; break;
		case XK_l:            l_key = Event::KEY_L; break;
		case XK_m:            l_key = Event::KEY_M; break;
		case XK_n:            l_key = Event::KEY_N; break;
		case XK_o:            l_key = Event::KEY_O; break;
		case XK_p:            l_key = Event::KEY_P; break;
		case XK_q:            l_key = Event::KEY_Q; break;
		case XK_r:            l_key = Event::KEY_R; break;
		case XK_s:            l_key = Event::KEY_S; break;
		case XK_t:            l_key = Event::KEY_T; break;
		case XK_u:            l_key = Event::KEY_U; break;
		case XK_v:            l_key = Event::KEY_V; break;
		case XK_w:            l_key = Event::KEY_W; break;
		case XK_x:            l_key = Event::KEY_X; break;
		case XK_y:            l_key = Event::KEY_Y; break;
		case XK_z:            l_key = Event::KEY_Z; break;
		case XK_Alt_L:        l_key = Event::KEY_ALT_L; break;
		case XK_Alt_R:        l_key = Event::KEY_ALT_R; break;
		case XK_BackSpace:    l_key = Event::KEY_BACKSPACE; break;
		case XK_Break:        l_key = Event::KEY_BREAK; break;
		case XK_Caps_Lock:    l_key = Event::KEY_CAPS_LOCK; break;
		case XK_Clear:        l_key = Event::KEY_CLEAR; break;
		case XK_Control_L:    l_key = Event::KEY_CONTROL_R; break;
		case XK_Control_R:    l_key = Event::KEY_CONTROL_L; break;
		case XK_Delete:       l_key = Event::KEY_DELETE; break;
		case XK_Down:         l_key = Event::KEY_DOWN; break;
		case XK_End:          l_key = Event::KEY_END; break;
		case XK_Escape:       l_key = Event::KEY_ESCAPE; break;
		case XK_Help:         l_key = Event::KEY_HELP; break;
		case XK_Home:         l_key = Event::KEY_HOME; break;
		case XK_Insert:       l_key = Event::KEY_INSERT; break;
		case XK_Left:         l_key = Event::KEY_LEFT; break;
		case XK_Menu:         l_key = Event::KEY_MENU; break;
		case XK_Meta_L:       l_key = Event::KEY_META_L; break;
		case XK_Meta_R:       l_key = Event::KEY_META_R; break;
		case XK_Num_Lock:     l_key = Event::KEY_NUM_LOCK; break;
		case XK_Page_Down:    l_key = Event::KEY_PAGE_DOWN; break;
		case XK_Page_Up:      l_key = Event::KEY_PAGE_UP; break;
		case XK_Pause:        l_key = Event::KEY_PAUSE; break;
		case XK_Print:        l_key = Event::KEY_PRINT; break;
		case XK_Return:       l_key = Event::KEY_RETURN; break;
		case XK_Right:        l_key = Event::KEY_RIGHT; break;
		case XK_Scroll_Lock:  l_key = Event::KEY_SCROLL_LOCK; break;
		case XK_Shift_L:      l_key = Event::KEY_SHIFT_L; break;
		case XK_Shift_R:      l_key = Event::KEY_SHIFT_R; break;
		case XK_Tab:          l_key = Event::KEY_TAB; break;
		case XK_Up:           l_key = Event::KEY_UP; break;
		case XK_backslash:    l_key = Event::KEY_BACKSLASH; break;
		case XK_bracketleft:  l_key = Event::KEY_BRACKETLEFT; break;
		case XK_bracketright: l_key = Event::KEY_BRACKETRIGHT; break;
		case XK_equal:        l_key = Event::KEY_EQUAL; break;
		case XK_less:         l_key = Event::KEY_LESS; break;
		case XK_quotedbl:     l_key = Event::KEY_DBLQUOTE; break;
		case XK_semicolon:    l_key = Event::KEY_SEMICOLON; break;
		case XK_space:        l_key = Event::KEY_SPACE; break;
		case XK_F1:           l_key = Event::KEY_F1; break;
		case XK_F2:           l_key = Event::KEY_F2; break;
		case XK_F3:           l_key = Event::KEY_F3; break;
		case XK_F4:           l_key = Event::KEY_F4; break;
		case XK_F5:           l_key = Event::KEY_F5; break;
		case XK_F6:           l_key = Event::KEY_F6; break;
		case XK_F7:           l_key = Event::KEY_F7; break;
		case XK_F8:           l_key = Event::KEY_F8; break;
		case XK_F9:           l_key = Event::KEY_F9; break;
		case XK_F10:          l_key = Event::KEY_F10; break;
		case XK_F11:          l_key = Event::KEY_F11; break;
		case XK_F12:          l_key = Event::KEY_F12; break;
		case XK_F13:          l_key = Event::KEY_F13; break;
		case XK_F14:          l_key = Event::KEY_F14; break;
		case XK_F15:          l_key = Event::KEY_F15; break;
		case XK_KP_0:         l_key = Event::KEY_K0; break;
		case XK_KP_1:         l_key = Event::KEY_K1; break;
		case XK_KP_2:         l_key = Event::KEY_K2; break;
		case XK_KP_3:         l_key = Event::KEY_K3; break;
		case XK_KP_4:         l_key = Event::KEY_K4; break;
		case XK_KP_5:         l_key = Event::KEY_K5; break;
		case XK_KP_6:         l_key = Event::KEY_K6; break;
		case XK_KP_7:         l_key = Event::KEY_K7; break;
		case XK_KP_8:         l_key = Event::KEY_K8; break;
		case XK_KP_9:         l_key = Event::KEY_K9; break;
		case XK_KP_Decimal:   l_key = Event::KEY_KDECIMAL; break;
		case XK_KP_Divide:    l_key = Event::KEY_KDIVIDE; break;
		case XK_KP_Multiply:  l_key = Event::KEY_KMULTIPLY; break;
		default: WARNING1("Unknown key pressed!");
		}

		Event::SharedEvent event(new Event::KeyboardEvent(l_key, l_action));
		Event::EventManager::Instance()->queue(event);
	}

} MVI;

bool
Viewport::Initialize(int w, int h, int d, bool f)
{
	bool l_success = MVI.createXWindow(w, h, d, f);

	if (l_success) Painter::Initialize();

	return(l_success);
}

void
Viewport::Finalize(void)
{
	Painter::Finalize();
	MVI.destroyXWindow();
}

bool
Viewport::Redisplay(int w, int h, int d, bool f)
{
	MVI.destroyXWindow();
	return(MVI.createXWindow(w, h, d, f));
}

void
Viewport::Tick(TIME t)
{
	TIMEOUT_INIT;
	XEvent e;

	while(TIMEOUT_DEC(t) > 0 && XPending(MVI.display)) {
		XNextEvent(MVI.display, &e);

		switch(e.type) {
		case ClientMessage: {
			XClientMessageEvent &client = e.xclient;
			if (static_cast<Atom>(client.data.l[0]) == MVI.wm_delete) {
				Event::QuitEvent event(-1);
				Event::EventManager::Instance()->dispatch(event);
			}
		} break;

		case KeyPress:
		case KeyRelease: MVI.handleKeyEvent(e.xkey); break;

		case ButtonPress:
		case ButtonRelease:
			/* TODO: Send Events */
		break;

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
	glXSwapBuffers(MVI.display, MVI.window);
	glClearColor(.0, .0, .0, .0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(static_cast<GLfloat>(MVI.size_hints->max_width / 2),
	             static_cast<GLfloat>(MVI.size_hints->max_height / 2), 0.0f);
}

const Math::Size2
Viewport::Size(void)
{
	if (MVI.size_hints)
		return(Math::Size2(static_cast<float>(MVI.size_hints->max_width),
		                   static_cast<float>(MVI.size_hints->max_height)));

	return(Math::Size2());
}

