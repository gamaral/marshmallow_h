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

#include "graphics/painter.h"
#include "graphics/transform.h"

#include "bcm_host.h"

#include "headers.h"
#ifdef MARSHMALLOW_OPENGL_GLES2
#  include <EGL/egl.h>
#else
#  error EGL/GLES2 is disabled!
#endif
#include "extensions.h"

#include <cmath>
#include <cstring>
#include <list>

MARSHMALLOW_NAMESPACE_USE
using namespace Graphics::OpenGL;
using namespace Graphics;

/******************************************************************************/

namespace
{
	struct ViewportData {
		EGL_DISPMANX_WINDOW_T window;
		DISPMANX_ELEMENT_HANDLE_T dispman_element;
		DISPMANX_UPDATE_HANDLE_T  dispman_update;

		DISPMANX_DISPLAY_HANDLE_T display;

		Math::Size2i  wsize;
		int           screen;
		EGLContext    egl_context;
		EGLDisplay    egl_display;
		EGLSurface    egl_surface;

		Transform     camera;
		float         radius2;
		Math::Size2f  size;
		Math::Size2f  scaled_size;
		bool          fullscreen;
		bool          loaded;
	} s_data;

	void UpdateCamera(void);

	void InitializeViewport(void)
	{
		s_data.camera.setRotation(.0f);
		s_data.camera.setScale(Math::Size2f::Identity());
		s_data.camera.setTranslation(Math::Point2::Zero());

		s_data.egl_context = EGL_NO_CONTEXT;
		s_data.egl_display = EGL_NO_DISPLAY;
		s_data.egl_surface = EGL_NO_SURFACE;

		s_data.display = 0;
		s_data.dispman_element = 0;
		s_data.dispman_update = 0;
		s_data.fullscreen = false;
		s_data.loaded = false;
		s_data.screen = 0;
		s_data.size.zero();
		s_data.wsize[0] = s_data.wsize[1] = 0;
	}

