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

#include "graphics/backend_p.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <cassert>

#include <bcm_host.h>

#include "core/logger.h"
#include "core/platform.h"
#include "core/type.h"

#include "event/eventmanager.h"
#include "event/keyboardevent.h"
#include "event/quitevent.h"
#include "event/viewportevent.h"

#include "graphics/camera.h"
#include "graphics/color.h"
#include "graphics/display.h"
#include "graphics/painter_p.h"


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


/*
 * This is used to give the display time to settle.
 */
#define VC_DISPLAY_SETTLE_SECS 3.f

/*
 * VC Backend Notes
 *
 * The fullscreen flag in the Display parameters is ignored since we will always
 * be in fullscreen.
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */
namespace OpenGL { /****************************** Graphics::OpenGL Namespace */
namespace { /************************ Graphics::OpenGL::<anonymous> Namespace */

namespace VCBackend {

	inline bool Initialize(void);
	inline void Finalize(void);

	inline void Reset(int state);
	inline void Tick(float delta);

	inline bool Create(const Display &display);
	inline void Destroy(void);

	inline void SwapBuffer(void);

	inline bool CreateGLContext(void);
	inline void DestroyGLContext(void);

	inline bool CreateVCWindow(void);
	inline void DestroyVCWindow(void);

	inline bool PowerOnTVOutput(uint16_t &did);
	inline void PowerOffTVOutput(void);
	inline void HandleTVOutputCallback(void *, uint32_t, uint32_t, uint32_t);

	enum StateFlag
	{
		sfUninitialized = 0,

		sfReady         = (1 <<  0),
		sfVCInit        = (1 <<  1),
		sfVCDisplay     = (1 <<  2),
		sfVCWindow      = (1 <<  3),
		sfVCTV          = (1 <<  4),
		sfVCSettled     = (1 <<  5),

		sfGLDisplay     = (1 <<  6),
		sfGLSurface     = (1 <<  7),
		sfGLContext     = (1 <<  8),
		sfGLCurrent     = (1 <<  9),

		sfTVStandard    = (1 << 10),
		sfTVDVI         = (1 << 11),
		sfTVHDMI        = (1 << 12),

		sfReset         = (1 << 13),
		sfTerminated    = (1 << 14),
		sfVCValid       = sfVCInit|sfVCDisplay|sfVCWindow|sfVCTV,
		sfGLValid       = sfGLDisplay|sfGLSurface|sfGLContext|sfGLCurrent,
		sfTVStates      = sfTVStandard|sfTVDVI|sfTVHDMI,
		sfValid         = sfVCValid|sfGLValid,
		sfReadyValid    = sfReady|sfValid,
		sfActive        = sfReadyValid|sfVCSettled
	};

	/************************* MARSHMALLOW */
	Display                   dpy;
	Math::Size2i              wsize;
	Math::Size2f              vsize;
	int                       flags;

	/*************************** VIDEOCORE */
	DISPMANX_DISPLAY_HANDLE_T vc_dpy;
	EGL_DISPMANX_WINDOW_T     vc_window;
	float                     vc_stimer;

