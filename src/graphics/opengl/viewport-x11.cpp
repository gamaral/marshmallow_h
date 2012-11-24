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

#include "graphics/viewport_p.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifdef MARSHMALLOW_X11_XINERAMA
#  include <X11/extensions/Xinerama.h>
#endif

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <cassert>
#include <cstring>

#include "core/logger.h"
#include "core/type.h"

#include "event/eventmanager.h"
#include "event/quitevent.h"
#include "event/viewportevent.h"

#ifdef MARSHMALLOW_INPUT_UNIX_X11
#  include "input/unix/x11.h"
#endif

#include "graphics/camera.h"
#include "graphics/color.h"
#include "graphics/display.h"
#include "graphics/painter_p.h"


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
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */
namespace OpenGL { /****************************** Graphics::OpenGL Namespace */
namespace { /************************ Graphics::OpenGL::<anonymous> Namespace */

namespace X11Viewport {

	inline bool Initialize(void);
	inline void Finalize(void);

	inline void Reset(int state);

	inline bool Create(const Display &display);
	inline void Destroy(void);

	inline bool Show(void);
	inline void Hide(void);

	inline void SwapBuffer(void);

	inline bool CreateGLContext(void);
	inline void DestroyGLContext(void);

#ifdef MARSHMALLOW_OPENGL_EGL
	inline bool CreateEGLContext(void);
	inline void DestroyEGLContext(void);
#else
	inline bool CreateGLXContext(void);
	inline void DestroyGLXContext(void);
#endif

	inline bool CreateX11Window(void);
	inline void DestroyX11Window(void);
	inline void ProcessX11Events(void);

	enum StateFlag
	{
		sfUninitialized = 0,
		sfReady         = (1 << 0),
		sfX11Display    = (1 << 1),
		sfX11Window     = (1 << 2),
		sfGLDisplay     = (1 << 3),
		sfGLSurface     = (1 << 4),
		sfGLContext     = (1 << 5),
		sfGLCurrent     = (1 << 6),
		sfExposed       = (1 << 7),
		sfFocused       = (1 << 8),
		sfTerminated    = (1 << 9),
		sfX11Valid      = sfX11Display|sfX11Window,
#ifdef MARSHMALLOW_OPENGL_EGL
		sfGLValid       = sfGLDisplay|sfGLSurface|sfGLContext|sfGLCurrent,
#else
		sfGLValid       = sfGLContext|sfGLCurrent,
#endif
		sfValid         = sfX11Valid|sfGLValid,
		sfActive        = sfReady|sfValid|sfExposed|sfFocused
	};

	/******************* MARSHMALLOW */
	Display             dpy;
	Math::Size2i        wsize;
	Math::Size2f        vsize;
	int                 flags;