	bool
	CreateSurface(uint16_t w, uint16_t h, uint8_t d, uint8_t r, bool, bool)
	{
		VC_RECT_T l_dst_rect;
		VC_RECT_T l_src_rect;

		s_data.egl_context = EGL_NO_CONTEXT;
		s_data.egl_display = EGL_NO_DISPLAY;
		s_data.egl_surface = EGL_NO_SURFACE;

		s_data.display   = 0;
		s_data.loaded    = false;
		s_data.screen    = 0;
		s_data.dispman_element = 0;
		s_data.dispman_update  = 0;

		s_data.fullscreen = true;
		s_data.wsize[0] = w;
		s_data.wsize[1] = h;

		/*** VC ***/

		l_src_rect.x = l_src_rect.y = 0;
		l_dst_rect.x = l_dst_rect.y = 0;
		l_dst_rect.width  = w;
		l_dst_rect.height = h;

		/*
		 * Try to use display size, if not use default window width and
		 * height.
		 */
		uint16_t l_display = DISPMANX_ID_SDTV; /* default to tv output */

		/*
		 * Power on HDMI port in stand-by mode (for state testing)
		 */
		vc_tv_hdmi_power_on_explicit(HDMI_MODE_HDMI,
		                             HDMI_RES_GROUP_CEA,
		                             HDMI_CEA_OFF);

		TV_GET_STATE_RESP_T l_tvstate;
		if (vc_tv_get_state(&l_tvstate) >= 0) {
			MMINFO("VC: state=" << l_tvstate.state);

			/*
			 *  HDMI/DVI
			 */
			if ((l_tvstate.state & VC_HDMI_UNPLUGGED) == 0) {
				/* switch to stand-by */
				if ((l_tvstate.state & VC_HDMI_STANDBY) == 0)
					vc_tv_power_off();

				/* turn on in correct mode */
				vc_tv_hdmi_power_on_preferred();

				/* update state */
				if (vc_tv_get_state(&l_tvstate) < 0) {
					MMERROR("HDMI: Failed to query tv state.");
					return(false);
				}

				/* check connection type */
				if ((l_tvstate.state & VC_HDMI_HDMI) == VC_HDMI_HDMI)
					l_display = DISPMANX_ID_HDMI;
				else if ((l_tvstate.state & VC_HDMI_DVI) == VC_HDMI_DVI)
					l_display = DISPMANX_ID_MAIN_LCD;

			}

			/*
			 *  SDTV
			 */
			else {
				/* make sure HDMI port is off */
				MMINFO("SDTV: Turning off HDMI port");
				vc_tv_hdmi_power_on_explicit(HDMI_MODE_OFF,
				                             HDMI_RES_GROUP_CEA,
				                             HDMI_CEA_OFF);
			}

			if (l_tvstate.width <= 0 || l_tvstate.height <= 0) {
				MMERROR("VC: Invalid TV size returned.");
				return(false);
			}
		}

		s_data.display = vc_dispmanx_display_open(l_display);
		if (s_data.display == 0) {
			MMERROR("VC: Failed to open display.");
			return(false);
		}

		DISPMANX_MODEINFO_T l_display_info;
		if (vc_dispmanx_display_get_info(s_data.display, &l_display_info) != 0) {
			MMERROR("VC: Failed to get display info.");
			return(false);
		}

		/* recalculate sizes */
		s_data.wsize[0]   = l_dst_rect.width  = l_display_info.width;
		s_data.wsize[1]   = l_dst_rect.height = l_display_info.height;
		l_src_rect.width  = l_dst_rect.width  << 16;
		l_src_rect.height = l_dst_rect.height << 16;
		MMINFO("VC: Display size (" << s_data.wsize[0] << "x" << s_data.wsize[1] << ")");

		s_data.dispman_update  = vc_dispmanx_update_start(0);
		s_data.dispman_element = vc_dispmanx_element_add
		    (s_data.dispman_update,
		     s_data.display,
		     0 /*layer*/,
		    &l_dst_rect,
		     0 /*src*/,
		    &l_src_rect,
		     DISPMANX_PROTECTION_NONE,
		     0 /*alpha*/,
		     0 /*clamp*/,
		     DISPMANX_NO_ROTATE /*transform*/);

		s_data.window.element = s_data.dispman_element;
		s_data.window.width   = l_dst_rect.width;
		s_data.window.height  = l_dst_rect.height;
		vc_dispmanx_update_submit_sync(s_data.dispman_update);

		/*** GL ***/

		s_data.egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		if (s_data.egl_display == EGL_NO_DISPLAY) {
			MMERROR("EGL: No display was found. EGLERROR=" << eglGetError());
			return(false);
		}
		
		if (!eglInitialize(s_data.egl_display, 0, 0)) {
			MMERROR("EGL: Initialization failed.");
			return(false);
		}

		/* color sizes */
		EGLint l_color_sizes[3];
		switch (l_display_info.input_format) {
		case VCOS_DISPLAY_INPUT_FORMAT_RGB888:
			l_color_sizes[0] = l_color_sizes[1] = l_color_sizes [2] = 8;
			break;
		case VCOS_DISPLAY_INPUT_FORMAT_RGB565:
			l_color_sizes[1] = 6;
			l_color_sizes[0] = l_color_sizes[2] = 5;
			break;
		default:
			l_color_sizes[0] = l_color_sizes[1] = l_color_sizes [2] = 0;
		}

		EGLint l_attr[] = {
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
			EGL_RED_SIZE,        l_color_sizes[0],
			EGL_GREEN_SIZE,      l_color_sizes[1],
			EGL_BLUE_SIZE,       l_color_sizes[2],
			EGL_NONE,            EGL_NONE
		};

		EGLint l_config_count;
		EGLConfig l_config;
		if (!eglChooseConfig(s_data.egl_display, l_attr, &l_config, 1, &l_config_count))
			MMERROR("EGL: No config was chosen. EGLERROR=" << eglGetError());

		s_data.egl_surface = eglCreateWindowSurface(s_data.egl_display, l_config, (EGLNativeWindowType) &s_data.window, 0);
		if (s_data.egl_surface == EGL_NO_SURFACE) {
			MMERROR("EGL: No surface was created. EGLERROR=" << eglGetError());
			return(false);
		}

		EGLint l_ctxattr[] = {
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE, EGL_NONE
		};
		s_data.egl_context = eglCreateContext(s_data.egl_display, l_config, EGL_NO_CONTEXT, l_ctxattr);
		if (s_data.egl_context == EGL_NO_CONTEXT) {
			MMERROR("EGL: No context was created. EGLERROR=" << eglGetError());
			return(false);
		}
		
		if (!eglMakeCurrent(s_data.egl_display, s_data.egl_surface, s_data.egl_surface, s_data.egl_context)) {
			MMERROR("EGL: Failed to switch current context. EGLERROR=" << eglGetError());
			return(false);
		}

		/* extensions */

		InitializeExtensions(eglQueryString(s_data.egl_display, EGL_EXTENSIONS));

		/* initialize context */

		glViewport(0, 0, s_data.wsize[0], s_data.wsize[1]);

		if(glGetError() != GL_NO_ERROR) {
			MMERROR("GL: Failed during initialization.");
			return(false);
		}

		/* set viewport size */

#if MARSHMALLOW_VIEWPORT_LOCK_WIDTH
		s_data.size[0] = MARSHMALLOW_VIEWPORT_WIDTH;
		s_data.size[1] = static_cast<float>(s_data.wsize[1]) *
		    (MARSHMALLOW_VIEWPORT_WIDTH / static_cast<float>(s_data.wsize[0]));
#else
		s_data.size[0] = static_cast<float>(s_data.wsize[0]) *
		    (MARSHMALLOW_VIEWPORT_HEIGHT / static_cast<float>(s_data.wsize[1]));
		s_data.size[1] = MARSHMALLOW_VIEWPORT_HEIGHT;
#endif

		Viewport::SetCamera(s_data.camera);

		return(s_data.loaded = true);
	}

