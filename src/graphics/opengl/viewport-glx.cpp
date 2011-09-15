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

#include <X11/X.h>
#include <X11/XKBlib.h>
#define XMD_H
#include <X11/extensions/xf86vmode.h>
#include <GL/gl.h>
#include <GL/glx.h>

#include <EASTL/list.h>

#include "core/logger.h"

#include "event/eventmanager.h"
#include "event/keyboardevent.h"
#include "event/quitevent.h"

#include "graphics/opengl/extensions/vbo.h"
#include "graphics/painter.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Graphics;
using namespace Graphics::OpenGL;

/******************************************************************************/

namespace
{
	struct ViewportData {
		XF86VidModeModeInfo dvminfo;
		Window        window;
		Display      *display;
		Math::Size2i  wsize;
		int           screen;
		GLXContext    context;
		Atom          wm_delete;
		Math::Triplet camera;
		Math::Size2f  size;
		Math::Point2  visible[2];
		bool          fullscreen;
		bool          loaded;
		bool          vbo_supported;
	} s_data;

	bool CheckVBOSupport(void);
	void UpdateViewport(void);
	void HandleKeyEvent(XKeyEvent &key);

	void InitializeViewport(void)
	{
		s_data.camera[0] = s_data.camera[1] = .0f; // camera x y
		s_data.camera[2] = 1.f;                    // camera zoom
		s_data.context = 0;
		s_data.display = 0;
		s_data.fullscreen = false;
		s_data.loaded = false;
		s_data.screen = 0;
		s_data.size.zero();
		s_data.vbo_supported = false;
		s_data.visible[0] = s_data.visible[1] = Math::Point2::Zero();
		s_data.window = 0;
		s_data.wm_delete = 0;
		s_data.wsize[0] = s_data.wsize[1] = 0;
	}

