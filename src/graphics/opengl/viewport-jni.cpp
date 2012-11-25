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

#include <jni.h>
#include <android/log.h>

#include <cassert>
#include <cstring>

#include "core/logger.h"
#include "core/type.h"

#include "event/eventmanager.h"
#include "event/quitevent.h"
#include "event/viewportevent.h"

#ifdef MARSHMALLOW_INPUT_LINUX_JNI
#  include "input/linux/jni.h"
#endif

#include "graphics/camera.h"
#include "graphics/color.h"
#include "graphics/display.h"
#include "graphics/painter_p.h"

#include "jni/android_native_app_glue.h"

#include "headers.h"
#ifdef MARSHMALLOW_OPENGL_EGL
#  include <EGL/egl.h>
#else
#  error Building without EGL support!
#endif
#ifndef MARSHMALLOW_OPENGL_ES2
#  error Building without OpenGL ES2 support!
#endif
#include "extensions.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

/*
 * JNI Viewport Notes
 *
 */

extern int MMain(int argc, char *argv[]);

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */
namespace OpenGL { /****************************** Graphics::OpenGL Namespace */
namespace { /************************ Graphics::OpenGL::<anonymous> Namespace */

namespace JNIViewport {

	inline bool Initialize(void);
	inline void Finalize(void);

	inline void Reset(int state);

	inline bool Create(const Display &display);
	inline void Destroy(void);

	inline void SwapBuffer(void);

	inline bool CreateGLContext(void);
	inline void DestroyGLContext(void);

	inline void ProcessJNIEvents(void);

	void HandleJNICommands(struct android_app *app, int32_t cmd);
	int32_t HandleJNIInputEvents(struct android_app *app, AInputEvent *event);

	enum StateFlag
	{
		sfUninitialized = 0,
		sfReady         = (1 << 0),
		sfGLDisplay     = (1 << 1),
		sfGLSurface     = (1 << 2),
		sfGLContext     = (1 << 3),
		sfGLCurrent     = (1 << 4),
		sfFocused       = (1 << 5),
		sfTerminated    = (1 << 6),
		sfGLValid       = sfGLDisplay|sfGLSurface|sfGLContext|sfGLCurrent,
		sfValid         = sfGLValid,
		sfActive        = sfReady|sfValid|sfFocused
	};

	/******************* MARSHMALLOW */
	Display             dpy;
	Math::Size2i        wsize;
	Math::Size2f        vsize;
	int                 flags;

	/*************************** JNI */
	struct android_app *s_android_app;

