/*
 * Copyright (c) 2011-2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
 * All rights reserved.
 *
 * This file is part of Marshmallow Game Engine.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the project as a whole.
 */

#include "graphics/backend_p.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QKeyEvent>

#include <cassert>
#include <list>

#include "core/logger.h"
#include "core/type.h"

#include "event/eventmanager.h"
#include "event/keyboardevent.h"
#include "event/quitevent.h"
#include "event/viewportevent.h"

#ifdef MARSHMALLOW_INPUT_MISC_QT4
#  include "input/misc/qt4.h"
#endif

#include "graphics/camera.h"
#include "graphics/color.h"
#include "graphics/display.h"
#include "graphics/painter_p.h"


#include "extensions.h"
#include <QtOpenGL/QGLFormat>
#include <QtOpenGL/QGLWidget>


/*
 * Qt4 Backend Notes
 *
 */

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */
namespace OpenGL { /****************************** Graphics::OpenGL Namespace */
namespace { /************************ Graphics::OpenGL::<anonymous> Namespace */

namespace Qt4Backend {

	class Widget;

	inline bool Initialize(void);
	inline void Finalize(void);

	inline void Reset(int state);

	inline bool Create(const Display &display);
	inline void Destroy(void);

	inline bool Show(void);
	inline void Hide(void);

	enum StateFlag
	{
		sfUninitialized = 0,
		sfReady         = (1 << 0),

		sfApplication   = (1 << 1),
		sfWidget        = (1 << 2),

		sfExposed       = (1 << 3),

		sfFocused       = (1 << 4),
		sfKeyboardFocus = (1 << 5),
		sfMouseFocus    = (1 << 6),

		sfTerminated    = (1 << 7),
		sfValid         = sfApplication|sfWidget,
		sfActive        = sfReady|sfValid|sfExposed|sfFocused
	};
	/******************* MARSHMALLOW */
	Display           dpy;
	Math::Size2i      wsize;
	Math::Size2f      vsize;
	int               flags;

	/*************************** QT4 */
	QApplication     *application(0);
	Widget           *window(0);

}

class Qt4Backend::Widget : public QGLWidget
{
public:

	Widget(const QGLFormat &format_)
	    : QGLWidget(format_)
	{
		setAutoBufferSwap(false);
		setWindowTitle(MARSHMALLOW_GRAPHICS_TITLE);
	}

protected: /* virtual */

	VIRTUAL void
	initializeGL()
	{
		/* extensions */

		Extensions::Initialize();

		/* initialize context */

		glViewport(0, 0, wsize.width, wsize.height);

		if (glGetError() != GL_NO_ERROR) {
			MMERROR("GL: Failed during initialization.");
			return;
		}
	}

	VIRTUAL void
	showEvent(QShowEvent *)
	{
		flags |= sfExposed;
	}

	VIRTUAL void
	hideEvent(QHideEvent *)
	{
		flags &= ~(sfExposed|sfFocused|sfKeyboardFocus|sfMouseFocus);
	}

	VIRTUAL void
	closeEvent(QCloseEvent *)
	{
		Event::QuitEvent l_event(-1);
		Event::EventManager::Instance()->dispatch(l_event);
	}

	VIRTUAL void
	enterEvent(QEvent *)
	{
		flags |= sfKeyboardFocus|sfFocused;
	}

	VIRTUAL void
	leaveEvent(QEvent *)
	{
		flags &= ~(sfKeyboardFocus);

		if (0 == (flags & sfMouseFocus))
			flags &= ~(sfFocused);
	}

	VIRTUAL void
	focusInEvent(QFocusEvent *)
	{
		flags |= sfMouseFocus|sfFocused;
	}

	VIRTUAL void
	focusOutEvent(QFocusEvent *)
	{
		flags &= ~(sfMouseFocus);

		if (0 == (flags & sfKeyboardFocus))
			flags &= ~(sfFocused);
	}

#ifdef MARSHMALLOW_INPUT_MISC_QT4
	VIRTUAL void
	keyPressEvent(QKeyEvent *e)
	{
		Input::Misc::Qt4::HandleKeyboardEvent(*e, true);
	}