	bool
	CreateWindow(int w, int h, int d, bool f)
	{
		s_data.context   = 0;
		s_data.display   = 0;
		s_data.loaded    = false;
		s_data.screen    = 0;
		s_data.window    = 0;
		s_data.wm_delete = 0;

		s_data.fullscreen = f;
		s_data.wsize[0] = w;
		s_data.wsize[1] = h;

		/* open display */
		if (!(s_data.display = XOpenDisplay(0))) {
			MMERROR1("Unable to open X Display.");
			return(false);
		}

		/* get visual info */
		GLint gattr[] = {GLX_RGBA, GLX_DEPTH_SIZE, d, GLX_DOUBLEBUFFER, None};
		XVisualInfo *l_vinfo;
		if(!(l_vinfo = glXChooseVisual(s_data.display, 0, gattr))) {
			MMERROR1("Unable to choose X Visual Info.");
			return(false);
		}
		s_data.screen = l_vinfo->screen;

		/* get root window */
		Window l_rwindow = RootWindow(s_data.display, s_data.screen);
		
		/* window attributes */
		XSetWindowAttributes l_swattr;
		l_swattr.colormap =
		    XCreateColormap(s_data.display, l_rwindow, l_vinfo->visual, AllocNone);
		l_swattr.background_pixel =
		    BlackPixel(s_data.display, s_data.screen);
		l_swattr.border_pixel =
		    BlackPixel(s_data.display, s_data.screen);
		l_swattr.event_mask =
			ButtonPressMask |
			ButtonReleaseMask |
			KeyPressMask |
			KeyReleaseMask |
			PointerMotionMask |
			StructureNotifyMask;

		/* create window */
		if (s_data.fullscreen) {
			/* get display modes */
			XF86VidModeModeLine l_cmline;
			int l_cdotclock;
			XF86VidModeModeInfo **l_modes;
			int l_mode_count;
			int l_mode_selected = -1;

			XF86VidModeGetModeLine(s_data.display, s_data.screen, &l_cdotclock, &l_cmline);
			XF86VidModeGetAllModeLines(s_data.display, s_data.screen, &l_mode_count, &l_modes);
			s_data.dvminfo.dotclock = 0;
			for (int i = 0; i < l_mode_count; ++i) {
				MMINFO("Display mode %dx%d [%d]",
				    l_modes[i]->hdisplay,
				    l_modes[i]->vdisplay, i);

				if (!s_data.dvminfo.dotclock &&
				    l_modes[i]->hdisplay == l_cmline.hdisplay &&
				    l_modes[i]->vdisplay == l_cmline.vdisplay &&
				    static_cast<int>(l_modes[i]->dotclock) == l_cdotclock) {
					MMINFO1("Found current display mode");
					s_data.dvminfo = *l_modes[i];
				}
				if (l_mode_selected == -1 &&
				    l_modes[i]->hdisplay == w &&
				    l_modes[i]->vdisplay == h) {
					MMINFO1("Found appropriate display mode");
					l_mode_selected = i;
				}
			}

			if (l_mode_selected == -1) {
				MMERROR1("Unable to find a suitable display mode.");
				return(false);
			}

			XF86VidModeSwitchToMode(s_data.display, s_data.screen, l_modes[l_mode_selected]);
			XF86VidModeSetViewPort(s_data.display, s_data.screen, 0, 0);
			XFree(l_modes);

			/* allow display to settle after vidmode switch */
			XSync(s_data.display, False);
			sleep(2);

			/* create a fullscreen window */
			l_swattr.override_redirect = true;
			s_data.window = XCreateWindow
			   (s_data.display,
			    l_rwindow,
			    0, 0, w, h, 0,
			    d,
			    InputOutput,
			    l_vinfo->visual,
			    CWBackPixel|CWBorderPixel|CWColormap|
			    CWEventMask|CWOverrideRedirect,
			    &l_swattr);
			XMapRaised(s_data.display, s_data.window);
			XWarpPointer(s_data.display, None, s_data.window, 0, 0, 0, 0, 0, 0);
			XGrabKeyboard(s_data.display, s_data.window, true, GrabModeAsync,
			    GrabModeAsync, CurrentTime);
			XGrabPointer(s_data.display, s_data.window, true,
			    ButtonPressMask, GrabModeAsync, GrabModeAsync,
			    s_data.window, None, CurrentTime);

		} else {
			s_data.window = XCreateWindow
			   (s_data.display,
			    l_rwindow,
			    (DisplayWidth(s_data.display, s_data.screen) - w) / 2,
			    (DisplayHeight(s_data.display, s_data.screen) - h) / 2,
			    w, h,
			    1,
			    d,
			    InputOutput,
			    l_vinfo->visual,
			    CWBackPixel|CWBorderPixel|CWColormap|CWEventMask,
			    &l_swattr);
			XMapRaised(s_data.display, s_data.window);

			/* set size hints */
			XSizeHints *l_size_hints;
			if(!(l_size_hints = XAllocSizeHints())) {
				MMERROR1("Unable to allocate window size hints.");
				return(false);
			}
			l_size_hints->flags = PMinSize|PMaxSize;
			l_size_hints->min_width = w;
			l_size_hints->min_height = h;
			l_size_hints->max_width = w;
			l_size_hints->max_height = h;
			XSetWMNormalHints(s_data.display, s_data.window, l_size_hints);
			XFree(l_size_hints);
		}
		XkbSetDetectableAutoRepeat(s_data.display, true, 0);

		/* catch window manager delete event */
		s_data.wm_delete = XInternAtom(s_data.display, "WM_DELETE_WINDOW", false);
		XSetWMProtocols(s_data.display, s_data.window, &s_data.wm_delete, 1);

		/* create context */
		if (!(s_data.context = glXCreateContext(s_data.display, l_vinfo, 0, GL_TRUE))) {
			MMERROR1("Failed to create context!");
			return(false);
		}

		if (!glXMakeCurrent(s_data.display, s_data.window, s_data.context)) {
			MMERROR1("Failed to make context current!");
			return(false);
		}

		if (!glXIsDirect(s_data.display, s_data.context)) {
			MMERROR1("GLX context doesn't support direct rendering.");
			return(false);
		}

		/* check extensions */

		s_data.vbo_supported = CheckVBOSupport();

		/* set defaults */

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);

