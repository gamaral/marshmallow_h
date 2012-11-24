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

#include "core/logger.h"
#include "core/type.h"

#include "event/eventmanager.h"
#include "event/quitevent.h"
#include "event/viewportevent.h"

#ifdef MARSHMALLOW_INPUT_W32_SYSTEM
#  include "input/w32/system.h"
#endif

#include "graphics/camera.h"
#include "graphics/color.h"
#include "graphics/display.h"
#include "graphics/painter_p.h"

#include "headers.h"
#include "extensions.h"

#define W32_CLASS_NAME "marshmallow_wgl"

/*
 * WGL Viewport Notes
 *
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */
namespace OpenGL { /****************************** Graphics::OpenGL Namespace */
namespace { /************************ Graphics::OpenGL::<anonymous> Namespace */

namespace WGLViewport {

	inline bool Initialize(void);
	inline void Finalize(void);

	inline void Reset(int state);

	inline bool Create(const Display &display);
	inline void Destroy(void);

	inline bool Show(void);
	inline void Hide(void);

	inline void SwapBuffer(void);

	inline bool CreateWGLContext(void);
	inline void DestroyWGLContext(void);

	inline bool CreateW32Window(void);
	inline void DestroyW32Window(void);
	LRESULT CALLBACK ProcessW32Events(HWND hwnd, UINT uMsg,
	                                  WPARAM wParam, LPARAM lParam);

	enum StateFlag
	{
		sfUninitialized = 0,
		sfReady         = (1 << 0),
		sfW32Class      = (1 << 1),
		sfW32Window     = (1 << 2),
		sfGLContext     = (1 << 3),
		sfGLCurrent     = (1 << 4),
		sfExposed       = (1 << 5),
		sfFocused       = (1 << 6),
		sfTerminated    = (1 << 7),
		sfW32Valid      = sfW32Class|sfW32Window,
		sfGLValid       = sfGLContext|sfGLCurrent,
		sfValid         = sfW32Valid|sfGLValid,
		sfActive        = sfReady|sfValid|sfExposed|sfFocused
	};

	/******************* MARSHMALLOW */
	Display             dpy;
	Math::Size2i        wsize;
	Math::Size2f        vsize;
	int                 flags;

	/*************************** W32 */
	HDC                 w32_ctx;
	HWND                w32_window;