	/*************************** X11 */
	::Display          *xdpy;
	Window              xroot;
	Window              xwindow;
	XVisualInfo         xvinfo;
	Atom                xa_delete;

#ifdef MARSHMALLOW_OPENGL_EGL
	/*************************** EGL */
	EGLDisplay          egl_dpy;
	EGLSurface          egl_surface;
	EGLContext          egl_ctx;
#else
	/*************************** GLX */
	GLXContext          glx_ctx;
#endif

}

bool
X11Viewport::Initialize(void)
{
	/* default display display */
	dpy.depth      = MARSHMALLOW_VIEWPORT_DEPTH;
	dpy.fullscreen = MARSHMALLOW_VIEWPORT_FULLSCREEN;
	dpy.height     = MARSHMALLOW_VIEWPORT_HEIGHT;
	dpy.vsync      = MARSHMALLOW_VIEWPORT_VSYNC;
	dpy.width      = MARSHMALLOW_VIEWPORT_WIDTH;

	Reset(sfUninitialized);

	/*
	 * Open X11 Display
	 */
	const char *l_display = getenv("DISPLAY");
	if (!(xdpy = XOpenDisplay(l_display))) {
		MMERROR("X11: Unable to open display: " << l_display);
		return(false);
	}
	flags |= sfX11Display;

#ifdef MARSHMALLOW_INPUT_UNIX_X11
	Input::Unix::X11::InitializeKeyboard(xdpy);
#endif

	xvinfo.screen = XDefaultScreen(xdpy);
	xroot = XDefaultRootWindow(xdpy);
	xa_delete = XInternAtom(xdpy, "WM_DELETE_WINDOW", False);

#ifdef MARSHMALLOW_OPENGL_EGL
	/*
	 * Open EGL Display
	 */
	egl_dpy = eglGetDisplay(reinterpret_cast<EGLNativeDisplayType>(xdpy));
	if (egl_dpy == EGL_NO_DISPLAY) {
		MMERROR("EGL: No display was found.");
		return(false);
	}
	if (eglInitialize(egl_dpy, 0, 0) == EGL_FALSE) {
		MMERROR("EGL: Initialization failed.");
		return(false);
	}
	flags |= sfGLDisplay;
#endif
	
	/*
	 * Initial Camera Reset (IMPORTANT)
	 */
	Camera::Reset();

	/*
	 * Initial Background Color (IMPORTANT)
	 */
	Painter::SetBackgroundColor(Color::Black());

	return(true);
}

void
X11Viewport::Finalize(void)
{
	/* set termination flag */
	flags |= sfTerminated;

	/*
	 * Destroy viewport if valid
	 */
	if (sfValid == (flags & sfValid))
		Destroy();

#ifdef MARSHMALLOW_OPENGL_EGL
	/*
	 * Close EGL Display
	 */
	if (sfGLDisplay == (flags & sfGLDisplay)) {
		eglTerminate(egl_dpy), egl_dpy = 0;
		flags ^= sfGLDisplay;
	}
#endif

	/*
	 * Close X11 Display
	 */
	if (sfX11Display == (flags & sfX11Display)) {
		XCloseDisplay(xdpy), xdpy = 0;
		xroot = 0;
		xvinfo.screen = 0;
		flags ^= sfX11Display;
	}

	/* sanity check */
	assert(flags == sfTerminated && "We seem to have some stray flags!");

	flags = sfUninitialized;
}

void
X11Viewport::Reset(int state)
{
	flags = state;
	vsize.zero();
	wsize.zero();

	if (state == sfUninitialized) {
		xwindow = 0;
		memset(&xvinfo, 0, sizeof(xvinfo));

#ifdef MARSHMALLOW_OPENGL_EGL
		egl_surface = EGL_NO_SURFACE;
		egl_ctx     = EGL_NO_CONTEXT;
#else
		glx_ctx = 0;
#endif
	}

	/* sanity check */
	else {
		assert(0 == xwindow
		    && "Viewport didn't get destroyed cleanly!");
#ifdef MARSHMALLOW_OPENGL_EGL
		assert(EGL_NO_SURFACE == egl_surface && EGL_NO_CONTEXT == egl_ctx
		    && "[EGL] Viewport didn't get destroyed cleanly!");
#else
		assert(0 == glx_ctx
		    && "[GLX] Viewport didn't get destroyed cleanly!");
#endif
	}
}

bool
X11Viewport::Create(const Display &display)
{
	/* sanity checks */
	assert(sfX11Display == (flags & sfX11Display)
	    && "No valid X11 display!");
#ifdef MARSHMALLOW_OPENGL_EGL
	assert(sfGLDisplay == (flags & sfGLDisplay)
	    && "No valid EGL display!");
#endif

	/*
	 * Check if already valid (no no)
	 */
	if (sfValid == (flags & sfValid))
		return(false);

	/* assign new display display */
	dpy = display;

	/*
	 * Create X11 Window
	 */
	if (!CreateX11Window()) {
		MMERROR("X11: Failed to create window.");
		return(false);
	}

	/*
	 * Create GLX/EGL Context
	 */
	if (!CreateGLContext()) {
		MMERROR("GL: Failed to create surface.");
		DestroyX11Window();
		return(false);
	}

	/* sanity check */
	assert(sfValid == (flags & sfValid)
	    && "Valid viewport was expected!");

	/* initialize context */

	glViewport(0, 0, wsize.width, wsize.height);

	if (glGetError() != GL_NO_ERROR) {
		MMERROR("GL: Failed during initialization.");
		DestroyGLContext();
		DestroyX11Window();
		return(false);
	}

	/* viewport size */

	if (dpy.fullscreen) {
#if MARSHMALLOW_VIEWPORT_LOCK_WIDTH
		vsize.width = static_cast<float>(dpy.width);
		vsize.height = (vsize.width * static_cast<float>(wsize.height)) /
		    static_cast<float>(wsize.width);
#else
		vsize.height = static_cast<float>(dpy.height);
		vsize.width = (vsize.height * static_cast<float>(wsize.width)) /
		    static_cast<float>(wsize.height);
#endif
	}
	else vsize.set(dpy.width, dpy.height);

	/* sub-systems */

	Camera::Update();

	Painter::Initialize();

	/* activate */

	flags |= sfReady;

	/* broadcast */

	Event::ViewportEvent l_event(Event::ViewportEvent::Created);
	Event::EventManager::Instance()->dispatch(l_event);

	return(true);
}

void
X11Viewport::Destroy(void)
{
	/* check for valid state */
	if (sfValid != (flags & sfValid))
		return;

	/* deactivate */

	flags &= ~(sfReady);

	/* hide viewport if exposed */
	if (sfExposed == (flags & sfExposed))
		Hide();

	/* broadcast */

	if (0 == (flags & sfTerminated)) {
		Event::ViewportEvent l_event(Event::ViewportEvent::Destroyed);
		Event::EventManager::Instance()->dispatch(l_event);
	}

	Painter::Finalize();

	DestroyGLContext();
	DestroyX11Window();

	/* sanity check */
	assert(0 == (flags & ~(sfX11Display|sfGLDisplay|sfTerminated))
	    && "We seem to have some stray flags!");

	Reset(flags & (sfX11Display|sfGLDisplay|sfTerminated));
}

bool
X11Viewport::Show(void)
{
	/* sanity checks */

	if (sfValid != (flags & sfValid)) {
	    assert(false && "Valid viewport was expected!");
	    return(false);
	}

	if (sfExposed == (flags & sfExposed))
	    return(true);

	/* map window */
	XMapRaised(xdpy, xwindow);

	/* move pointer to lower right */
	XWarpPointer(xdpy, xroot, xwindow, 0, 0, 0, 0,
	    wsize.width, wsize.height);

	if (dpy.fullscreen) {
		/* notify window manager */
		Atom l_wm_state = XInternAtom(xdpy, "_NET_WM_STATE", False);
		Atom l_wms_above = XInternAtom(xdpy, "_NET_WM_STATE_ABOVE", False);
		Atom l_wms_fullscreen = XInternAtom(xdpy, "_NET_WM_STATE_FULLSCREEN", False);

		XEvent l_event;
		memset(&l_event, 0, sizeof(l_event));
		
		l_event.type                 = ClientMessage;
		l_event.xclient.window       = xwindow;
		l_event.xclient.message_type = l_wm_state;
		l_event.xclient.format       = 32;
		l_event.xclient.data.l[0]    = 1; /* enable */
		l_event.xclient.data.l[1]    = static_cast<long>(l_wms_fullscreen);
		l_event.xclient.data.l[2]    = static_cast<long>(l_wms_above);
		l_event.xclient.data.l[3]    = 1; /* source is window */
		l_event.xclient.data.l[4]    = 0;

		if (0 == XSendEvent(xdpy, xroot, False,
		                    SubstructureRedirectMask, &l_event))
			MMERROR("X11: Failed to send fullscreen event to window manager.");

		/* take pointer and keyboard */

#ifdef MARSHMALLOW_X11_GRAB_KEYBOARD
		XGrabKeyboard(xdpy, xwindow, True, GrabModeAsync,
		    GrabModeAsync, CurrentTime);
#endif

#ifdef MARSHMALLOW_X11_GRAB_POINTER
		XGrabPointer(xdpy, xwindow, True,
		    ButtonPressMask, GrabModeAsync, GrabModeAsync,
		    xwindow, None, CurrentTime);
#endif
	}

	XSync(xdpy, False);

	return((flags |= sfExposed));
}

void
X11Viewport::Hide(void)
{
	/* sanity checks */

	if (sfValid != (flags & sfValid)) {
	    assert(false && "Valid viewport was expected!");
	    return;
	}

	if (0 == (flags & sfExposed))
		return;

	XUnmapWindow(xdpy, xwindow);
	flags &= ~(sfExposed|sfFocused);

	XSync(xdpy, True);
}

void
X11Viewport::SwapBuffer(void)
{
	assert(sfValid == (flags & sfValid)
	    && "Attempted to swap buffer with an invalid viewport");
#ifdef MARSHMALLOW_OPENGL_EGL
	eglSwapBuffers(egl_dpy, egl_surface);
#else
	glXSwapBuffers(xdpy, xwindow);
#endif
}

bool
X11Viewport::CreateX11Window(void)
{
	/* select visual */
	if (!XMatchVisualInfo(xdpy, xvinfo.screen, dpy.depth, TrueColor, &xvinfo)) {
		MMERROR("X11: Failed to find an appropriate visual.");
		return(false);
	}

	/* window attributes */
	XSetWindowAttributes l_swattr;
	memset(&l_swattr, 0, sizeof(l_swattr));

	l_swattr.background_pixel = 0;
	l_swattr.border_pixel = 0;
	l_swattr.colormap =
	    XCreateColormap(xdpy, xroot, xvinfo.visual, AllocNone);
	l_swattr.event_mask = FocusChangeMask|StructureNotifyMask;

#ifdef MARSHMALLOW_INPUT_UNIX_X11
	l_swattr.event_mask |= ButtonPressMask
	    | ButtonReleaseMask
	    | KeyPressMask
	    | KeyReleaseMask
	    | PointerMotionMask;
#endif

	int l_window_x = 0;
	int l_window_y = 0;

	int l_dpy_x = 0;
	int l_dpy_y = 0;
	int l_dpy_width  = XDisplayWidth(xdpy,  xvinfo.screen);
	int l_dpy_height = XDisplayHeight(xdpy, xvinfo.screen);

	unsigned long l_mask = CWBackPixel|CWBorderPixel|CWColormap|CWEventMask;

#ifdef MARSHMALLOW_X11_XINERAMA
	int l_mon_sel = 0;
	int l_mon_c;
	XineramaScreenInfo *l_mon;

	l_mon = XineramaQueryScreens(xdpy, &l_mon_c);

	if (l_mon_c > 0) {
		char *l_mon_selstr = getenv("MM_XINERAMA_SCREEN");
		if (l_mon_selstr)
			l_mon_sel = atoi(l_mon_selstr) % l_mon_c;

		if (l_mon_c > l_mon_sel) {
			l_dpy_x = l_mon[l_mon_sel].x_org;
			l_dpy_y = l_mon[l_mon_sel].y_org;
			l_dpy_width  = l_mon[l_mon_sel].width;
			l_dpy_height = l_mon[l_mon_sel].height;
		}
	}
	XFree(l_mon), l_mon = 0;
#endif

	/*
	 * ** FULLSCREEN MODE **
	 */
	if (dpy.fullscreen) {
		/* window size = display size */
		wsize.set(l_dpy_width, l_dpy_height);

#ifdef MARSHMALLOW_X11_OVERRIDE_REDIRECT
		l_swattr.override_redirect = true;
		l_mask |= CWOverrideRedirect;
#endif
	}

	/*
	 * ** WINDOW MODE **
	 */
	else {
		wsize.set(dpy.width, dpy.height);
		l_window_x = (l_dpy_width  - wsize.width)  / 2;
		l_window_y = (l_dpy_height - wsize.height) / 2;
	}

	/* create a fullscreen window */
	xwindow = XCreateWindow
	   (xdpy, xroot,
	    l_dpy_x + l_window_x, l_dpy_y + l_window_y,
	    static_cast<unsigned int>(wsize.width),
	    static_cast<unsigned int>(wsize.height),
	    dpy.fullscreen ? 0 : 1,
	    xvinfo.depth,
	    InputOutput,
	    xvinfo.visual,
	    l_mask,
	    &l_swattr);

	/* set size hints */

	XSizeHints *l_size_hints;
	if (0 == (l_size_hints = XAllocSizeHints())) {
		MMERROR("X11: Unable to allocate memory for window size"
			" hints.");
		return(false);
	}
	l_size_hints->min_height = l_size_hints->max_height = wsize.height;
	l_size_hints->min_width  = l_size_hints->max_width  = wsize.width;
	l_size_hints->flags      = PMinSize|PMaxSize;
	XSetWMNormalHints(xdpy, xwindow, l_size_hints);
	XFree(l_size_hints);

	/* set window title */
	XTextProperty l_window_name;
	static uint8_t l_window_title[] = MARSHMALLOW_VIEWPORT_TITLE;
	l_window_name.value    = l_window_title;
	l_window_name.encoding = XA_STRING;
	l_window_name.format   = 8;
	l_window_name.nitems   = strlen(MARSHMALLOW_VIEWPORT_TITLE);
	XSetWMName(xdpy, xwindow, &l_window_name);
	XSetWMProtocols(xdpy, xwindow, &xa_delete, 1);

	XSync(xdpy, False);

	flags |= sfX11Window;
	return(true);
}

void
X11Viewport::DestroyX11Window(void)
{
	XDestroyWindow(xdpy, xwindow), xwindow = 0;
	XSync(xdpy, True);

	flags &= ~(sfX11Window);
}

void
X11Viewport::ProcessX11Events(void)
{
	XEvent e;

	while(XPending(xdpy)) {
		XNextEvent(xdpy, &e);

		switch(e.type) {
		case ClientMessage: {
			XClientMessageEvent &client = e.xclient;
			if (static_cast<Atom>(client.data.l[0]) == xa_delete) {
				Event::QuitEvent l_event(-1);
				Event::EventManager::Instance()->dispatch(l_event);
				return;
			}
		} break;

		case FocusIn:
			MMDEBUG("Viewport focus.");

			flags |= sfFocused;
			break;

		case FocusOut:
			MMDEBUG("Viewport lost focus.");
			flags &= ~(sfFocused);
			break;

		default:
#ifdef MARSHMALLOW_INPUT_UNIX_X11
			Input::Unix::X11::HandleEvent(e);
#else
			MMINFO("Unknown viewport event received.");
#endif
			break;
		}
	}
}

bool
X11Viewport::CreateGLContext(void)
{
#ifdef MARSHMALLOW_OPENGL_EGL
	return(CreateEGLContext());
#else
	return(CreateGLXContext());
#endif
}

void
X11Viewport::DestroyGLContext(void)
{
	/* extensions */

	Extensions::Finalize();

#ifdef MARSHMALLOW_OPENGL_EGL
	DestroyEGLContext();
#else
	DestroyGLXContext();
#endif
}

#ifdef MARSHMALLOW_OPENGL_EGL
bool
X11Viewport::CreateEGLContext(void)
{
	/*
	 * Bind OpenGL API
	 */
	const EGLBoolean l_bind_result =
#ifdef MARSHMALLOW_OPENGL_ES2
	    eglBindAPI(EGL_OPENGL_ES_API);
#else
	    eglBindAPI(EGL_OPENGL_API);
#endif
	if (l_bind_result != EGL_TRUE) {
		MMERROR("EGL: Failed to bind required OpenGL API.");
		return(false);
	}

	/*
	 * Choose EGL Config
	 */
	EGLint l_attr[] = {
		EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
#ifdef MARSHMALLOW_OPENGL_ES2
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
#else
		EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
#endif
		EGL_BUFFER_SIZE,     xvinfo.depth,
		EGL_DEPTH_SIZE,      0,
		EGL_RED_SIZE,        0,
		EGL_GREEN_SIZE,      0,
		EGL_BLUE_SIZE,       0,
		EGL_ALPHA_SIZE,      0,
		EGL_NONE,            EGL_NONE
	};
	EGLint l_config_count;
	EGLConfig l_config;
	if (!eglChooseConfig(egl_dpy, l_attr, &l_config, 1, &l_config_count))
		MMERROR("EGL: No config was chosen. EGLERROR=" << eglGetError());

	/*
	 * Create EGL Surface
	 */
	egl_surface = eglCreateWindowSurface(egl_dpy,
	                                     l_config,
	                (EGLNativeWindowType)xwindow,
	                                     0);
	if (egl_surface == EGL_NO_SURFACE) {
		MMERROR("EGL: No surface was created. EGLERROR=" << eglGetError());
		return(false);
	}
	flags |= sfGLSurface;

	/*
	 * Create EGL Context
	 */
	EGLint l_ctxattr[] = {
#ifdef MARSHMALLOW_OPENGL_ES2
		EGL_CONTEXT_CLIENT_VERSION, 2,
#endif
		EGL_NONE,                   EGL_NONE
	};
	egl_ctx = eglCreateContext(egl_dpy, l_config, EGL_NO_CONTEXT, l_ctxattr);
	if (egl_ctx == EGL_NO_CONTEXT) {
		MMERROR("EGL: No context was created. EGLERROR=" << eglGetError());
		return(false);
	}
	flags |= sfGLContext;

	/*
	 * Make EGL Context Current
	 */
	if (!eglMakeCurrent(egl_dpy, egl_surface, egl_surface, egl_ctx)) {
		MMERROR("EGL: Failed to switch current context. EGLERROR=" << eglGetError());
		return(false);
	}
	flags |= sfGLCurrent;

	/* extensions */

	Extensions::Initialize(eglQueryString(egl_dpy, EGL_EXTENSIONS));

	/* vsync */

	if (eglSwapInterval(egl_dpy, dpy.vsync) != EGL_TRUE)
		MMERROR("EGL: Swap interval request was ignored!");

	/* clear error state */

	eglGetError();

	return(true);
}

void
X11Viewport::DestroyEGLContext(void)
{
	if (0 == (flags & sfGLValid))
		return;

	/*
	 * Clear Current EGL Context
	 */
	if (sfGLCurrent == (flags & sfGLCurrent)) {
		eglMakeCurrent(egl_dpy, EGL_NO_SURFACE, EGL_NO_SURFACE,
		               EGL_NO_CONTEXT);
		flags ^= sfGLCurrent;
	}

	/*
	 * Destroy EGL Context
	 */
	if (sfGLContext == (flags & sfGLContext)) {
		eglDestroyContext(egl_dpy, egl_ctx),
		    egl_ctx = EGL_NO_CONTEXT;
		flags ^= sfGLContext;
	}

	/*
	 * Destroy EGL Surface
	 */
	if (sfGLSurface == (flags & sfGLSurface)) {
		eglDestroySurface(egl_dpy, egl_surface),
		    egl_surface = EGL_NO_SURFACE;
		flags ^= sfGLSurface;
	}
}
#else // !MARSHMALLOW_OPENGL_EGL (GLX)
bool
X11Viewport::CreateGLXContext(void)
{
	if (!(glx_ctx = glXCreateContext(xdpy, &xvinfo, 0, GL_TRUE))) {
		MMERROR("GLX: Failed to create context!");
		return(false);
	}
	flags |= sfGLContext;

	if (!glXMakeCurrent(xdpy, xwindow, glx_ctx)) {
		MMERROR("GLX: Failed to make context current!");
		return(false);
	}
	flags |= sfGLCurrent;

	if (!glXIsDirect(xdpy, glx_ctx)) {
		MMERROR("GLX: Context doesn't support direct rendering.");
		return(false);
	}

	/* extensions */

	Extensions::Initialize(glXQueryExtensionsString(xdpy, xvinfo.screen));
	
	/* vsync */

	if (Extensions::glXSwapIntervalMESA) {
		Extensions::glXSwapIntervalMESA(dpy.vsync);
		if (Extensions::glXGetSwapIntervalMESA
		    && dpy.vsync != Extensions::glXGetSwapIntervalMESA())
			MMERROR("X11: Swap interval request was ignored!");
	}
	else if (Extensions::glxSwapInterval)
		Extensions::glxSwapInterval(dpy.vsync);

	return(true);
}

void
X11Viewport::DestroyGLXContext(void)
{
	if (0 == (flags & sfGLValid))
		return;

	if (sfGLCurrent == (flags & sfGLCurrent)) {
		glXMakeCurrent(xdpy, None, 0);
		flags ^= sfGLCurrent;
	}

	if (sfGLContext == (flags & sfGLContext)) {
		glXDestroyContext(xdpy, glx_ctx), glx_ctx = 0;
		flags ^= sfGLContext;
	}
}
#endif

} /********************************** Graphics::OpenGL::<anonymous> Namespace */

PFNPROC
glGetProcAddress(const char *f)
{
#ifdef MARSHMALLOW_OPENGL_EGL
	return(eglGetProcAddress(f));
#else
	return(glXGetProcAddress(reinterpret_cast<const GLubyte *>(f)));
#endif
}

} /*********************************************** Graphics::OpenGL Namespace */