	VIRTUAL void
	keyReleaseEvent(QKeyEvent *e)
	{
		Input::Misc::Qt4::HandleKeyboardEvent(*e, false);
	}
#endif
};

bool
Qt4Backend::Initialize(void)
{
	/* default display display */
	dpy.depth      = MARSHMALLOW_GRAPHICS_DEPTH;
	dpy.fullscreen = MARSHMALLOW_GRAPHICS_FULLSCREEN;
	dpy.height     = MARSHMALLOW_GRAPHICS_HEIGHT;
	dpy.vsync      = MARSHMALLOW_GRAPHICS_VSYNC;
	dpy.width      = MARSHMALLOW_GRAPHICS_WIDTH;

	Reset(sfUninitialized);

	/*
	 * Create QApplication
	 */
	static int    argc = 0;
	static char **argv = 0;
	application = new QApplication(argc, argv);
	flags |= sfApplication;

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
Qt4Backend::Finalize(void)
{
	/* set termination flag */
	flags |= sfTerminated;

	/*
	 * Destroy viewport if valid
	 */
	if (sfValid == (flags & sfValid))
		Destroy();

	/*
	 * Destroy QApplication
	 */
	delete application, application = 0;
	flags &= ~(sfApplication);

	/* sanity check */
	assert(flags == sfTerminated && "We seem to have some stray flags!");

	flags = sfUninitialized;
}

void
Qt4Backend::Reset(int state)
{
	flags = state;

	vsize.zero();
	wsize.zero();

	window = 0;
}

bool
Qt4Backend::Create(const Display &display)
{
	const QRect &desktop = QApplication::desktop()->availableGeometry();

	dpy = display;

	/*
	 * Backend Size
	 */
	if (dpy.fullscreen) {
		wsize.set(desktop.width(), desktop.height());
#if MARSHMALLOW_GRAPHICS_LOCK_WIDTH
		vsize.width = static_cast<float>(dpy.width);
		vsize.height = (vsize.width * static_cast<float>(wsize.height)) /
		    static_cast<float>(wsize.width);
#else
		vsize.height = static_cast<float>(dpy.height);
		vsize.width = (vsize.height * static_cast<float>(wsize.width)) /
		    static_cast<float>(wsize.height);
#endif
	}
	else {
		wsize.set(dpy.width, dpy.height);
		vsize.set(static_cast<float>(dpy.width), static_cast<float>(dpy.height));
	}

	/*
	 * Create Widget
	 */
	const int l_swap_interval = static_cast<int>(dpy.vsync) - 1;
	QGLFormat l_format(QGL::DoubleBuffer
	                  |QGL::DirectRendering
	                  |QGL::NoDepthBuffer
	                  |QGL::Rgba);
	l_format.setDepthBufferSize(0);
	l_format.setRedBufferSize(0);
	l_format.setGreenBufferSize(0);
	l_format.setBlueBufferSize(0);
	l_format.setAlphaBufferSize(0);
	l_format.setSwapInterval(l_swap_interval);
	l_format.setVersion(2, 0);

	window = new Widget(l_format);
	window->setFixedSize(wsize.width, wsize.height);

	/* check swap interval */
	if (dpy.vsync > 0 && window->format().swapInterval() != l_swap_interval)
		MMERROR("QT4: Swap interval request was ignored! " );

	/*
	 * We need to show the widget so it creates the context
	 */
	if (dpy.fullscreen)
		window->showFullScreen();
	else {
		window->move((desktop.width()  / 2) - (wsize.width  / 2),
		             (desktop.height() / 2) - (wsize.height / 2));

		window->show();
	}
	window->setFocus();

	/* move cursor */

	QCursor::setPos(window->mapToGlobal(QPoint(wsize.width, wsize.height)));

	/* validity check */
	if (!window->isValid()) {
		MMERROR("QT4: Failed to create a valid GL context.");
		delete window, window = 0;
		return(false);
	}
	
	flags |= sfWidget|sfExposed;

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
Qt4Backend::Destroy(void)
{
	/* check for valid state */
	if (sfValid != (flags & sfValid))
		return;

	flags &= ~(sfReady);

	/* hide viewport if exposed */
	if (sfExposed == (flags & sfExposed))
		Hide();

	Painter::Finalize();

	delete window, window = 0;
	flags ^= sfWidget;

	/* sanity check */
	assert(0 == (flags & ~(sfApplication|sfTerminated))
	    && "We seem to have some stray flags!");

	Reset(flags & (sfApplication|sfTerminated));
}

bool
Qt4Backend::Show(void)
{
	/* sanity checks */

	if (sfValid != (flags & sfValid)) {
	    assert(false && "Valid viewport was expected!");
	    return(false);
	}

	if (sfExposed == (flags & sfExposed))
		return(true);

	if (dpy.fullscreen)
		window->showFullScreen();
	else
		window->show();

	window->setFocus();

	return(flags |= sfExposed);
}

void
Qt4Backend::Hide(void)
{
	/* sanity checks */

	if (sfValid != (flags & sfValid)) {
	    assert(false && "Valid viewport was expected!");
	    return;
	}

	if (0 == (flags & sfExposed))
		return;

	window->hide();

	flags &= ~(sfExposed|sfFocused|sfKeyboardFocus|sfMouseFocus);
}

} /********************************** Graphics::OpenGL::<anonymous> Namespace */

PFNPROC
glGetProcAddress(const char *f)
{
	union {
		PFNPROC fptr;
		void    *ptr;
	} conv;

	assert(Qt4Backend::window
	    && Qt4Backend::window->context()
	    && "Invalid GL context!");

	conv.ptr = Qt4Backend::window->context()->getProcAddress(f);
	return(conv.fptr);
}

} /*********************************************** Graphics::OpenGL Namespace */

bool
Backend::Active(void)
{
	using namespace OpenGL::Qt4Backend;
	return(sfActive == (flags & sfActive));
}

bool
Backend::Initialize(void)
{
	using namespace OpenGL;
	return(Qt4Backend::Initialize());
}

void
Backend::Finalize(void)
{
	using namespace OpenGL;
	Qt4Backend::Finalize();
}

bool
Backend::Setup(const Graphics::Display &display)
{
	using namespace OpenGL;
	Qt4Backend::Destroy();

	if (!Qt4Backend::Create(display)) {
		Qt4Backend::Destroy();
		return(false);
	}

	return(Qt4Backend::Show());
}

void
Backend::Tick(float)
{
	using namespace OpenGL;
	Qt4Backend::application->processEvents();
}

void
Backend::Finish(void)
{
	using namespace OpenGL;
	Qt4Backend::window->swapBuffers();
	Painter::Reset();
}

const Graphics::Display &
Backend::Display(void)
{
	using namespace OpenGL;
	return(Qt4Backend::dpy);
}

const Math::Size2f &
Backend::Size(void)
{
	using namespace OpenGL;
	return(Qt4Backend::vsize);
}

const Math::Size2i &
Backend::WindowSize(void)
{
	using namespace OpenGL;
	return(Qt4Backend::wsize);
}

const Core::Identifier &
Backend::Implementation(void)
{
	static const Core::Identifier s_implementation("QT4");
	return(s_implementation);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