	/*************************** WGL */
	HGLRC               wgl_ctx;

}

bool
WGLViewport::Initialize(void)
{
	/* default display display */
	dpy.depth      = MARSHMALLOW_VIEWPORT_DEPTH;
	dpy.fullscreen = MARSHMALLOW_VIEWPORT_FULLSCREEN;
	dpy.height     = MARSHMALLOW_VIEWPORT_HEIGHT;
	dpy.vsync      = MARSHMALLOW_VIEWPORT_VSYNC;
	dpy.width      = MARSHMALLOW_VIEWPORT_WIDTH;

	Reset(sfUninitialized);

	/*
	 * Register window class
	 */
	WNDCLASS l_wc;
	l_wc.style         = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
	l_wc.lpfnWndProc   = ProcessW32Events;
	l_wc.cbClsExtra    = 0;
	l_wc.cbWndExtra    = 0;
	l_wc.hInstance     = GetModuleHandle(0);
	l_wc.hIcon         = 0;
	l_wc.hCursor       = LoadCursor(0, IDC_ARROW);
	l_wc.hbrBackground = 0;
	l_wc.lpszMenuName  = 0;
	l_wc.lpszClassName = W32_CLASS_NAME;

	if (!(RegisterClass(&l_wc))) {
		MMERROR("Failed to register window class.");
		return(false);
	}
	flags |= sfW32Class;

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
WGLViewport::Finalize(void)
{
	/* set termination flag */
	flags |= sfTerminated;

	/*
	 * Destroy viewport if valid
	 */
	if (sfValid == (flags & sfValid))
		Destroy();

	/*
	 * Unregister window class
	 */
	if (!UnregisterClass(W32_CLASS_NAME, GetModuleHandle(0)))
		MMWARNING("Failed to unregister window class");
	flags &= ~(sfW32Class);

	/* sanity check */
	assert(flags == sfTerminated && "We seem to have some stray flags!");

	flags = sfUninitialized;
}

void
WGLViewport::Reset(int state)
{
	flags = state;
	vsize.zero();
	wsize.zero();

	if (state == sfUninitialized) {
		w32_ctx = 0;
		w32_window = 0;
		wgl_ctx = 0;
	}

	/* sanity check */
	else {
		assert(0 == w32_ctx && 0 == w32_window
		    && "[W32] Viewport didn't get destroyed cleanly!");
		assert(0 == wgl_ctx
		    && "[WGL] Viewport didn't get destroyed cleanly!");
	}
}

bool
WGLViewport::Create(const Display &display)
{
	/*
	 * Check if already valid (no no)
	 */
	if (sfValid == (flags & sfValid))
		return(false);

	/* assign new display display */
	dpy = display;

	/*
	 * Create Window
	 */
	if (!CreateW32Window()) {
		MMERROR("WGL: Failed to create window.");
		return(false);
	}

	/*
	 * Create WGL Context
	 */
	if (!CreateWGLContext()) {
		MMERROR("WGL: Failed to create surface.");
		DestroyW32Window();
		return(false);
	}

	/* sanity check */
	assert(sfValid == (flags & sfValid)
	    && "Valid viewport was expected!");

	/* initialize context */

	glViewport(0, 0, wsize.width, wsize.height);

	if (glGetError() != GL_NO_ERROR) {
		MMERROR("WGL: Failed during initialization.");
		DestroyWGLContext();
		DestroyW32Window();
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

	/* broadcast */

	Event::ViewportEvent l_event(Event::ViewportEvent::Created);
	Event::EventManager::Instance()->dispatch(l_event);

	return(true);
}

void
WGLViewport::Destroy(void)
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

	DestroyWGLContext();
	DestroyW32Window();

	/* sanity check */
	assert(0 == (flags & ~(sfTerminated|sfW32Class))
	    && "We seem to have some stray flags!");

	Reset(flags & (sfTerminated|sfW32Class));
}

bool
WGLViewport::Show(void)
{
	/* sanity checks */

	if (sfValid != (flags & sfValid)) {
	    assert(false && "Valid viewport was expected!");
	    return(false);
	}

	if (sfExposed == (flags & sfExposed))
	    return(true);

	ShowWindow(w32_window, SW_SHOW);
	SetForegroundWindow(w32_window);
	SetActiveWindow(w32_window);
	SetFocus(w32_window);
	ShowCursor(!dpy.fullscreen);

	flags |= sfExposed;
	return(true);
}

void
WGLViewport::Hide(void)
{
	/* sanity checks */

	if (sfValid != (flags & sfValid)) {
		assert(false && "Valid viewport was expected!");
		return;
	}

	if (0 == (flags & sfExposed))
		return;

	ShowCursor(dpy.fullscreen);
	ShowWindow(w32_window, SW_HIDE);

	flags &= ~(sfExposed);
}

void
WGLViewport::SwapBuffer(void)
{
	assert(sfValid == (flags & sfValid)
	    && "Attempted to swap buffer with an invalid viewport");

	SwapBuffers(w32_ctx);
}

bool
WGLViewport::CreateWGLContext(void)
{
	if (!(wgl_ctx = wglCreateContext(w32_ctx))) {
		MMERROR("WGL: Failed to create context!");
		return(false);
	}
	flags |= sfGLContext;

	if (!wglMakeCurrent(w32_ctx, wgl_ctx)) {
		MMERROR("WGL: Failed to set current context");
		return(false);
	}
	flags |= sfGLCurrent;

	/* extensions */

	Extensions::Initialize();
	
	/* vsync */

	if (Extensions::wglSwapIntervalEXT) {
		Extensions::wglSwapIntervalEXT(dpy.vsync);
		if (Extensions::wglGetSwapIntervalEXT
		    && dpy.vsync != Extensions::wglGetSwapIntervalEXT())
			MMERROR("WGL: Swap interval request was ignored!");
	}

	return(true);
}

void
WGLViewport::DestroyWGLContext(void)
{
	if (0 == (flags & sfGLValid))
		return;

	if (sfGLCurrent == (flags & sfGLCurrent)) {
		wglMakeCurrent(0, 0);
		flags ^= sfGLCurrent;
	}

	if (sfGLContext == (flags & sfGLContext)) {
		wglDeleteContext(wgl_ctx), wgl_ctx = 0;
		flags ^= sfGLContext;
	}
}

bool
WGLViewport::CreateW32Window(void)
{
	int l_warea[2];
	RECT l_wrect;

	/* get desktop size */
	l_warea[0] = GetSystemMetrics(SM_CXSCREEN);
	l_warea[1] = GetSystemMetrics(SM_CYSCREEN);

	DWORD l_wstyle[2];
	l_wstyle[0] = WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
	l_wstyle[1] = WS_EX_APPWINDOW;

	/*
	 * ** FULLSCREEN MODE **
	 */
	if (dpy.fullscreen) {
		l_wstyle[0] |= WS_POPUP;
		l_wstyle[1] |= WS_EX_TOPMOST;
		wsize.set(l_warea[0], l_warea[1]);
		l_wrect.left = l_wrect.top = 0;
		l_wrect.right = l_warea[0];
		l_wrect.bottom = l_warea[1];
	}

	/*
	 * ** WINDOW MODE **
	 */
	else {
		l_wstyle[0] |= WS_OVERLAPPEDWINDOW;
		l_wstyle[1] |= WS_EX_WINDOWEDGE;
		wsize.set(dpy.width, dpy.height);
		l_wrect.left   = (l_warea[0] - dpy.width) / 2;
		l_wrect.right  = l_wrect.left + dpy.width;
		l_wrect.top    = (l_warea[1] - dpy.height) / 2;
		l_wrect.bottom = l_wrect.top  + dpy.height;
	}

	AdjustWindowRectEx(&l_wrect, l_wstyle[0], false, l_wstyle[1]);

	/* create actual window */
	w32_window =
	    CreateWindowEx(l_wstyle[1], W32_CLASS_NAME,
	                   MARSHMALLOW_VIEWPORT_TITLE,
	                   l_wstyle[0],
	                   l_wrect.left, l_wrect.top,
	                   l_wrect.right  - l_wrect.left,
	                   l_wrect.bottom - l_wrect.top,
	                   0, 0, GetModuleHandle(0), 0);
	if (!w32_window) {
		MMERROR("W32: Failed to create window");
		return(false);
	}

	/* create device context */
	if (!(w32_ctx = GetDC(w32_window))) {
		DestroyWindow(w32_window), w32_window = 0;
		MMERROR("W32: Failed to create device context");
		return(false);
	}

	/* select pixel format */
	static PIXELFORMATDESCRIPTOR l_pfd;
	memset(&l_pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	l_pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
	l_pfd.nVersion   = 1;
	l_pfd.dwFlags    = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
	l_pfd.iPixelType = PFD_TYPE_RGBA;
	l_pfd.cColorBits = dpy.depth;
	l_pfd.cRedBits   = 0;
	l_pfd.cGreenBits = 0;
	l_pfd.cBlueBits  = 0;
	l_pfd.cAlphaBits = 0;
	l_pfd.iLayerType = PFD_MAIN_PLANE;

	unsigned int l_pfindex;
	if (!(l_pfindex = ChoosePixelFormat(w32_ctx, &l_pfd))) {
		ReleaseDC(w32_window, w32_ctx), w32_ctx = 0;
		DestroyWindow(w32_window), w32_window = 0;
		MMERROR("W32: Failed to select a pixel format");
		return(false);
	}

	if (!SetPixelFormat(w32_ctx, l_pfindex, &l_pfd)) {
		ReleaseDC(w32_window, w32_ctx), w32_ctx = 0;
		DestroyWindow(w32_window), w32_window = 0;
		MMERROR("W32: Failed to bind pixel format to window context");
		return(false);
	}

	flags |= sfW32Window;
	return(true);
}

void
WGLViewport::DestroyW32Window(void)
{
	ReleaseDC(w32_window, w32_ctx), w32_ctx = 0;
	DestroyWindow(w32_window), w32_window = 0;
	flags &= ~(sfW32Window);
}

LRESULT CALLBACK
WGLViewport::ProcessW32Events(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {

	case WM_SIZE:
	return(0);

	case WM_SETFOCUS:
		flags |= sfFocused;
	return(0);

	case WM_KILLFOCUS:
		flags &= ~(sfFocused);
	return(0);

	case WM_ACTIVATE:
		if (wParam != WA_INACTIVE)
			flags |= sfReady;
		else
			flags &= ~(sfReady);
	return(0);

	case WM_SYSCOMMAND:
		if (dpy.fullscreen
		    && (SC_MONITORPOWER == wParam || SC_SCREENSAVE == wParam))
			return(0);
	break;

	case WM_CLOSE:
		PostQuitMessage(0);
	return(0);

#ifdef MARSHMALLOW_INPUT_W32_SYSTEM
	case WM_KEYDOWN:
	case WM_KEYUP:
		if (Input::W32::System::HandleEvent(uMsg, wParam, lParam))
			return(0);
#endif
	}

	return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}

} /********************************** Graphics::OpenGL::<anonymous> Namespace */

OpenGL::PFNPROC
OpenGL::glGetProcAddress(const char *f)
{
	return(reinterpret_cast<PFNPROC>(wglGetProcAddress(f)));
}

} /*********************************************** Graphics::OpenGL Namespace */

bool
Viewport::Active(void)
{
	using namespace OpenGL::WGLViewport;
	return(sfActive == (flags & sfActive));
}

bool
Viewport::Initialize(void)
{
	using namespace OpenGL;
	return(WGLViewport::Initialize());
}

void
Viewport::Finalize(void)
{
	using namespace OpenGL;
	WGLViewport::Finalize();
}

bool
Viewport::Setup(const Graphics::Display &display)
{
	using namespace OpenGL;

	WGLViewport::Destroy();

	if (!WGLViewport::Create(display)) {
		WGLViewport::Destroy();
		return(false);
	}

	return(WGLViewport::Show());
}

void
Viewport::Tick(float)
{
	MSG l_msg;
	while (PeekMessage(&l_msg, 0, 0, 0, PM_NOREMOVE)) {
		if (GetMessage(&l_msg, 0, 0, 0) > 0)
			DispatchMessage(&l_msg);
		else if (WM_QUIT == l_msg.message) {
			Event::QuitEvent event(static_cast<int>(l_msg.wParam));
			Event::EventManager::Instance()->dispatch(event);
			break;
		}
	}
}

void
Viewport::SwapBuffer(void)
{
	using namespace OpenGL;
	WGLViewport::SwapBuffer();
	Painter::Reset();
}

const Graphics::Display &
Viewport::Display(void)
{
	using namespace OpenGL;
	return(WGLViewport::dpy);
}

const Math::Size2f &
Viewport::Size(void)
{
	using namespace OpenGL;
	return(WGLViewport::vsize);
}

const Math::Size2i &
Viewport::WindowSize(void)
{
	using namespace OpenGL;
	return(WGLViewport::wsize);
}

const Core::Type &
Viewport::Type(void)
{
	static const Core::Type s_type("WGL");
	return(s_type);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