		/* initialize context */

		glViewport(0, 0, w, h);
		glClearColor(.0f, .0f, .0f, .0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		UpdateViewport();
		Viewport::SwapBuffer();

		if(glGetError() != GL_NO_ERROR) {
			MMERROR1("GLX failed during initialization.");
			return(false);
		}

		return(s_data.loaded = true);
	}

	void
	DestroyWindow(void)
	{
		if (s_data.display) {
			glXMakeCurrent(s_data.display, None, 0);
			if (s_data.context) glXDestroyContext(s_data.display, s_data.context);
			if (s_data.fullscreen) {
				XF86VidModeSwitchToMode(s_data.display, s_data.screen, &s_data.dvminfo);
				XF86VidModeSetViewPort(s_data.display, s_data.screen, 0, 0);
			}
			if (s_data.window) XDestroyWindow(s_data.display, s_data.window);
			XCloseDisplay(s_data.display);
		}

		s_data.context = 0;
		s_data.display = 0;
		s_data.loaded = false;
		s_data.screen = 0;
		s_data.window = 0;
		s_data.wm_delete = 0;
	}

	bool
	IsExtensionSupported(const char *list, const char *extension)
	{
		assert(list && extension
		    && 0 == strchr(extension, ' ')
		    && "Invalid list and/or extension");

		const char *start = list;
		const char *where, *terminator;

		while ((where = strstr( start, extension ))) {
			terminator = where + strlen( extension );

			if ((where == start || *(where - 1) == ' ')
			    && (*terminator == ' ' || *terminator == '\0'))
				return(true);
		}

		return(false);
	}

	bool
	CheckVBOSupport(void)
	{
		HasVectorBufferObjectSupport = false;

		if (!IsExtensionSupported
		    (reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS)), "GL_ARB_vertex_buffer_object"))
			return(false);

		glGenBuffersARB =
		    reinterpret_cast<PFNGLGENBUFFERSARBPROC>
		        (glXGetProcAddress(reinterpret_cast<const GLubyte*>("glGenBuffersARB")));
		glBindBufferARB =
		    reinterpret_cast<PFNGLBINDBUFFERARBPROC>
		        (glXGetProcAddress(reinterpret_cast<const GLubyte*>("glBindBufferARB")));
		glBufferDataARB =
		    reinterpret_cast<PFNGLBUFFERDATAARBPROC>
		        (glXGetProcAddress(reinterpret_cast<const GLubyte*>("glBufferDataARB")));
		glBufferSubDataARB =
		    reinterpret_cast<PFNGLBUFFERSUBDATAARBPROC>
		        (glXGetProcAddress(reinterpret_cast<const GLubyte*>("glBufferSubDataARB")));
		glDeleteBuffersARB =
		    reinterpret_cast<PFNGLDELETEBUFFERSARBPROC>
		        (glXGetProcAddress(reinterpret_cast<const GLubyte*>("glDeleteBuffersARB")));
		glGetBufferParameterivARB =
		    reinterpret_cast<PFNGLGETBUFFERPARAMETERIVARBPROC>
		        (glXGetProcAddress(reinterpret_cast<const GLubyte*>("glGetBufferParameterivARB")));
		glMapBufferARB =
		    reinterpret_cast<PFNGLMAPBUFFERARBPROC>
		        (glXGetProcAddress(reinterpret_cast<const GLubyte*>("glMapBufferARB")));
		glUnmapBufferARB =
		    reinterpret_cast<PFNGLUNMAPBUFFERARBPROC>
		        (glXGetProcAddress(reinterpret_cast<const GLubyte*>("glUnmapBufferARB")));

		if (glGenBuffersARB
		 && glBindBufferARB
		 && glBufferDataARB
		 && glBufferSubDataARB
		 && glDeleteBuffersARB
		 && glGetBufferParameterivARB
		 && glMapBufferARB
		 && glUnmapBufferARB) {
			HasVectorBufferObjectSupport = true;
			return(true);
		}

		/* clean up */
		glBindBufferARB    = 0;
		glBufferDataARB    = 0;
		glBufferSubDataARB = 0;
		glDeleteBuffersARB = 0;
		glGenBuffersARB    = 0;
		glGetBufferParameterivARB = 0;
		glMapBufferARB     = 0;
		glUnmapBufferARB   = 0;
		return(false);
	}

	void
	UpdateViewport(void)
	{
		s_data.size[0] = DEFAULT_VIEWPORT_VWIDTH * s_data.camera[2];
		s_data.size[1] = DEFAULT_VIEWPORT_VHEIGHT * s_data.camera[2];

		const float l_hw = s_data.size[0] / 2.f;
		const float l_hh = s_data.size[1] / 2.f;

		/* update visible area */
		s_data.visible[0][0] = -l_hw + s_data.camera[0];
		s_data.visible[0][1] =  l_hh + s_data.camera[1];
		s_data.visible[1][0] =  l_hw + s_data.camera[0];
		s_data.visible[1][1] = -l_hh + s_data.camera[1];

		/* update projection */

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(s_data.visible[0].x(), s_data.visible[1].x(),
		        s_data.visible[1].y(), s_data.visible[0].y(), -1.f, 1.f);
		glMatrixMode(GL_MODELVIEW);
	}

	void
	HandleKeyEvent(XKeyEvent &key)
	{
		typedef eastl::list<Event::KBKeys> KeyList;
		static KeyList s_keys_pressed;

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
		default: MMWARNING1("Unknown key pressed!");
		}

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

