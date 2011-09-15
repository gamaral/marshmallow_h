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

#include <windows.h>

#include <GL/gl.h>

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
		HDC           dcontext;
		HGLRC         context;
		HWND          window;
		Math::Size2i  wsize;
		Math::Triplet camera;
		Math::Size2f  size;
		Math::Point2  visible[2];
		bool          fullscreen;
		bool          loaded;
		bool          vbo_supported;
	} s_data;

	bool CheckVBOSupport(void);
	void UpdateViewport(void);
	void HandleKeyEvent(int keycode, bool down);
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void InitializeViewport(void)
	{
		s_data.camera[0] = s_data.camera[1] = .0f; // camera x y
		s_data.camera[2] = 1.f;                    // camera zoom
		s_data.dcontext = 0;
		s_data.context = 0;
		s_data.window = 0;
		s_data.fullscreen = false;
		s_data.loaded = false;
		s_data.size.zero();
		s_data.vbo_supported = false;
		s_data.visible[0] = s_data.visible[1] = Math::Point2::Zero();
	}

	bool
	CreateWWindow(int w, int h, int d, bool f)
	{
		s_data.context     = 0;
		s_data.dcontext    = 0;
		s_data.loaded      = false;
		s_data.window      = 0;

		s_data.fullscreen  = f;
		s_data.wsize[0]    = w;
		s_data.wsize[1]    = h;

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
	
		if (s_data.fullscreen) {
			DEVMODE l_dm;
			memset(&l_dm, 0, sizeof(l_dm));

			l_dm.dmSize       = sizeof(l_dm);
			l_dm.dmPelsWidth  = w;
			l_dm.dmPelsHeight = h;
			l_dm.dmBitsPerPel = d;
			l_dm.dmFields     = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

			if (ChangeDisplaySettings(&l_dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
				MMERROR("Failed to switch modes for fullscreen viewport");
				s_data.fullscreen = false;
			} else {
				ShowCursor(false);
			}
		}

		DWORD l_wstyle[2];
		l_wstyle[0] = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		l_wstyle[1] = WS_EX_APPWINDOW;
		if (s_data.fullscreen) l_wstyle[0] |= WS_POPUP;
		else {
			l_wstyle[0] |= WS_OVERLAPPEDWINDOW;
			l_wstyle[1] |= WS_EX_WINDOWEDGE;
		}

		/* get desktop size */
		RECT l_warea;
		if (!SystemParametersInfo(SPI_GETWORKAREA, sizeof(RECT), &l_warea, 0)) {
			l_warea.left   = l_warea.top = 0;
			l_warea.right  = GetSystemMetrics(SM_CXSCREEN);
			l_warea.bottom = GetSystemMetrics(SM_CYSCREEN);
		}

		/* create window rect */
		RECT l_wrect;
		l_wrect.left   = ((l_warea.right - l_warea.left) - w) / 2;
		l_wrect.right  = l_wrect.left + w;
		l_wrect.top    = ((l_warea.bottom - l_warea.top) - h) / 2;
		l_wrect.bottom = l_wrect.top  + h;
		AdjustWindowRectEx(&l_wrect, l_wstyle[0], false, l_wstyle[1]);

		/* create actual window */
		s_data.window = CreateWindowEx(l_wstyle[1], "marshmallow_wgl", 0, l_wstyle[0],
		    l_wrect.left, l_wrect.top, l_wrect.right - l_wrect.left,
		    l_wrect.bottom - l_wrect.top, 0, 0, l_wc.hInstance, 0);
		if (!s_data.window) {
			MMERROR1("Failed to create window");
			return(false);
		}

		/* create device context */
		if (!(s_data.dcontext = GetDC(s_data.window))) {
			MMERROR1("Failed to create device context");
			return(false);
		}

		/* select pixel format */
		static PIXELFORMATDESCRIPTOR l_pfd;
		memset(&l_pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		l_pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
		l_pfd.nVersion   = 1;
		l_pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		l_pfd.iPixelType = PFD_TYPE_RGBA;
		l_pfd.cColorBits = d;
		l_pfd.cDepthBits = 16;
		l_pfd.iLayerType = PFD_MAIN_PLANE;
	
		unsigned int l_pfindex;
		if (!(l_pfindex = ChoosePixelFormat(s_data.dcontext, &l_pfd))) {
			MMERROR1("Failed to select a pixel format");
			return(false);
		}

		if(!SetPixelFormat(s_data.dcontext, l_pfindex, &l_pfd)) {
			MMERROR1("Failed to bind pixel format to window context");
			return(false);
		}

		/* create wiggle context */
		if (!(s_data.context = wglCreateContext(s_data.dcontext))) {
			MMERROR1("Failed to create OpenGL context");
			return(false);
		}

		/* make wgl context current */
		if(!wglMakeCurrent(s_data.dcontext, s_data.context)) {
			MMERROR1("Failed to set current OpenGL context");
			return(false);
		}

		ShowWindow(s_data.window, SW_SHOW);
		SetForegroundWindow(s_data.window);
		SetFocus(s_data.window);

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
			MMERROR1("WGL failed during initialization.");
			return(false);
		}

		return(s_data.loaded = true);
	}

	void
	DestroyWWindow(void)
	{
		if (s_data.fullscreen) {
			ShowCursor(true);
			ChangeDisplaySettings(0, 0);
		}

		if (s_data.context) {
			if (!wglMakeCurrent(0, 0))
				MMWARNING1("Failed to unset current OpenGL context");

			if (!wglDeleteContext(s_data.context))
				MMWARNING1("Failed to delete OpenGL context");

			s_data.context = 0;
		}

		if (s_data.dcontext && !ReleaseDC(s_data.window, s_data.dcontext))
			MMWARNING1("Failed to release device context");
		s_data.dcontext = 0;

		if (s_data.window && !DestroyWindow(s_data.window))
			MMWARNING1("Failed to destroy window");
		s_data.window = 0;

		if (!UnregisterClass("marshmallow_wgl", GetModuleHandle(0)))
			MMWARNING1("Failed to unregister window class");

		s_data.context   = 0;
		s_data.dcontext  = 0;
		s_data.loaded    = false;
		s_data.window    = 0;
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
		        (wglGetProcAddress(reinterpret_cast<LPCSTR>("glGenBuffersARB")));
		glBindBufferARB =
		    reinterpret_cast<PFNGLBINDBUFFERARBPROC>
		        (wglGetProcAddress(reinterpret_cast<LPCSTR>("glBindBufferARB")));
		glBufferDataARB =
		    reinterpret_cast<PFNGLBUFFERDATAARBPROC>
		        (wglGetProcAddress(reinterpret_cast<LPCSTR>("glBufferDataARB")));
		glBufferSubDataARB =
		    reinterpret_cast<PFNGLBUFFERSUBDATAARBPROC>
		        (wglGetProcAddress(reinterpret_cast<LPCSTR>("glBufferSubDataARB")));
		glDeleteBuffersARB =
		    reinterpret_cast<PFNGLDELETEBUFFERSARBPROC>
		        (wglGetProcAddress(reinterpret_cast<LPCSTR>("glDeleteBuffersARB")));
		glGetBufferParameterivARB =
		    reinterpret_cast<PFNGLGETBUFFERPARAMETERIVARBPROC>
		        (wglGetProcAddress(reinterpret_cast<LPCSTR>("glGetBufferParameterivARB")));
		glMapBufferARB =
		    reinterpret_cast<PFNGLMAPBUFFERARBPROC>
		        (wglGetProcAddress(reinterpret_cast<LPCSTR>("glMapBufferARB")));
		glUnmapBufferARB =
		    reinterpret_cast<PFNGLUNMAPBUFFERARBPROC>
		        (wglGetProcAddress(reinterpret_cast<LPCSTR>("glUnmapBufferARB")));

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
		s_data.size[0] = DEFAULT_VIEWPORT_VWIDTH  * s_data.camera[2];
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
	HandleKeyEvent(int keycode, bool down)
	{
		typedef eastl::list<Event::KBKeys> KeyList;
		static KeyList s_keys_pressed;

		Event::KBKeys l_key = Event::KEY_NONE;
		Event::KBActions l_action =
		    (down ? Event::KeyPressed : Event::KeyReleased);

		/*
		 * Win32 virtual key codes for alphanumerical chars correspond
		 * to character values themselfs
		 */
		if ((keycode >= '0' && keycode <= '9') ||
		    (keycode >= 'a' && keycode <= 'z') ) {
			l_key = static_cast<Event::KBKeys>(keycode);
		} else {
			switch (keycode) {
#if 0
			case XK_Alt_L:        l_key = Event::KEY_ALT_L; break;
			case XK_Alt_R:        l_key = Event::KEY_ALT_R; break;
#endif
			case VK_BACK:         l_key = Event::KEY_BACKSPACE; break;
			case VK_CAPITAL:      l_key = Event::KEY_CAPS_LOCK; break;
			case VK_CLEAR:        l_key = Event::KEY_CLEAR; break;
			case VK_LCONTROL:     l_key = Event::KEY_CONTROL_R; break;
			case VK_RCONTROL:     l_key = Event::KEY_CONTROL_L; break;
			case VK_DELETE:       l_key = Event::KEY_DELETE; break;
			case VK_DOWN:         l_key = Event::KEY_DOWN; break;
			case VK_END:          l_key = Event::KEY_END; break;
			case VK_ESCAPE:       l_key = Event::KEY_ESCAPE; break;
			case VK_HELP:         l_key = Event::KEY_HELP; break;
			case VK_HOME:         l_key = Event::KEY_HOME; break;
			case VK_INSERT:       l_key = Event::KEY_INSERT; break;
			case VK_LEFT:         l_key = Event::KEY_LEFT; break;
			case VK_MENU:         l_key = Event::KEY_MENU; break;
			case VK_NUMLOCK:      l_key = Event::KEY_NUM_LOCK; break;
			case VK_NEXT:         l_key = Event::KEY_PAGE_DOWN; break;
			case VK_PRIOR:        l_key = Event::KEY_PAGE_UP; break;
			case VK_PAUSE:        l_key = Event::KEY_PAUSE; break;
			case VK_PRINT:        l_key = Event::KEY_PRINT; break;
			case VK_RETURN:       l_key = Event::KEY_RETURN; break;
			case VK_RIGHT:        l_key = Event::KEY_RIGHT; break;
			case VK_SCROLL:       l_key = Event::KEY_SCROLL_LOCK; break;
			case VK_LSHIFT:       l_key = Event::KEY_SHIFT_L; break;
			case VK_RSHIFT:       l_key = Event::KEY_SHIFT_R; break;
			case VK_TAB:          l_key = Event::KEY_TAB; break;
			case VK_UP:           l_key = Event::KEY_UP; break;
#if 0
			case XK_backslash:    l_key = Event::KEY_BACKSLASH; break;
			case XK_bracketleft:  l_key = Event::KEY_BRACKETLEFT; break;
			case XK_bracketright: l_key = Event::KEY_BRACKETRIGHT; break;
			case XK_equal:        l_key = Event::KEY_EQUAL; break;
			case XK_less:         l_key = Event::KEY_LESS; break;
			case XK_quotedbl:     l_key = Event::KEY_DBLQUOTE; break;
			case XK_semicolon:    l_key = Event::KEY_SEMICOLON; break;
#endif
			case VK_SPACE:        l_key = Event::KEY_SPACE; break;
			case VK_F1:           l_key = Event::KEY_F1; break;
			case VK_F2:           l_key = Event::KEY_F2; break;
			case VK_F3:           l_key = Event::KEY_F3; break;
			case VK_F4:           l_key = Event::KEY_F4; break;
			case VK_F5:           l_key = Event::KEY_F5; break;
			case VK_F6:           l_key = Event::KEY_F6; break;
			case VK_F7:           l_key = Event::KEY_F7; break;
			case VK_F8:           l_key = Event::KEY_F8; break;
			case VK_F9:           l_key = Event::KEY_F9; break;
			case VK_F10:          l_key = Event::KEY_F10; break;
			case VK_F11:          l_key = Event::KEY_F11; break;
			case VK_F12:          l_key = Event::KEY_F12; break;
			case VK_F13:          l_key = Event::KEY_F13; break;
			case VK_F14:          l_key = Event::KEY_F14; break;
			case VK_F15:          l_key = Event::KEY_F15; break;
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
			case VK_DECIMAL:      l_key = Event::KEY_KDECIMAL; break;
			case VK_DIVIDE:       l_key = Event::KEY_KDIVIDE; break;
			case VK_MULTIPLY:     l_key = Event::KEY_KMULTIPLY; break;
			default: MMWARNING1("Unknown key pressed!");
			}
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
} // namespace

/******************************************************************************/

bool
Viewport::Initialize(int w, int h, int d, bool f)
{
	InitializeViewport();

	if (!CreateWWindow(w, h, d, f)) {
		DestroyWWindow();
		return(false);
	}

	Painter::Initialize();
	return(true);
}

void
Viewport::Finalize(void)
{
	Painter::Finalize();
	DestroyWWindow();
}

bool
Viewport::Redisplay(int w, int h, int d, bool f)
{
	DestroyWWindow();

	if(!CreateWWindow(w, h, d, f)) {
		DestroyWWindow();
		return(false);
	}
	return(true);
}

void
Viewport::Tick(TIME t)
{
	TIMEOUT_INIT;

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
	SwapBuffers(s_data.dcontext);
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
	static const Core::Type s_type("WGL");
	return(s_type);
}

