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

#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QKeyEvent>

#include <list>

#include "extensions.h"
#include <QtOpenGL/QGLFormat>
#include <QtOpenGL/QGLWidget>

/*
 * Qt4 Viewport Notes
 *
 * Refresh rate is ignored since we don't switch between modes, depth is also
 * ignored. We assume the user has selected an appropriate mode.
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace { /******************************************** Anonymous Namespace */

class ViewportWidget : public QGLWidget
{
	Math::Size2i        m_wsize;
	Math::Size2f        m_size;
public:

	ViewportWidget(const QGLFormat &_format, uint16_t _width, uint16_t _height,
	               bool fullscreen)
	    : QGLWidget(_format)
	{
		setAutoBufferSwap(false);
		setWindowTitle(MARSHMALLOW_BUILD_TITLE);

		const QRect &desktop = QApplication::desktop()->availableGeometry();
		if (fullscreen) {
			m_wsize[0] = desktop.width();
			m_wsize[1] = desktop.height();
		}
		else {
			m_wsize[0] = _width;
			m_wsize[1] = _height;
			move((desktop.width()  / 2) - (_width  / 2),
			     (desktop.height() / 2) - (_height / 2));
		}

		setFixedSize(m_wsize.width(), m_wsize.height());

		/* set viewport size */

		if (fullscreen) {
#if MARSHMALLOW_VIEWPORT_LOCK_WIDTH
			m_size[0] = static_cast<float>(_width);
			m_size[1] = (m_size[0] * static_cast<float>(m_wsize[1])) /
			    static_cast<float>(m_wsize[0]);
#else
			m_size[1] = static_cast<float>(_height);
			m_size[0] = (m_size[1] * static_cast<float>(m_wsize[0])) /
			    static_cast<float>(m_wsize[1]);
#endif
		}
		else {
			m_size[0] = static_cast<float>(_width);
			m_size[1] = static_cast<float>(_height);
		}
	}

	virtual ~ViewportWidget(void)
	{
	}

	const Math::Size2i & wsize(void) const
	    { return(m_wsize); }

	const Math::Size2f & size(void) const
	    { return(m_size); }

protected: /* virtual */

	VIRTUAL void
	initializeGL()
	{
		using namespace Graphics::OpenGL;

		/* extensions */

		Extensions::Initialize();

		/* initialize context */

		glViewport(0, 0, m_wsize.width(), m_wsize.height());

		if (glGetError() != GL_NO_ERROR) {
			MMERROR("GL: Failed during initialization.");
			return;
		}
	}

	VIRTUAL void
	closeEvent(QCloseEvent *e)
	{
		MMUNUSED(e);

		Event::QuitEvent l_event(-1);
		Event::EventManager::Instance()->dispatch(l_event);
	}

	VIRTUAL void
	keyPressEvent(QKeyEvent *e)
	{
		handleKeyEvent(*e, true);
	}

	VIRTUAL void
	keyReleaseEvent(QKeyEvent *e)
	{
		handleKeyEvent(*e, false);
	}

protected:

	void
	handleKeyEvent(const QKeyEvent &e, bool down)
	{
		typedef std::list<Event::KBKeys> KeyList;
		static KeyList s_keys_pressed;

		Event::KBKeys l_key = Event::KEY_NONE;
		Event::KBActions l_action =
		    (down ? Event::KeyPressed : Event::KeyReleased);

		int keycode = e.key();

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

static QApplication   *s_application(0);
static ViewportWidget *s_window(0);

bool
CreateWidget(uint16_t width, uint16_t height, bool fullscreen, bool vsync)
{
	using namespace Graphics;
	QGLFormat l_format(QGL::DoubleBuffer|QGL::DirectRendering|QGL::NoDepthBuffer|QGL::Rgba);
	l_format.setDepthBufferSize(0);
	l_format.setRedBufferSize(0);
	l_format.setGreenBufferSize(0);
	l_format.setBlueBufferSize(0);
	l_format.setAlphaBufferSize(0);
	l_format.setSwapInterval(vsync ? 1 : 0);
	l_format.setVersion(2, 0);

	s_window = new ViewportWidget(l_format, width, height, fullscreen);

	/* show window */

	if (fullscreen)
		s_window->showFullScreen();
	else
		s_window->show();

	Camera::Reset();
	Camera::Update();

	/* check status */

	if (!s_window->format().directRendering()) {
		MMERROR("GL: Context doesn't support direct rendering.");
		return(false);
	}

	if (glGetError() != GL_NO_ERROR) {
		MMERROR("GL failed during initialization.");
		return(false);
	}

	return(true);
}

void
DestroyWidget(void)
{
	delete s_window;
	s_window = 0;
}

} /****************************************************** Anonymous Namespace */

namespace Graphics { /************************************ Graphics Namespace */

OpenGL::PFNPROC
OpenGL::glGetProcAddress(const char *f)
{
	union {
		PFNPROC fptr;
		void    *ptr;
	} conv;

	assert(s_window && s_window->context() && "Invalid GL context!");

	conv.ptr = s_window->context()->getProcAddress(f);
	return(conv.fptr);
}

/********************************************************* Graphics::Viewport */

bool
Viewport::Initialize(uint16_t width, uint16_t height, uint8_t, uint8_t,
                     bool fullscreen, bool vsync)
{
	static int    argc = 0;
	static char **argv = 0;
	s_application = new QApplication(argc, argv);

	if (!CreateWidget(width, height, fullscreen, vsync)) {
		DestroyWidget();
		return(false);
	}

	Painter::Initialize();
	return(true);
}

void
Viewport::Finalize(void)
{
	Painter::Finalize();
	DestroyWidget();

	delete s_application, s_application = 0;
}

bool
Viewport::Redisplay(uint16_t width, uint16_t height, uint8_t, uint8_t,
                    bool fullscreen, bool vsync)
{
	DestroyWidget();

	if (!CreateWidget(width, height, fullscreen, vsync)) {
		DestroyWidget();
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
	static const Core::Type s_type("QT4");
	return(s_type);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