	void
	DestroySurface(void)
	{
		if (s_data.display) {
			eglMakeCurrent(s_data.egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
			eglDestroyContext(s_data.egl_display, s_data.egl_context);
			eglDestroySurface(s_data.egl_display, s_data.egl_surface);
			eglTerminate(s_data.egl_display);

			if (s_data.display > 0) {
				vc_dispmanx_element_remove(s_data.dispman_update, s_data.dispman_element);
				vc_dispmanx_display_close(s_data.display);
			}
		}

		s_data.egl_context = EGL_NO_CONTEXT;
		s_data.egl_display = EGL_NO_DISPLAY;
		s_data.egl_surface = EGL_NO_SURFACE;

		s_data.display = 0;
		s_data.loaded = false;
		s_data.screen = 0;

		/* power off */
		vc_tv_hdmi_power_on_explicit(HDMI_MODE_OFF,
		                             HDMI_RES_GROUP_CEA,
		                             HDMI_CEA_OFF);
		vc_tv_power_off();
	}

	void
	UpdateCamera(void)
	{
		/* calculate scaled viewport size */
		s_data.scaled_size[0] = s_data.size[0] / s_data.camera.scale().width();
		s_data.scaled_size[1] = s_data.size[1] / s_data.camera.scale().height();

		/* calculate magnitude and pass it off as radius squared */
		s_data.radius2 = powf(s_data.scaled_size[0] / 2.f, 2.f) +
		                 powf(s_data.scaled_size[1] / 2.f, 2.f);
	}
} // namespace

/***************************************************************** Extensions */

PFNPROC
OpenGL::glGetProcAddress(const char *f)
{
	return(eglGetProcAddress(f));
}

/******************************************************************************/

bool
Viewport::Initialize(uint16_t w, uint16_t h, uint8_t d, uint8_t r, bool f, bool v)
{
	bcm_host_init();

	InitializeViewport();

	if (!CreateSurface(w, h, d, r, f, v)) {
		DestroySurface();
		return(false);
	}

	Painter::Initialize();
	return(true);
}

void
Viewport::Finalize(void)
{
	Painter::Finalize();
	DestroySurface();

	bcm_host_deinit();
}

bool
Viewport::Redisplay(uint16_t w, uint16_t h, uint8_t d, uint8_t r, bool f, bool v)
{
	DestroySurface();

	if(!CreateSurface(w, h, d, r, f, v)) {
		DestroySurface();
		return(false);
	}
	return(true);
}

void
Viewport::Tick(void)
{
	/* TODO */
}

void
Viewport::SwapBuffer(void)
{
	eglSwapBuffers(s_data.egl_display, s_data.egl_surface);
	Painter::Reset();
}

const Graphics::Transform &
Viewport::Camera(void)
{
	return(s_data.camera);
}

void
Viewport::SetCamera(const Graphics::Transform &camera)
{
	s_data.camera = camera;
	UpdateCamera();
}

float
Viewport::Radius2(void)
{
	return(s_data.radius2);
}

const Math::Size2f &
Viewport::ScaledSize(void)
{
	return(s_data.scaled_size);
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
	static const Core::Type s_type("VC");
	return(s_type);
}

