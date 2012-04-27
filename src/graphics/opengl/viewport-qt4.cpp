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

#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QKeyEvent>

#include <QtOpenGL/QGLWidget>

#include <cmath>
#include <list>

MARSHMALLOW_NAMESPACE_USE
using namespace Graphics;

/******************************************************************************/

namespace
{
	class ViewportWidget : public QGLWidget
	{
		Math::Size2i  m_wsize;
		Transform     m_camera;
		float         m_radius2;
		Math::Size2f  m_scaled_size;
		Math::Size2f  m_size;
	public:

		ViewportWidget(uint16_t w, uint16_t h, bool f)
		{
			setAutoBufferSwap(false);
			setFixedSize(w, h);
			setWindowTitle(MARSHMALLOW_BUILD_TITLE);

			m_wsize[0] = w;
			m_wsize[1] = h;

			m_camera.setRotation(.0f);
			m_camera.setScale(Math::Pair::One());
			m_camera.setTranslation(Math::Point2::Zero());

			m_size.zero();
			m_scaled_size.zero();

			if (!f) {
				const QRect &desktop = QApplication::desktop()->availableGeometry();
				move((desktop.width() / 2) - (w / 2), (desktop.height() / 2) - (h / 2));
			}
		}

		virtual ~ViewportWidget(void)
		{
		}

		float radius2(void) const
		    { return(m_radius2); }

		const Math::Size2i &wsize(void) const
		    { return(m_wsize); }

		const Math::Size2f &scaledSize(void) const
		    { return(m_scaled_size); }

		const Math::Size2f &size(void) const
		    { return(m_size); }

		const Transform &camera(void) const
		    { return(m_camera); }

		void
		updateCamera(const Transform &c)
		{
			m_camera = c;

			/* calculate scaled viewport size */
			m_scaled_size[0] = m_size[0] / m_camera.scale().first();
			m_scaled_size[1] = m_size[1] / m_camera.scale().second();

			/* calculate magnitude and pass it off as radius squared */
			m_radius2 = powf(m_scaled_size[0] / 2.f, 2.f) +
				    powf(m_scaled_size[1] / 2.f, 2.f);
		}

	protected: /* virtual */

		VIRTUAL void
		initializeGL()
		{
			/* initialize context */

			glViewport(0, 0, m_wsize.width(), m_wsize.height());

			if(glGetError() != GL_NO_ERROR) {
				MMERROR("GLX failed during initialization.");
				return;
			}

			/* set viewport size */

			m_size[0] = MARSHMALLOW_VIEWPORT_VWIDTH;
			m_size[1] = MARSHMALLOW_VIEWPORT_VHEIGHT;

			Viewport::SetCamera(m_camera);

		}

		VIRTUAL void
		closeEvent(QCloseEvent * event)
		{
			Event::QuitEvent l_event(-1);
			Event::EventManager::Instance()->dispatch(l_event);
		}

		VIRTUAL void
		keyPressEvent(QKeyEvent *event)
		{
			handleKeyEvent(*event, true);
		}

		VIRTUAL void
		keyReleaseEvent(QKeyEvent *event)
		{
			handleKeyEvent(*event, false);
		}

	protected:

		void
		handleKeyEvent(const QKeyEvent &event, bool down)
		{
			typedef std::list<Event::KBKeys> KeyList;
			static KeyList s_keys_pressed;

			Event::KBKeys l_key = Event::KEY_NONE;
			Event::KBActions l_action =
			    (down ? Event::KeyPressed : Event::KeyReleased);

			int keycode = event.key();

			/* force lower case */
			if (keycode >= 65 && keycode <= 90)
				keycode += 32;

			if ((keycode >= ' ' && keycode <= '@') ||
			    (keycode >= '[' && keycode <= '~') )
				l_key = static_cast<Event::KBKeys>(keycode);
			switch (keycode) {
			case Qt::Key_Backspace:    l_key = Event::KEY_BACKSPACE; break;
			case Qt::Key_Tab:          l_key = Event::KEY_TAB; break;
			case Qt::Key_Clear:        l_key = Event::KEY_CLEAR; break;
			case Qt::Key_Return:       l_key = Event::KEY_RETURN; break;
			case Qt::Key_Pause:        l_key = Event::KEY_PAUSE; break;
			case Qt::Key_Escape:       l_key = Event::KEY_ESCAPE; break;

			case Qt::Key_Alt:          l_key = Event::KEY_ALT_L; break;
			case Qt::Key_CapsLock:     l_key = Event::KEY_CAPS_LOCK; break;
			case Qt::Key_Control:      l_key = Event::KEY_CONTROL_L; break;
			case Qt::Key_Delete:       l_key = Event::KEY_DELETE; break;
			case Qt::Key_Down:         l_key = Event::KEY_DOWN; break;
			case Qt::Key_End:          l_key = Event::KEY_END; break;
			case Qt::Key_Help:         l_key = Event::KEY_HELP; break;
			case Qt::Key_Home:         l_key = Event::KEY_HOME; break;
			case Qt::Key_Insert:       l_key = Event::KEY_INSERT; break;
			case Qt::Key_Left:         l_key = Event::KEY_LEFT; break;
			case Qt::Key_Menu:         l_key = Event::KEY_MENU; break;
			case Qt::Key_Meta:         l_key = Event::KEY_META_L; break;
			case Qt::Key_NumLock:      l_key = Event::KEY_NUM_LOCK; break;
			case Qt::Key_PageDown:     l_key = Event::KEY_PAGE_DOWN; break;
			case Qt::Key_PageUp:       l_key = Event::KEY_PAGE_UP; break;
			case Qt::Key_Print:        l_key = Event::KEY_PRINT; break;
			case Qt::Key_Right:        l_key = Event::KEY_RIGHT; break;
			case Qt::Key_ScrollLock:   l_key = Event::KEY_SCROLL_LOCK; break;
			case Qt::Key_Shift:        l_key = Event::KEY_SHIFT_L; break;
			case Qt::Key_Up:           l_key = Event::KEY_UP; break;
			case Qt::Key_Backslash:    l_key = Event::KEY_BACKSLASH; break;
			case Qt::Key_BraceLeft:    l_key = Event::KEY_BRACKET_LEFT; break;
			case Qt::Key_BraceRight:   l_key = Event::KEY_BRACKET_RIGHT; break;
			case Qt::Key_Equal:        l_key = Event::KEY_EQUAL; break;
			case Qt::Key_Less:         l_key = Event::KEY_LESS; break;
			case Qt::Key_QuoteDbl:     l_key = Event::KEY_DBLQUOTE; break;
			case Qt::Key_Semicolon:    l_key = Event::KEY_SEMICOLON; break;
			case Qt::Key_Space:        l_key = Event::KEY_SPACE; break;
			case Qt::Key_F1:           l_key = Event::KEY_F1; break;
			case Qt::Key_F2:           l_key = Event::KEY_F2; break;
			case Qt::Key_F3:           l_key = Event::KEY_F3; break;
			case Qt::Key_F4:           l_key = Event::KEY_F4; break;
			case Qt::Key_F5:           l_key = Event::KEY_F5; break;
			case Qt::Key_F6:           l_key = Event::KEY_F6; break;
			case Qt::Key_F7:           l_key = Event::KEY_F7; break;
			case Qt::Key_F8:           l_key = Event::KEY_F8; break;
			case Qt::Key_F9:           l_key = Event::KEY_F9; break;
			case Qt::Key_F10:          l_key = Event::KEY_F10; break;
			case Qt::Key_F11:          l_key = Event::KEY_F11; break;
			case Qt::Key_F12:          l_key = Event::KEY_F12; break;
			case Qt::Key_F13:          l_key = Event::KEY_F13; break;
			case Qt::Key_F14:          l_key = Event::KEY_F14; break;
			case Qt::Key_F15:          l_key = Event::KEY_F15; break;
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
				Event::SharedEvent l_event(new Event::KeyboardEvent(l_key, l_action));
				Event::EventManager::Instance()->queue(l_event);

				if (l_key_pressed) s_keys_pressed.remove(l_key);
				else s_keys_pressed.push_front(l_key);
			}
		}
	};

	static QApplication *s_application(0);
	static ViewportWidget *s_window(0);

	bool
	CreateWindow(uint16_t w, uint16_t h, uint8_t, bool f, bool v)
	{
		s_window = new ViewportWidget(w, h, f);

		/* show window */

		if (f) s_window->showFullScreen();
		else s_window->show();

		/* check status */

		if (!s_window->format().directRendering()) {
			MMERROR("GL context doesn't support direct rendering.");
			return(false);
		}

#if 0
		/* vsync */

#if GLX_SGI_swap_control
		if (GLEE_GLX_SGI_swap_control)
			glXSwapIntervalSGI(v ? 1 : 0);
#elif WGL_EXT_swap_control
		if(GLEE_WGL_EXT_swap_control)
			wglSwapIntervalEXT(v ? 1 : 0);
#endif
#endif

		if(glGetError() != GL_NO_ERROR) {
			MMERROR("GL failed during initialization.");
			return(false);
		}

		return(true);
	}

	void
	DestroyWindow(void)
	{
		delete s_window;
		s_window = 0;
	}

} // namespace

/******************************************************************************/

bool
Viewport::Initialize(uint16_t w, uint16_t h, uint8_t d, bool f, bool v)
{
	static int argc = 0;
	static char **argv = 0;
	s_application = new QApplication(argc, argv);

	if (!CreateWindow(w, h, d, f, v)) {
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

	delete s_application;
	s_application = 0;
}

bool
Viewport::Redisplay(uint16_t w, uint16_t h, uint8_t d, bool f, bool v)
{
	DestroyWindow();

	if(!CreateWindow(w, h, d, f, v)) {
		DestroyWindow();
		return(false);
	}
	return(true);
}

void
Viewport::Tick(void)
{
	s_application->processEvents();
}

void
Viewport::SwapBuffer(void)
{
	s_window->swapBuffers();
	Painter::Reset();
}

const Graphics::Transform &
Viewport::Camera(void)
{
	return(s_window->camera());
}

void
Viewport::SetCamera(const Graphics::Transform &camera)
{
	s_window->updateCamera(camera);
}

float
Viewport::Radius2(void)
{
	return(s_window->radius2());
}

const Math::Size2f &
Viewport::ScaledSize(void)
{
	return(s_window->scaledSize());
}

const Math::Size2f &
Viewport::Size(void)
{
	return(s_window->size());
}

const Math::Size2i &
Viewport::WindowSize(void)
{
	return(s_window->wsize());
}

const Core::Type &
Viewport::Type(void)
{
	static const Core::Type s_type("QT");
	return(s_type);
}