/******************************************************************************/

bool
Viewport::Initialize(int w, int h, int d, bool f)
{
	InitializeViewport();

	if (!CreateWindow(w, h, d, f)) {
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
}

bool
Viewport::Redisplay(int w, int h, int d, bool f)
{
	DestroyWindow();

	if(!CreateWindow(w, h, d, f)) {
		DestroyWindow();
		return(false);
	}
	return(true);
}

void
Viewport::Tick(TIME t)
{
	TIMEOUT_INIT;
	XEvent e;

	while(TIMEOUT_DEC(t) > 0 && XPending(s_data.display)) {
		XNextEvent(s_data.display, &e);

		switch(e.type) {
		case ClientMessage: {
			XClientMessageEvent &client = e.xclient;
			if (static_cast<Atom>(client.data.l[0]) == s_data.wm_delete) {
				Event::QuitEvent l_event(-1);
				Event::EventManager::Instance()->dispatch(l_event);
			}
		} break;

		case KeyPress:
		case KeyRelease: HandleKeyEvent(e.xkey); break;

		case ButtonPress:
		case ButtonRelease:
			/* TODO: Send Events */
		break;

		case MotionNotify:
			/* TODO: Send Events */
			break;

		default: MMINFO1("Unknown viewport event received."); break;
		}
	}
}

void
Viewport::SwapBuffer(void)
{
	glXSwapBuffers(s_data.display, s_data.window);
	glClearColor(.0f, .0f, .0f, .0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

const Math::Triplet &
Viewport::Camera(void)
{
	return(s_data.camera);
}

void
Viewport::MoveCamera(const Math::Triplet &c)
{
	s_data.camera[0] = c[0];
	s_data.camera[1] = c[1];
	s_data.camera[2] = c[2];
	UpdateViewport();
}

void
Viewport::VisibleArea(Math::Point2 *tl, Math::Point2 *br)
{
	if (tl) *tl = s_data.visible[0];
	if (br) *br = s_data.visible[1];
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
	static const Core::Type s_type("GLX");
	return(s_type);
}