bool
Viewport::Active(void)
{
	using namespace OpenGL::X11Viewport;
	return(sfActive == (flags & sfActive));
}

bool
Viewport::Initialize(void)
{
	using namespace OpenGL;
	return(X11Viewport::Initialize());
}

void
Viewport::Finalize(void)
{
	using namespace OpenGL;
	X11Viewport::Finalize();
}

bool
Viewport::Setup(const Graphics::Display &display)
{
	using namespace OpenGL;

	X11Viewport::Destroy();

	if (!X11Viewport::Create(display)) {
		X11Viewport::Destroy();
		return(false);
	}

	return(X11Viewport::Show());
}

void
Viewport::Tick(float)
{
	using namespace OpenGL;
	X11Viewport::ProcessX11Events();
}

void
Viewport::SwapBuffer(void)
{
	using namespace OpenGL;
	X11Viewport::SwapBuffer();
	Painter::Reset();
}

const Graphics::Display &
Viewport::Display(void)
{
	using namespace OpenGL;
	return(X11Viewport::dpy);
}

const Math::Size2f &
Viewport::Size(void)
{
	using namespace OpenGL;
	return(X11Viewport::vsize);
}

const Math::Size2i &
Viewport::WindowSize(void)
{
	using namespace OpenGL;
	return(X11Viewport::wsize);
}

const Core::Type &
Viewport::Type(void)
{
	static const Core::Type s_type("X11");
	return(s_type);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

