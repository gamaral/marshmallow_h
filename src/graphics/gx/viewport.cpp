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

#include "math/pair.h"

#include "event/eventmanager.h"
#include "event/keyboardevent.h"
#include "event/quitevent.h"

#include "graphics/painter.h"
#include "graphics/transform.h"

#include <wiiuse/wpad.h>
#include <gccore.h>

#include <cstdio> //tmp
#include <cmath>
#include <cstring>
#include <list>

MARSHMALLOW_NAMESPACE_USE
using namespace Graphics;

/******************************************************************************/

namespace
{
	struct ViewportData {
		void         *xfb;
		GXRModeObj   *rmode;
		Math::Size2i  wsize;
		int           screen;
		Transform     camera;
		float         radius2;
		Math::Size2f  size;
		Math::Size2f  scaled_size;
		bool          loaded;
	} s_data;

	void UpdateCamera(void);
#if 0
	void HandleKeyEvent(XKeyEvent &key);
#endif

	void InitializeViewport(void)
	{
		s_data.camera.setRotation(.0f);
		s_data.camera.setScale(Math::Pair::One());
		s_data.camera.setTranslation(Math::Point2::Zero());

		s_data.loaded = false;
		s_data.xfb = 0;
		s_data.rmode = 0;
		s_data.size.zero();
		s_data.wsize[0] = s_data.wsize[1] = 0;

		VIDEO_Init();
		WPAD_Init();

		s_data.rmode = VIDEO_GetPreferredMode(0);
		s_data.xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(s_data.rmode));

		console_init(s_data.xfb, 20, 20, s_data.rmode->fbWidth, s_data.rmode->xfbHeight, s_data.rmode->fbWidth * VI_DISPLAY_PIX_SZ);

		VIDEO_Configure(s_data.rmode);
		VIDEO_SetNextFramebuffer(s_data.xfb);
		VIDEO_SetBlack(FALSE);
		VIDEO_Flush();

		VIDEO_WaitVSync();
		if(s_data.rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

		printf("\x1b[10;0H");
		printf("Hello from @marshmallow_h!\nTesting new GX backend!");

		s_data.loaded = true;
	}

	void
	UpdateCamera(void)
	{
		/* calculate scaled viewport size */
		s_data.scaled_size[0] = s_data.size[0] / s_data.camera.scale().first();
		s_data.scaled_size[1] = s_data.size[1] / s_data.camera.scale().second();

		/* calculate magnitude and pass it off as radius squared */
		s_data.radius2 = powf(s_data.scaled_size[0] / 2.f, 2.f) +
		                 powf(s_data.scaled_size[1] / 2.f, 2.f);
	}

#if 0
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
		default: MMWARNING("Unknown key pressed!");
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
#endif
} // namespace

/******************************************************************************/

bool
Viewport::Initialize(uint16_t w, uint16_t h, uint8_t d, bool f, bool v)
{
	MMUNUSED(w);
	MMUNUSED(h);
	MMUNUSED(d);
	MMUNUSED(f);
	MMUNUSED(v);

	InitializeViewport();
	Painter::Initialize();
	return(true);
}

void
Viewport::Finalize(void)
{
	Painter::Finalize();
}

bool
Viewport::Redisplay(uint16_t w, uint16_t h, uint8_t d, bool f, bool v)
{
	/* XXX: TODO */
	return(true);
}

void
Viewport::Tick(void)
{
	WPAD_ScanPads();

	u32 pressed = WPAD_ButtonsDown(0);

	if ( pressed & WPAD_BUTTON_HOME ) {
		Event::QuitEvent l_event(-1);
		Event::EventManager::Instance()->dispatch(l_event);
	}

#if 0
	XEvent e;

	while(XPending(s_data.display)) {
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

		default: MMINFO("Unknown viewport event received."); break;
		}
	}
#endif
}

void
Viewport::SwapBuffer(void)
{
	VIDEO_WaitVSync();
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
	static const Core::Type s_type("GLX");
	return(s_type);
}