	/*************************** EGL */
	EGLDisplay          egl_dpy;
	EGLSurface          egl_surface;
	EGLContext          egl_ctx;
}

bool
JNIViewport::Initialize(void)
{
	/* default display display */
	dpy.depth      = MARSHMALLOW_VIEWPORT_DEPTH;
	dpy.fullscreen = MARSHMALLOW_VIEWPORT_FULLSCREEN;
	dpy.height     = MARSHMALLOW_VIEWPORT_HEIGHT;
	dpy.vsync      = MARSHMALLOW_VIEWPORT_VSYNC;
	dpy.width      = MARSHMALLOW_VIEWPORT_WIDTH;

	Reset(sfUninitialized);

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
JNIViewport::Finalize(void)
{
	/* set termination flag */
	flags |= sfTerminated;

	/*
	 * Destroy viewport if valid
	 */
	if (sfValid == (flags & sfValid))
		Destroy();

	/* sanity check */
	assert(flags == sfTerminated && "We seem to have some stray flags!");

	flags = sfUninitialized;
}

void
JNIViewport::Reset(int state)
{
	flags = state;
	vsize.zero();
	wsize.zero();

	if (state == sfUninitialized) {
		egl_dpy     = EGL_NO_DISPLAY;
		egl_surface = EGL_NO_SURFACE;
		egl_ctx     = EGL_NO_CONTEXT;
	}
}

bool
JNIViewport::Create(const Display &display)
{
	/*
	 * Check if already valid (no no)
	 */
	if (sfValid == (flags & sfValid))
		return(false);

	/* assign new display display */
	dpy = display;

	/*
	 * Create EGL Context
	 */
	if (!CreateGLContext()) {
		MMERROR("GL: Failed to create surface.");
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
JNIViewport::Destroy(void)
{
	/* check for valid state */
	if (sfValid != (flags & sfValid))
		return;

	/* deactivate */

	flags &= ~(sfReady);

	/* broadcast */

	if (0 == (flags & sfTerminated)) {
		Event::ViewportEvent l_event(Event::ViewportEvent::Destroyed);
		Event::EventManager::Instance()->dispatch(l_event);
	}

	Painter::Finalize();

	DestroyGLContext();

	/* sanity check */
	assert(0 == (flags & ~(sfTerminated))
	    && "We seem to have some stray flags!");

	Reset(flags & sfTerminated);
}

void
JNIViewport::SwapBuffer(void)
{
	assert(sfValid == (flags & sfValid)
	    && "Attempted to swap buffer with an invalid viewport");
	eglSwapBuffers(egl_dpy, egl_surface);
}

bool
JNIViewport::CreateGLContext(void)
{
	/*
	 * Open EGL Display
	 */
	egl_dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (egl_dpy == EGL_NO_DISPLAY) {
		MMERROR("EGL: No display was found.");
		return(false);
	}
	if (eglInitialize(egl_dpy, 0, 0) == EGL_FALSE) {
		MMERROR("EGL: Initialization failed.");
		return(false);
	}
	flags |= sfGLDisplay;
	
	/*
	 * Bind OpenGL API
	 */
	const EGLBoolean l_bind_result =
	    eglBindAPI(EGL_OPENGL_ES_API);
	if (l_bind_result != EGL_TRUE) {
		MMERROR("EGL: Failed to bind required OpenGL API.");
		return(false);
	}

	/*
	 * Choose EGL Config
	 */
	EGLint l_attr[] = {
		EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_BUFFER_SIZE,     dpy.depth,
		EGL_DEPTH_SIZE,      0,
		EGL_RED_SIZE,        8,
		EGL_GREEN_SIZE,      8,
		EGL_BLUE_SIZE,       8,
		EGL_ALPHA_SIZE,      0,
		EGL_NONE,            EGL_NONE
	};
	EGLint l_config_count;
	EGLConfig l_config;
	if (!eglChooseConfig(egl_dpy, l_attr, &l_config, 1, &l_config_count))
		MMERROR("EGL: No config was chosen. EGLERROR=" << eglGetError());

	EGLint l_format;
	eglGetConfigAttrib(egl_dpy, l_config, EGL_NATIVE_VISUAL_ID, &l_format);
	ANativeWindow_setBuffersGeometry(s_android_app->window, 0, 0, l_format);

	/*
	 * Create EGL Surface
	 */
	egl_surface = eglCreateWindowSurface(egl_dpy,
	                                     l_config,
	                (EGLNativeWindowType)s_android_app->window,
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
		EGL_CONTEXT_CLIENT_VERSION, 2,
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

	/*
	 * Get EGL context dimensions
	 */
	eglQuerySurface(egl_dpy, egl_surface, EGL_WIDTH,  &wsize.width);
	eglQuerySurface(egl_dpy, egl_surface, EGL_HEIGHT, &wsize.height);

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
JNIViewport::DestroyGLContext(void)
{
	/* extensions */

	Extensions::Finalize();

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

	/*
	 * Close EGL Display
	 */
	if (sfGLDisplay == (flags & sfGLDisplay)) {
		eglTerminate(egl_dpy), egl_dpy = 0;
		flags ^= sfGLDisplay;
	}
}

void
JNIViewport::ProcessJNIEvents(void)
{
	LOGW("TICK!!!");

	int l_ident;
	int l_events;
	struct android_poll_source *l_source;

	while ((l_ident = ALooper_pollAll(-1, NULL, &l_events, (void**)&l_source)) >= 0) {
		if (l_source != NULL)
			l_source->process(s_android_app, l_source);

		/* check if we are exiting */
		if (s_android_app->destroyRequested != 0) {
			Event::QuitEvent l_mmevent(-1);
			Event::EventManager::Instance()->dispatch(l_mmevent);
			break;
		}
	}
}

void
JNIViewport::HandleJNICommands(struct android_app *app, int32_t cmd)
{
	switch (cmd) {
	case APP_CMD_SAVE_STATE:
		// TODO
		break;
	case APP_CMD_INIT_WINDOW:
		if (s_android_app->window != NULL)
			Create(dpy);
		break;
	case APP_CMD_TERM_WINDOW:
			Destroy();
		break;
	case APP_CMD_GAINED_FOCUS:
		flags |= sfFocused;
		break;
	case APP_CMD_LOST_FOCUS:
		flags ^= sfFocused;
		break;
	}
}

int32_t
JNIViewport::HandleJNIInputEvents(struct android_app *app, AInputEvent *event)
{
    return(0);
}

extern "C" {
void
android_main(struct android_app* state)
{
    app_dummy();

    state->userData = &JNIViewport::dpy;
    state->onAppCmd = JNIViewport::HandleJNICommands;
    state->onInputEvent = JNIViewport::HandleJNIInputEvents;

    JNIViewport::s_android_app = state;

    char *l_argv[1] = {0};
    MMain(0, l_argv);
}}

} /********************************** Graphics::OpenGL::<anonymous> Namespace */

PFNPROC
glGetProcAddress(const char *f)
{
	return(eglGetProcAddress(f));
}

} /*********************************************** Graphics::OpenGL Namespace */

bool
Viewport::Active(void)
{
	using namespace OpenGL::JNIViewport;
	return(sfActive == (flags & sfActive));
}

bool
Viewport::Initialize(void)
{
	using namespace OpenGL;
	return(JNIViewport::Initialize());
}

void
Viewport::Finalize(void)
{
	using namespace OpenGL;
	JNIViewport::Finalize();
}

bool
Viewport::Setup(const Graphics::Display &display)
{
	using namespace OpenGL;
	JNIViewport::dpy = display;
	return(true);
}

void
Viewport::Tick(float)
{
	using namespace OpenGL;
	JNIViewport::ProcessJNIEvents();
}

void
Viewport::SwapBuffer(void)
{
	using namespace OpenGL;
	JNIViewport::SwapBuffer();
	Painter::Reset();
}

const Graphics::Display &
Viewport::Display(void)
{
	using namespace OpenGL;
	return(JNIViewport::dpy);
}

const Math::Size2f &
Viewport::Size(void)
{
	using namespace OpenGL;
	return(JNIViewport::vsize);
}

const Math::Size2i &
Viewport::WindowSize(void)
{
	using namespace OpenGL;
	return(JNIViewport::wsize);
}

const Core::Type &
Viewport::Type(void)
{
	static const Core::Type s_type("JNI");
	return(s_type);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