	/********************************* EGL */
	EGLDisplay                egl_dpy;
	EGLSurface                egl_surface;
	EGLContext                egl_ctx;

}

bool
VCBackend::Initialize(void)
{
	/* default display display */
	dpy.depth      = MARSHMALLOW_GRAPHICS_DEPTH;
	dpy.fullscreen = MARSHMALLOW_GRAPHICS_FULLSCREEN;
	dpy.height     = MARSHMALLOW_GRAPHICS_HEIGHT;
	dpy.vsync      = MARSHMALLOW_GRAPHICS_VSYNC;
	dpy.width      = MARSHMALLOW_GRAPHICS_WIDTH;

	Reset(sfUninitialized);

	/*
	 * Broadcom
	 */
	bcm_host_init();
	vc_tv_register_callback(VCBackend::HandleTVOutputCallback, 0);
	vc_tv_power_off();
	flags |= sfVCInit;

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
VCBackend::Finalize(void)
{
	/* set termination flag */
	flags |= sfTerminated;

	/*
	 * Destroy viewport if valid
	 */
	if (sfValid == (flags & sfValid))
		Destroy();

	/*
	 * Broadcom
	 */
	if (sfVCInit == (flags & sfVCInit)) {
		vc_tv_unregister_callback(VCBackend::HandleTVOutputCallback);
		bcm_host_deinit();
		flags ^= sfVCInit;
	}

	/* sanity check */
	assert(flags == sfTerminated && "We seem to have some stray flags!");

	flags = sfUninitialized;
}

void
VCBackend::Reset(int state)
{
	flags = state;
	vsize.zero();
	wsize.zero();

	vc_dpy = 0;
	memset(&vc_window, 0, sizeof(vc_window));
	vc_stimer = .0f;

	egl_dpy     = EGL_NO_DISPLAY;
	egl_surface = EGL_NO_SURFACE;
	egl_ctx     = EGL_NO_CONTEXT;
}

void
VCBackend::Tick(float delta)
{
	/* settle timer */
	if (sfValid == (flags & (sfValid|sfVCSettled))) {
		if ((vc_stimer += delta) >= VC_DISPLAY_SETTLE_SECS) {
			vc_stimer = .0f;
			flags |= sfVCSettled;
			MMDEBUG("VC: Display should be settled now.");
		}
	}
	else vc_stimer = .0f;

	/* reset viewport */
	if (sfReset == (flags & sfReset)) {
		MMDEBUG("Backend reset in progress!");
		flags ^= sfReset;
		Backend::Setup(dpy);
	}
}

bool
VCBackend::Create(const Display &display_)
{
	/*
	 * Check if already valid (no no)
	 */
	if (sfValid == (flags & sfValid))
		return(false);

	/* assign new display display */
	dpy = display_;

	/*
	 * Create VideoCore Window
	 */
	if (!CreateVCWindow()) {
		MMERROR("VC: Failed to create window.");
		return(false);
	}

	/*
	 * Create EGL Context
	 */
	if (!CreateGLContext()) {
		MMERROR("GL: Failed to create surface.");
		DestroyVCWindow();
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
		DestroyVCWindow();
		return(false);
	}

	/* viewport size */

#if MARSHMALLOW_GRAPHICS_LOCK_WIDTH
	vsize.width = static_cast<float>(dpy.width);
	vsize.height = (vsize.width * static_cast<float>(wsize.height)) /
	    static_cast<float>(wsize.width);
#else
	vsize.height = static_cast<float>(dpy.height);
	vsize.width = (vsize.height * static_cast<float>(wsize.width)) /
	    static_cast<float>(wsize.height);
#endif

	/* sub-systems */
	Camera::Update();
	Painter::Initialize();

	/* broadcast */
	Event::ViewportEvent l_event(Event::ViewportEvent::Created);
	Event::EventManager::Instance()->dispatch(l_event);

	return(true);
}

void
VCBackend::Destroy(void)
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
	DestroyVCWindow();

	/* sanity check */
	assert(0 == (flags & ~(sfVCInit|sfReady|sfTerminated))
	    && "We seem to have some stray flags!");

	Reset(flags & (sfVCInit|sfTerminated));
}

void
VCBackend::SwapBuffer(void)
{
	if (sfValid != (flags & sfValid))
		return;

	eglSwapBuffers(egl_dpy, egl_surface);
}

bool
VCBackend::CreateGLContext(void)
{
	/*
	 * Bind OpenGL API
	 */
	if (eglBindAPI(EGL_OPENGL_ES_API) != EGL_TRUE) {
		MMERROR("EGL: Failed to bind required OpenGL API.");
		return(false);
	}

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
	 * Choose EGL Config
	 */
	EGLint l_attr[] = {
		EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_BUFFER_SIZE,     dpy.depth,
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
	               (EGLNativeWindowType)&vc_window,
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
	 * Make Current EGL Context
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
VCBackend::DestroyGLContext(void)
{
	if (0 == (flags & sfGLValid))
		return;

	/* extensions */

	Extensions::Finalize();

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
		eglTerminate(egl_dpy), egl_dpy = EGL_NO_DISPLAY;
		flags ^= sfGLDisplay;
	}
	eglReleaseThread();
}

bool
VCBackend::CreateVCWindow(void)
{
	VC_RECT_T l_dst_rect;
	VC_RECT_T l_src_rect;

	wsize.set(dpy.width, dpy.height);

	l_src_rect.x = l_src_rect.y = 0;
	l_dst_rect.x = l_dst_rect.y = 0;
	l_dst_rect.width  = dpy.width;
	l_dst_rect.height = dpy.height;

	/* set SPD infoframe */
	vc_tv_hdmi_set_spd("MMGE", "MES", HDMI_SPD_TYPE_GAME);

	uint16_t l_display_type;
	if (!PowerOnTVOutput(l_display_type)) {
		MMERROR("VC: Failed to power on display.");
		return(false);
	}

	/* open display */

	vc_dpy = vc_dispmanx_display_open(l_display_type);
	if (0 == vc_dpy) {
		MMERROR("VC: Failed to open display.");
		return(false);
	}
	flags |= sfVCDisplay;

	DISPMANX_MODEINFO_T l_display_info;
	if (vc_dispmanx_display_get_info(vc_dpy, &l_display_info) != 0) {
		MMERROR("VC: Failed to get display info.");
		return(false);
	}

	/* calculate sizes */

	wsize.width  = l_dst_rect.width  = l_display_info.width;
	wsize.height = l_dst_rect.height = l_display_info.height;
	l_src_rect.width  = l_dst_rect.width  << 16;
	l_src_rect.height = l_dst_rect.height << 16;

	MMDEBUG("VC: Display size (" << wsize.width << "x" << wsize.height << ")");

	DISPMANX_UPDATE_HANDLE_T l_dispman_update = vc_dispmanx_update_start(0);
	if (l_dispman_update == DISPMANX_NO_HANDLE) {
		MMERROR("VC: Failed to get display update handle.");
		return(false);
	}

	/* add window element */

	vc_window.element = vc_dispmanx_element_add
	    (l_dispman_update,
	     vc_dpy,
	     0 /*layer*/,
	    &l_dst_rect,
	     0 /*src*/,
	    &l_src_rect,
	     DISPMANX_PROTECTION_NONE,
	     0 /*alpha*/,
	     0 /*clamp*/,
	     DISPMANX_NO_ROTATE /*transform*/);
	vc_dispmanx_update_submit_sync(l_dispman_update);
	flags |= sfVCWindow;

	vc_window.width  = l_dst_rect.width;
	vc_window.height = l_dst_rect.height;

	return(true);
}

void
VCBackend::DestroyVCWindow(void)
{
	/* remove window element */

	if (sfVCWindow == (flags & sfVCWindow)) {
		DISPMANX_UPDATE_HANDLE_T l_dispman_update = vc_dispmanx_update_start(0);
		if (l_dispman_update != DISPMANX_NO_HANDLE) {
			vc_dispmanx_element_remove(l_dispman_update, vc_window.element),
			    vc_window.element = 0;
			vc_dispmanx_update_submit_sync(l_dispman_update);
		}
		else MMERROR("VC: Display manager returned an invalid update "
		             "handle. IGNORING.");

		flags ^= sfVCWindow;
	}

	/* close display */

	if (sfVCDisplay == (flags & sfVCDisplay)) {
		vc_dispmanx_display_close(vc_dpy), vc_dpy = 0;
		flags ^= sfVCDisplay;
	}

	/* power off tv and hdmi ports */

	PowerOffTVOutput();
}

bool
VCBackend::PowerOnTVOutput(uint16_t &display_id)
{
	uint16_t l_display;

	/* Get current TV state */
	TV_GET_STATE_RESP_T l_tvstate;
	if (vc_tv_get_state(&l_tvstate) != 0) {
		MMERROR("HDMI: Failed to query initial tv state.");
		return(false);
	}

	MMDEBUG("VC: Got initial state=" << l_tvstate.state);

	/*
	 *  HDMI/DVI
	 */
	if (0 == (l_tvstate.state & VC_HDMI_UNPLUGGED)) {

		/* power on in preferred mode */
		if (0 != vc_tv_hdmi_power_on_preferred()) {
			MMERROR("HDMI: Failed to power on.");
			return(false);
		} else MMDEBUG("VC: HDMI was powered on.");
	}

	/*
	 * Standard TV
	 */
	else if (0 != vc_tv_sdtv_power_on(SDTV_MODE_NTSC, 0)) {
		MMERROR("SDTV: Failed to power on.");
		return(false);
	} else MMDEBUG("VC: SDTV was powered on.");

	/* update state */
	if (0 != vc_tv_get_state(&l_tvstate)) {
		MMERROR("VC: Failed to query TV state.");
		return(false);
	}

	/* check connection type */
	if ((l_tvstate.state & VC_HDMI_HDMI) == VC_HDMI_HDMI)
		l_display = DISPMANX_ID_HDMI;
	else if ((l_tvstate.state & VC_HDMI_DVI) == VC_HDMI_DVI)
		l_display = DISPMANX_ID_MAIN_LCD;
	else l_display = DISPMANX_ID_SDTV;

	/* sanity check */
	if (l_tvstate.width <= 0 || l_tvstate.height <= 0) {
		MMERROR("VC: Invalid TV size received.");
		return(false);
	}

	MMDEBUG("VC: Using #" << l_display << " display at "
	    "(" << l_tvstate.width << "x" << l_tvstate.height << ")" );

	/* set flags */

	switch (l_display) {
	case DISPMANX_ID_SDTV:     flags |= sfTVStandard; break;
	case DISPMANX_ID_HDMI:     flags |= sfTVHDMI; break;
	case DISPMANX_ID_MAIN_LCD: flags |= sfTVDVI; break;
	}
	flags |= sfVCTV;

	display_id = l_display;
	return(true);
}

void
VCBackend::PowerOffTVOutput(void)
{
	assert((flags & sfTVStates) != (flags & (sfVCTV|sfTVStates))
	    && "We seem to have some stray flags!");

	if (0 == (flags & sfVCTV))
		return;

	vc_tv_power_off();

	/* clear all TV states */
	flags &= ~(sfVCTV|sfTVStates|sfVCSettled);
}

void
VCBackend::HandleTVOutputCallback(void *, uint32_t reason, uint32_t, uint32_t)
{
	switch(reason) {

	case VC_SDTV_UNPLUGGED:
		MMDEBUG("VC_SDTV_UNPLUGGED");
		if ((sfReadyValid|sfTVStandard)
		    != (flags & (sfReadyValid|sfTVStandard)))
			break;

		MMDEBUG("Backend deactivated!");
		flags ^= sfReady;
		break;

	case VC_HDMI_UNPLUGGED:
		MMDEBUG("VC_HDMI_UNPLUGGED");
		if (0 == (flags & sfReadyValid)
		    || (0 == (flags & (sfTVDVI|sfTVHDMI))))
			break;

		MMDEBUG("Backend deactivated!");
		flags ^= sfReady;

		/* we fallback to StandardTV mode */
		MMDEBUG("Backend reset flagged!");
		flags |= sfReset;
		break;

	case VC_SDTV_STANDBY:
	case VC_HDMI_STANDBY:
		MMDEBUG((reason == VC_SDTV_STANDBY ?
		    "VC_SDTV_STANDBY" : "VC_HDMI_STANDBY"));
		/* ignore if busy */
		if (0 == (flags & sfValid))
			break;

		MMDEBUG("Backend reset flagged!");
		flags |= sfReset;
		break;

	case VC_SDTV_NTSC:
	case VC_SDTV_PAL:
		MMDEBUG("VC_SDTV_NTSC");
		if (0 == (flags & sfValid))
			break;

		MMDEBUG("Backend Activated");
		flags |= sfReady;
		break;

	case VC_HDMI_DVI:
	case VC_HDMI_HDMI:
		MMDEBUG((reason == VC_HDMI_DVI ?
		    "VC_HDMI_DVI" : "VC_HDMI_HDMI"));
		if (0 == (flags & sfValid))
			break;

		MMDEBUG("Backend Activated");
		flags |= sfReady;
		break;

	default: MMDEBUG("TVCB: Unknown!");
	}
}

} /********************************** Graphics::OpenGL::<anonymous> Namespace */

PFNPROC
glGetProcAddress(const char *f)
{
	return(eglGetProcAddress(f));
}

} /*********************************************** Graphics::OpenGL Namespace */

bool
Backend::Active(void)
{
	using namespace OpenGL::VCBackend;
	return(sfActive == (flags & sfActive));
}

bool
Backend::Initialize(void)
{
	using namespace OpenGL;
	return(VCBackend::Initialize());
}

void
Backend::Finalize(void)
{
	using namespace OpenGL;
	VCBackend::Finalize();
}

bool
Backend::Setup(const Graphics::Display &display)
{
	using namespace Core;
	using namespace OpenGL;

	static bool s_working = false;
	if (s_working)
		return(false);
	s_working = true;

	VCBackend::Destroy();

	if (!VCBackend::Create(display)) {
		VCBackend::Destroy();
		s_working = false;
		return(false);
	}

	s_working = false;
	return(true);
}

void
Backend::Tick(float delta)
{
	using namespace OpenGL;
	VCBackend::Tick(delta);
}

void
Backend::Finish(void)
{
	using namespace OpenGL;
	VCBackend::SwapBuffer();
	Painter::Reset();
}

const Graphics::Display &
Backend::Display(void)
{
	using namespace OpenGL;
	return(VCBackend::dpy);
}

const Math::Size2f &
Backend::Size(void)
{
	using namespace OpenGL;
	return(VCBackend::vsize);
}

const Math::Size2i &
Backend::WindowSize(void)
{
	using namespace OpenGL;
	return(VCBackend::wsize);
}

const Core::Identifier &
Backend::Implementation(void)
{
	static const Core::Identifier s_implementation("VC");
	return(s_implementation);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

