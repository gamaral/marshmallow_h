/*
 * Copyright 2011-2012 Marshmallow Engine. All rights reserved.
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

#include <X11/X.h>
#include <X11/XKBlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include <cstring>

#include "headers.h"
#ifdef MARSHMALLOW_OPENGL_EGL
#  include <EGL/egl.h>
#else
#  include <GL/glx.h>
#endif
#include "extensions.h"

/*
 * X11 Viewport Notes
 *
 * Refresh rate is ignored since we don't switch between modes, we assume the
 * user has selected an appropriate mode.
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace { /******************************************** Anonymous Namespace */

struct ViewportData
{
	Window              window;
	XVisualInfo         vinfo;
	Display            *dpy;
	Math::Size2i        wsize;
#ifdef MARSHMALLOW_OPENGL_EGL
	EGLContext          egl_ctx;
	EGLDisplay          egl_dpy;
	EGLSurface          egl_surface;
#else
	GLXContext          ctx;
#endif
	Atom                wm_delete;
	Math::Size2f        size;
	bool                fullscreen;
	bool                loaded;
} s_data;

void
ResetViewportData(void)
{
	s_data.loaded = false;

	memset(&s_data.vinfo, 0, sizeof(s_data.vinfo));
	s_data.dpy = 0;
	s_data.fullscreen = false;
	s_data.size.zero();
	s_data.window = 0;
	s_data.wm_delete = 0;
	s_data.wsize[0] = s_data.wsize[1] = 0;

#ifdef MARSHMALLOW_OPENGL_EGL
	s_data.egl_ctx     = EGL_NO_CONTEXT;
	s_data.egl_dpy     = EGL_NO_DISPLAY;
	s_data.egl_surface = EGL_NO_SURFACE;
#else
	s_data.ctx = 0;
#endif
}

bool GLCreateSurface(bool vsync);
bool X11CreateWindow(uint16_t width, uint16_t height, uint8_t depth,
                     bool fullscreen);

bool
CreateWindow(uint16_t width, uint16_t height, uint8_t depth, bool fullscreen,
             bool vsync)
{
	using namespace Graphics;

	ResetViewportData();

	/* display */

	if (!X11CreateWindow(width, height, depth, fullscreen)) {
		MMERROR("X11: Failed to create window.");
		return(false);
	}

	/* context */

	if (!GLCreateSurface(vsync)) {
		MMERROR("GL: Failed to create surface.");
		return(false);
	}

	/* initialize context */

	glViewport(0, 0, s_data.wsize[0], s_data.wsize[1]);

	if (glGetError() != GL_NO_ERROR) {
		MMERROR("GL: Failed during initialization.");
		return(false);
	}

	/* viewport size */

	if (fullscreen) {
#if MARSHMALLOW_VIEWPORT_LOCK_WIDTH
		s_data.size[0] = static_cast<float>(width);
		s_data.size[1] = (s_data.size[0] * static_cast<float>(s_data.wsize[1])) /
		    static_cast<float>(s_data.wsize[0]);
#else
		s_data.size[1] = static_cast<float>(height);
		s_data.size[0] = (s_data.size[1] * static_cast<float>(s_data.wsize[0])) /
		    static_cast<float>(s_data.wsize[1]);
#endif
	}
	else {
		s_data.size[0] = static_cast<float>(width);
		s_data.size[1] = static_cast<float>(height);
	}

	Camera::Update();

	return(s_data.loaded = true);
}

