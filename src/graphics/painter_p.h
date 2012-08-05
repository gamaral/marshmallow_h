/*
 * Copyright 2012 Marshmallow Engine. All rights reserved.
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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef MARSHMALLOW_GRAPHICS_PAINTER_P_H
#define MARSHMALLOW_GRAPHICS_PAINTER_P_H 1

#include "graphics/painter.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */

/**** IMPLEMENTATION NOTES *****************************************************
 *
 *  Painter::Initialize and Painter::Finalize may be called multiple times
 *  by the Viewport.
 *
 */
namespace Painter { /**************************** Graphics::Painter Namespace */

	/*
	 * Painter::Initialized is called after getting a valid display by the
	 * Viewport implementation, all resources constructed here must be
	 * cleanly destroyed during Painter::Finalize.
	 */
	MARSHMALLOW_GRAPHICS_EXPORT
	void Initialize(void);

	/*
	 * Painter::Finalize is called prior to display destruction by the
	 * Viewport, all resources should be cleanly destroyed.
	 *
	 * The Painter must be ready to get reinitialized at any moment.
	 */
	MARSHMALLOW_GRAPHICS_EXPORT
	void Finalize(void);

	/*
	 * Painter::Render is called pre-rendering.
	 */
	MARSHMALLOW_GRAPHICS_EXPORT
	void Render(void);

	/*
	 * Painter::Reset is called post-rendering. The painter should
	 * automatically reload the View Projection Matrix at this time.
	 */
	MARSHMALLOW_GRAPHICS_EXPORT
	void Reset(void);

} /********************************************** Graphics::Painter Namespace */
} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
