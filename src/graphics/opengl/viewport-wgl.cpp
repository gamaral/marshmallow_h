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

#include <list>

#include "headers.h"
#include "extensions.h"

/*
 * WGL Viewport Notes
 *
 * Refresh rate is ignored since we don't switch between modes, we assume the
 * user has selected an appropriate mode.
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace { /******************************************** Anonymous Namespace */

struct ViewportData
{
	HDC          dctx;
	HGLRC        ctx;
	HWND         window;
	Math::Size2i wsize;
	Math::Size2f size;
	bool         fullscreen;
	bool         loaded;
} s_data;

void
ResetViewportData(void)
{
	s_data.dctx = 0;
	s_data.ctx = 0;
	s_data.window = 0;
	s_data.fullscreen = false;
	s_data.size.zero();

	s_data.loaded = false;
}

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool GLCreateSurface(uint8_t vsync);
bool WWCreateWindow(uint16_t width, uint16_t height, uint8_t depth, bool fullscreen);

bool
CreateDisplay(uint16_t width, uint16_t height, uint8_t depth, uint8_t refresh,
              bool fullscreen, bool vsync)
{
	using namespace Graphics;

	ResetViewportData();

	/* display */

	if (!WWCreateWindow(width, height, depth, fullscreen)) {
		MMERROR("VC: Failed to create display.");
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
		MMERROR("WGL failed during initialization.");
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
DestroyDisplay(void)
{
	if (s_data.fullscreen)
		ShowCursor(true);

	if (s_data.ctx) {
		if (!wglMakeCurrent(0, 0))
			MMWARNING("WGL: Failed to unset current context");

		if (!wglDeleteContext(s_data.ctx))
			MMWARNING("WGL: Failed to delete context");

		s_data.ctx = 0;
	}

	if (s_data.dctx && !ReleaseDC(s_data.window, s_data.dctx))
		MMWARNING("Failed to release device context");
	s_data.dctx = 0;

	if (s_data.window && !DestroyWindow(s_data.window))
		MMWARNING("Failed to destroy window");
	s_data.window = 0;

	if (!UnregisterClass("marshmallow_wgl", GetModuleHandle(0)))
		MMWARNING("Failed to unregister window class");

	s_data.ctx    = 0;
	s_data.dctx   = 0;
	s_data.loaded = false;
	s_data.window = 0;
}

void
HandleKeyEvent(int keycode, bool down)
{
	typedef std::list<Event::KBKeys> KeyList;
	static KeyList s_keys_pressed;

	Event::KBKeys l_key = Event::KEY_NONE;
	Event::KBActions l_action =
	    (down ? Event::KeyPressed : Event::KeyReleased);

	/*
	 * Win32 virtual key codes for alphanumerical chars correspond
	 * to character values themselfs
	 */
	if (keycode >= '0' && keycode <= '9')
		l_key = static_cast<Event::KBKeys>(keycode);
	else if (keycode >= 'A' && keycode <= 'Z')
		l_key = static_cast<Event::KBKeys>(Event::KEY_A + (keycode - 'A'));
	else
		switch (keycode) {
		case VK_BACK:         l_key = Event::KEY_BACKSPACE; break;
		case VK_CAPITAL:      l_key = Event::KEY_CAPS_LOCK; break;
		case VK_CLEAR:        l_key = Event::KEY_CLEAR; break;
		case VK_DECIMAL:      l_key = Event::KEY_KDECIMAL; break;
		case VK_DELETE:       l_key = Event::KEY_DELETE; break;
		case VK_DIVIDE:       l_key = Event::KEY_KDIVIDE; break;
		case VK_DOWN:         l_key = Event::KEY_DOWN; break;
		case VK_END:          l_key = Event::KEY_END; break;
		case VK_ESCAPE:       l_key = Event::KEY_ESCAPE; break;
		case VK_F10:          l_key = Event::KEY_F10; break;
		case VK_F11:          l_key = Event::KEY_F11; break;
		case VK_F12:          l_key = Event::KEY_F12; break;
		case VK_F13:          l_key = Event::KEY_F13; break;
		case VK_F14:          l_key = Event::KEY_F14; break;
		case VK_F15:          l_key = Event::KEY_F15; break;
		case VK_F1:           l_key = Event::KEY_F1; break;
		case VK_F2:           l_key = Event::KEY_F2; break;
		case VK_F3:           l_key = Event::KEY_F3; break;
		case VK_F4:           l_key = Event::KEY_F4; break;
		case VK_F5:           l_key = Event::KEY_F5; break;
		case VK_F6:           l_key = Event::KEY_F6; break;
		case VK_F7:           l_key = Event::KEY_F7; break;
		case VK_F8:           l_key = Event::KEY_F8; break;
		case VK_F9:           l_key = Event::KEY_F9; break;
		case VK_HELP:         l_key = Event::KEY_HELP; break;
		case VK_HOME:         l_key = Event::KEY_HOME; break;
		case VK_INSERT:       l_key = Event::KEY_INSERT; break;
		case VK_LCONTROL:     l_key = Event::KEY_CONTROL_R; break;
		case VK_LEFT:         l_key = Event::KEY_LEFT; break;
		case VK_SHIFT:        l_key = Event::KEY_SHIFT_L; break;
		case VK_MENU:         l_key = Event::KEY_MENU; break;
		case VK_MULTIPLY:     l_key = Event::KEY_KMULTIPLY; break;
		case VK_NEXT:         l_key = Event::KEY_PAGE_DOWN; break;
		case VK_NUMLOCK:      l_key = Event::KEY_NUM_LOCK; break;
		case VK_NUMPAD0:      l_key = Event::KEY_K0; break;
		case VK_NUMPAD1:      l_key = Event::KEY_K1; break;
		case VK_NUMPAD2:      l_key = Event::KEY_K2; break;
		case VK_NUMPAD3:      l_key = Event::KEY_K3; break;
		case VK_NUMPAD4:      l_key = Event::KEY_K4; break;
		case VK_NUMPAD5:      l_key = Event::KEY_K5; break;
		case VK_NUMPAD6:      l_key = Event::KEY_K6; break;
		case VK_NUMPAD7:      l_key = Event::KEY_K7; break;
		case VK_NUMPAD8:      l_key = Event::KEY_K8; break;
		case VK_NUMPAD9:      l_key = Event::KEY_K9; break;
		case VK_PAUSE:        l_key = Event::KEY_PAUSE; break;
		case VK_PRINT:        l_key = Event::KEY_PRINT; break;
		case VK_PRIOR:        l_key = Event::KEY_PAGE_UP; break;
		case VK_RCONTROL:     l_key = Event::KEY_CONTROL_L; break;
		case VK_RETURN:       l_key = Event::KEY_RETURN; break;
		case VK_RIGHT:        l_key = Event::KEY_RIGHT; break;
		case VK_RSHIFT:       l_key = Event::KEY_SHIFT_R; break;
		case VK_SCROLL:       l_key = Event::KEY_SCROLL_LOCK; break;
		case VK_SPACE:        l_key = Event::KEY_SPACE; break;
		case VK_TAB:          l_key = Event::KEY_TAB; break;
		case VK_UP:           l_key = Event::KEY_UP; break;
		default: MMWARNING("Unknown key pressed!");
		}

	bool l_key_pressed = false;
	KeyList::const_iterator l_pressed_key_i;
	for (l_pressed_key_i  = s_keys_pressed.begin();
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
GLCreateSurface(uint8_t vsync)
{
	using namespace Graphics::OpenGL;

	/* create wiggle context */
	if (!(s_data.ctx = wglCreateContext(s_data.dctx))) {
		MMERROR("WGL: Failed to create context");
		return(false);
	}

	/* make wgl context current */
	if (!wglMakeCurrent(s_data.dctx, s_data.ctx)) {
		MMERROR("WGL: Failed to set current context");
		return(false);
	}

	/* extensions */

	Extensions::Initialize();

	/* vsync */

	if (Extensions::wglSwapInterval)
		Extensions::wglSwapInterval(vsync ? 1 : 0);

	return(true);
}

bool
WWCreateWindow(uint16_t width, uint16_t height, uint8_t depth, bool fullscreen)
{
	s_data.fullscreen = fullscreen;
	s_data.wsize[0]   = width;
	s_data.wsize[1]   = height;

	WNDCLASS l_wc;
	l_wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	l_wc.lpfnWndProc   = reinterpret_cast<WNDPROC>(WindowProc);
	l_wc.cbClsExtra    = 0;
	l_wc.cbWndExtra    = 0;
	l_wc.hInstance     = GetModuleHandle(0);
	l_wc.hIcon         = 0;
	l_wc.hCursor       = LoadCursor(0, IDC_ARROW);
	l_wc.hbrBackground = 0;
	l_wc.lpszMenuName  = 0;
	l_wc.lpszClassName = "marshmallow_wgl";

	if (!(RegisterClass(&l_wc))) {
		MMERROR("Failed to register window class.");
		return false;
	}

	RECT l_warea;
	RECT l_wrect;

	/* get desktop size */
	if (!SystemParametersInfo(SPI_GETWORKAREA, sizeof(RECT), &l_warea, 0)) {
		l_warea.left   = l_warea.top = 0;
		l_warea.right  = GetSystemMetrics(SM_CXSCREEN);
		l_warea.bottom = GetSystemMetrics(SM_CYSCREEN);
	}

	DWORD l_wstyle[2];
	l_wstyle[0] = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	l_wstyle[1] = WS_EX_APPWINDOW;
	if (s_data.fullscreen) {
		l_wstyle[0] |= WS_POPUP;
		l_wstyle[1] |= WS_EX_TOPMOST;
		s_data.wsize[0] = l_warea.right;
		s_data.wsize[1] = l_warea.bottom;
		l_wrect = l_warea;
		ShowCursor(false);
	}
	else {
		l_wstyle[0] |= WS_OVERLAPPEDWINDOW;
		l_wstyle[1] |= WS_EX_WINDOWEDGE;
		l_wrect.left   = ((l_warea.right - l_warea.left) - width) / 2;
		l_wrect.right  = l_wrect.left + width;
		l_wrect.top    = ((l_warea.bottom - l_warea.top) - height) / 2;
		l_wrect.bottom = l_wrect.top  + height;
	}
	AdjustWindowRectEx(&l_wrect, l_wstyle[0], false, l_wstyle[1]);

	/* create actual window */
	s_data.window =
	    CreateWindowEx(l_wstyle[1], "marshmallow_wgl", MARSHMALLOW_BUILD_TITLE, l_wstyle[0],
	                   l_wrect.left, l_wrect.top,
	                   l_wrect.right - l_wrect.left,
	                   l_wrect.bottom - l_wrect.top,
	                   0, 0, l_wc.hInstance, 0);
	if (!s_data.window) {
		MMERROR("Failed to create window");
		return(false);
	}
	ShowWindow(s_data.window, SW_SHOW);
	SetForegroundWindow(s_data.window);
	SetFocus(s_data.window);

	/* create device context */
	if (!(s_data.dctx = GetDC(s_data.window))) {
		MMERROR("Failed to create device context");
		return(false);
	}

	/* select pixel format */
	static PIXELFORMATDESCRIPTOR l_pfd;
	memset(&l_pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	l_pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
	l_pfd.nVersion   = 1;
	l_pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	l_pfd.iPixelType = PFD_TYPE_RGBA;
	l_pfd.cColorBits = depth;
	l_pfd.cRedBits   = 0;
	l_pfd.cGreenBits = 0;
	l_pfd.cBlueBits  = 0;
	l_pfd.cAlphaBits = 0;
	l_pfd.iLayerType = PFD_MAIN_PLANE;

	unsigned int l_pfindex;
	if (!(l_pfindex = ChoosePixelFormat(s_data.dctx, &l_pfd))) {
		MMERROR("Failed to select a pixel format");
		return(false);
	}

	if (!SetPixelFormat(s_data.dctx, l_pfindex, &l_pfd)) {
		MMERROR("Failed to bind pixel format to window context");
		return(false);
	}

	return(true);
}

LRESULT CALLBACK
WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CLOSE: {
		Event::QuitEvent event(-1);
		Event::EventManager::Instance()->dispatch(event);
		return(0);
	} break;

	case WM_SIZE:
		return(0);
	break;

	case WM_SYSCOMMAND:
		switch (wParam) {
		case SC_MONITORPOWER:
		case SC_SCREENSAVE:
			if (s_data.fullscreen)
				return(0);
		break;
		}
	break;

	case WM_KEYDOWN:
		HandleKeyEvent(static_cast<int>(wParam), true);
		break;
	case WM_KEYUP:
		HandleKeyEvent(static_cast<int>(wParam), false);
		break;
	}

	return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}

} /****************************************************** Anonymous Namespace */

namespace Graphics { /************************************ Graphics Namespace */

OpenGL::PFNPROC
OpenGL::glGetProcAddress(const char *f)
{
	return(reinterpret_cast<PFNPROC>(wglGetProcAddress(f)));
}

/********************************************************* Graphics::Viewport */

bool
Viewport::Initialize(uint16_t width, uint16_t height, uint8_t depth,
                     bool fullscreen, uint8_t refresh, uint8_t vsync)
{
	Camera::Reset();

	if (!CreateDisplay(width, height, depth, refresh, fullscreen, vsync)) {
		DestroyDisplay();
		return(false);
	}

	Painter::Initialize();
	return(true);
}

void
Viewport::Finalize(void)
{
	Painter::Finalize();

	DestroyDisplay();
}

bool
Viewport::Redisplay(uint16_t width, uint16_t height, uint8_t depth,
                    bool fullscreen, uint8_t refresh, uint8_t vsync)
{
	DestroyDisplay();

	if (!CreateDisplay(width, height, depth, refresh, fullscreen, vsync)) {
		DestroyDisplay();
		return(false);
	}
	return(true);
}

void
Viewport::Tick(void)
{
	MSG l_msg;
	while (PeekMessage(&l_msg, 0, 0, 0, PM_NOREMOVE)) {
		if (l_msg.message == WM_QUIT) {
			Event::QuitEvent event(-1);
			Event::EventManager::Instance()->dispatch(event);
			break;
		}

		if (GetMessage(&l_msg, 0, 0, 0) > 0) {
			TranslateMessage(&l_msg);
			DispatchMessage(&l_msg);
		}
	}
}

void
Viewport::SwapBuffer(void)
{
	SwapBuffers(s_data.dctx);
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
	static const Core::Type s_type("WGL");
	return(s_type);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