void
DestroyWindow(void)
{
	if (s_data.dpy) {
#ifdef MARSHMALLOW_OPENGL_EGL
		eglMakeCurrent(s_data.egl_dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroyContext(s_data.egl_dpy, s_data.egl_ctx);
		eglDestroySurface(s_data.egl_dpy, s_data.egl_surface);
		eglTerminate(s_data.egl_dpy);
#else
		glXMakeCurrent(s_data.dpy, None, 0);
		if (s_data.ctx)
		    glXDestroyContext(s_data.dpy, s_data.ctx);
#endif
		if (s_data.window)
		    XDestroyWindow(s_data.dpy, s_data.window);

		XCloseDisplay(s_data.dpy);
	}

#ifdef MARSHMALLOW_OPENGL_EGL
	s_data.egl_ctx     = EGL_NO_CONTEXT;
	s_data.egl_dpy     = EGL_NO_DISPLAY;
	s_data.egl_surface = EGL_NO_SURFACE;
#else
	s_data.ctx = 0;
#endif
	s_data.dpy = 0;
	s_data.loaded = false;
	s_data.window = 0;
	s_data.wm_delete = 0;
}

void
HandleKeyEvent(XKeyEvent &key)
{
	typedef std::list<Event::KBKeys> KeyList;
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
	case XK_bracketleft:  l_key = Event::KEY_BRACKET_LEFT; break;
	case XK_bracketright: l_key = Event::KEY_BRACKET_RIGHT; break;
	case XK_equal:        l_key = Event::KEY_EQUAL; break;
	case XK_greater:      l_key = Event::KEY_GREATER; break;
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
	default: MMWARNING("Unknown key pressed!"); break;
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

bool
X11CreateWindow(uint16_t w, uint16_t h, uint8_t depth, bool fullscreen)
{
	s_data.fullscreen = fullscreen;
	s_data.wsize[0] = w;
	s_data.wsize[1] = h;

	/* open display */
	const char *l_display = getenv("DISPLAY");
	if (!(s_data.dpy = XOpenDisplay(l_display))) {
		MMERROR("X11: Unable to open display: " << l_display);
		return(false);
	}

	Window l_rwindow = DefaultRootWindow(s_data.dpy);

	/* select visual */
	if (!XMatchVisualInfo(s_data.dpy, XDefaultScreen(s_data.dpy), depth, TrueColor, &s_data.vinfo)) {
		MMERROR("X11: Failed to find an appropriate visual.");
		return(false);
	}

	XSync(s_data.dpy, true);

	/* window attributes */
	XSetWindowAttributes l_swattr;
	memset(&l_swattr, 0, sizeof(l_swattr));
	l_swattr.colormap = XCreateColormap(s_data.dpy, l_rwindow, s_data.vinfo.visual, AllocNone);
	l_swattr.event_mask =
		ButtonPressMask    |
		ButtonReleaseMask  |
		KeyPressMask       |
		KeyReleaseMask     |
		PointerMotionMask  |
		StructureNotifyMask;

	/*
	 * ** FULLSCREEN MODE **
	 */
	if (s_data.fullscreen) {

		/* default window size to display size */
		s_data.wsize[0] = DisplayWidth(s_data.dpy, s_data.vinfo.screen);
		s_data.wsize[1] = DisplayHeight(s_data.dpy, s_data.vinfo.screen);

		/* create a fullscreen window */
		l_swattr.override_redirect = true;
		s_data.window = XCreateWindow
		   (s_data.dpy,
		    l_rwindow,
		    0, 0, s_data.wsize[0], s_data.wsize[1],
		    0, s_data.vinfo.depth,
		    InputOutput,
		    s_data.vinfo.visual,
		    CWColormap|CWEventMask|CWOverrideRedirect,
		    &l_swattr);
		XMapRaised(s_data.dpy, s_data.window);

		/* notify window manager */
		Atom l_wm_state   = XInternAtom(s_data.dpy, "_NET_WM_STATE", false);
		Atom l_fullscreen = XInternAtom(s_data.dpy, "_NET_WM_STATE_FULLSCREEN", false);

		XEvent l_event;
		memset(&l_event, 0, sizeof(l_event));
		
		l_event.type                 = ClientMessage;
		l_event.xclient.window       = s_data.window;
		l_event.xclient.message_type = l_wm_state;
		l_event.xclient.format       = 32;
		l_event.xclient.data.l[0]    = 1;
		l_event.xclient.data.l[1]    = static_cast<long>(l_fullscreen);
		XSendEvent(s_data.dpy, l_rwindow, false, SubstructureNotifyMask, &l_event );
	}

	/*
	 * ** WINDOW MODE **
	 */
	else {
		s_data.window = XCreateWindow
		   (s_data.dpy,
		    l_rwindow,
		    (DisplayWidth(s_data.dpy, s_data.vinfo.screen) - s_data.wsize[0]) / 2,
		    (DisplayHeight(s_data.dpy, s_data.vinfo.screen) - s_data.wsize[1]) / 2,
		    s_data.wsize[0], s_data.wsize[1],
		    1, s_data.vinfo.depth,
		    InputOutput,
		    s_data.vinfo.visual,
		    CWColormap|CWEventMask,
		    &l_swattr);
		XMapRaised(s_data.dpy, s_data.window);

		/* set size hints */
		XSizeHints *l_size_hints;
		if (!(l_size_hints = XAllocSizeHints())) {
			MMERROR("Unable to allocate window size hints.");
			return(false);
		}
		l_size_hints->flags = PMinSize|PMaxSize;
		l_size_hints->min_width = s_data.wsize[0];
		l_size_hints->min_height = s_data.wsize[1];
		l_size_hints->max_width = s_data.wsize[0];
		l_size_hints->max_height = s_data.wsize[1];
		XSetWMNormalHints(s_data.dpy, s_data.window, l_size_hints);
		XFree(l_size_hints);
	}
	XkbSetDetectableAutoRepeat(s_data.dpy, true, 0);

#ifdef MARSHMALLOW_VIEWPORT_GRAB_INPUT
	/* take pointer and keyboard */
	XWarpPointer(s_data.dpy, None, s_data.window, 0, 0, 0, 0, 0, 0);
	XGrabKeyboard(s_data.dpy, s_data.window, true, GrabModeAsync,
	    GrabModeAsync, CurrentTime);
	XGrabPointer(s_data.dpy, s_data.window, true,
	    ButtonPressMask, GrabModeAsync, GrabModeAsync,
	    s_data.window, None, CurrentTime);
#endif

	XSync(s_data.dpy, true);

	/* set window title */
	XTextProperty l_window_name;
	static uint8_t l_window_title[] = MARSHMALLOW_BUILD_TITLE;
	l_window_name.value    = l_window_title;
	l_window_name.encoding = XA_STRING;
	l_window_name.format   = 8;
	l_window_name.nitems   = strlen(MARSHMALLOW_BUILD_TITLE);
	XSetWMName(s_data.dpy, s_data.window, &l_window_name);

	/* catch window manager delete event */
	s_data.wm_delete = XInternAtom(s_data.dpy, "WM_DELETE_WINDOW", false);
	XSetWMProtocols(s_data.dpy, s_data.window, &s_data.wm_delete, 1);

	return(true);
}

bool
GLCreateSurface(bool vsync)
{
	using namespace Graphics::OpenGL;

#ifdef MARSHMALLOW_OPENGL_EGL
	s_data.egl_dpy = eglGetDisplay(reinterpret_cast<EGLNativeDisplayType>(s_data.dpy));
	if (s_data.egl_dpy == EGL_NO_DISPLAY) {
		MMERROR("EGL: No display was found.");
		return(false);
	}
	
	if (!eglInitialize(s_data.egl_dpy, 0, 0)) {
		MMERROR("EGL: Initialization failed.");
		return(false);
	}
	
	EGLint l_attr[] = {
		EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
#ifdef MARSHMALLOW_OPENGL_ES2
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
#else
		EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
#endif
		EGL_BUFFER_SIZE,     s_data.vinfo.depth,
		EGL_DEPTH_SIZE,      0,
		EGL_RED_SIZE,        0,
		EGL_GREEN_SIZE,      0,
		EGL_BLUE_SIZE,       0,
		EGL_ALPHA_SIZE,      0,
		EGL_NONE,            EGL_NONE
	};

	EGLint l_config_count;
	EGLConfig l_config;
	if (!eglChooseConfig(s_data.egl_dpy, l_attr, &l_config, 1,
	                     &l_config_count))
		MMERROR("EGL: No config was chosen. EGLERROR=" << eglGetError());

	s_data.egl_surface =
	    eglCreateWindowSurface(s_data.egl_dpy, l_config,
	                           (EGLNativeWindowType)s_data.window,
	                           0);
	if (s_data.egl_surface == EGL_NO_SURFACE) {
		MMERROR("EGL: No surface was created. EGLERROR=" << eglGetError());
		return(false);
	}

	/* bind api */

#ifdef MARSHMALLOW_OPENGL_ES2
	eglBindAPI(EGL_OPENGL_ES_API);
#else
	eglBindAPI(EGL_OPENGL_API);
#endif

	EGLint l_ctxattr[] = {
#ifdef MARSHMALLOW_OPENGL_ES2
		EGL_CONTEXT_CLIENT_VERSION, 2,
#endif
		EGL_NONE,                   EGL_NONE
	};
	s_data.egl_ctx = eglCreateContext(s_data.egl_dpy, l_config, EGL_NO_CONTEXT, l_ctxattr);
	if (s_data.egl_ctx == EGL_NO_CONTEXT) {
		MMERROR("EGL: No context was created. EGLERROR=" << eglGetError());
		return(false);
	}

	if (!eglMakeCurrent(s_data.egl_dpy, s_data.egl_surface, s_data.egl_surface, s_data.egl_ctx)) {
		MMERROR("EGL: Failed to switch current context. EGLERROR=" << eglGetError());
		return(false);
	}

	/* extensions */

	Extensions::Initialize(eglQueryString(s_data.egl_dpy, EGL_EXTENSIONS));

	/* vsync */

	if (eglSwapInterval(s_data.egl_dpy, vsync ? 1 : 0) != EGL_TRUE)
		MMWARNING("EGL: Ignored out vsync request!");

	/* clear error state */

	eglGetError();
#else
	if (!(s_data.ctx = glXCreateContext(s_data.dpy, &s_data.vinfo, 0, GL_TRUE))) {
		MMERROR("GLX: Failed to create context!");
		return(false);
	}

	if (!glXMakeCurrent(s_data.dpy, s_data.window, s_data.ctx)) {
		MMERROR("GLX: Failed to make context current!");
		return(false);
	}

	if (!glXIsDirect(s_data.dpy, s_data.ctx)) {
		MMERROR("GLX: Context doesn't support direct rendering.");
		return(false);
	}

	/* extensions */

	Extensions::Initialize(glXQueryExtensionsString(s_data.dpy, s_data.vinfo.screen));
	
	/* vsync */

	if (Extensions::glxSwapInterval)
		Extensions::glxSwapInterval(vsync ? 1 : 0);
#endif

	return(true);
}

} /****************************************************** Anonymous Namespace */

namespace Graphics { /************************************ Graphics Namespace */

OpenGL::PFNPROC
OpenGL::glGetProcAddress(const char *f)
{
#ifdef MARSHMALLOW_OPENGL_EGL
	return(eglGetProcAddress(f));
#else
	return(glXGetProcAddress(reinterpret_cast<const GLubyte *>(f)));
#endif
}

/********************************************************* Graphics::Viewport */

bool
Viewport::Initialize(uint16_t width, uint16_t height, uint8_t depth, uint8_t,
                     bool fullscreen, bool vsync)
{
	Camera::Reset();

	if (!CreateWindow(width, height, depth, fullscreen, vsync)) {
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
Viewport::Redisplay(uint16_t width, uint16_t height, uint8_t depth, uint8_t,
                    bool fullscreen, bool vsync)
{
	DestroyWindow();

	if (!CreateWindow(width, height, depth, fullscreen, vsync)) {
		DestroyWindow();
		return(false);
	}
	return(true);
}

void
Viewport::Tick(void)
{
	XEvent e;

	while(XPending(s_data.dpy)) {
		XNextEvent(s_data.dpy, &e);

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

		default: MMINFO("Unknown viewport event received."); break;
		}
	}
}

void
Viewport::SwapBuffer(void)
{
#ifdef MARSHMALLOW_OPENGL_EGL
	eglSwapBuffers(s_data.egl_dpy, s_data.egl_surface);
#else
	glXSwapBuffers(s_data.dpy, s_data.window);
#endif
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
	static const Core::Type s_type("X11");
	return(s_type);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

