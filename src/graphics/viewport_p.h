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

#ifndef MARSHMALLOW_GRAPHICS_VIEWPORT_P_H
#define MARSHMALLOW_GRAPHICS_VIEWPORT_P_H 1

#include "graphics/viewport.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */

/**** IMPLEMENTATION NOTES *****************************************************
 *
 *  The Game::IEngine implementation used should first bootstrap the viewport by
 *  calling Viewport::Initialize. It may then can call Viewport::Setup to create
 *  the display desired.
 *
 *  It's important to keep in mind that Viewport::Setup can be called multiple
 *  times during game play, when changing from *fullscreen* to *windowed* mode
 *  for example, so plan accordingly.
 *
 *  Viewport::Setup is required to call Painter::Initialize after successful
 *  Viewport Display construction and Painter::Finalize after destruction.
 */
namespace Viewport { /************************** Graphics::Viewport Namespace */

	/*
	 * Viewport::Active is called prior to every render call, this is used
	 * to skip rendering if Viewport is busy or unavailable.
	 */
	MARSHMALLOW_GRAPHICS_EXPORT
	bool Active(void);

	/*
	 * Viewport::Initialized is called prior to Viewport::Setup, all
	 * resources constructed here must be cleanly destroyed during
	 * Viewport::Finalize.
	 */
	MARSHMALLOW_GRAPHICS_EXPORT
	bool Initialize(void);

	/*
	 * Viewport::Finalize is called right after game loop termination, all
	 * resources created during Viewport::Setup and Viewport::Initialize
	 * should be cleanly released here.
	 */
	MARSHMALLOW_GRAPHICS_EXPORT
	void Finalize(void);

	/*
	 * Viewport::Tick is called every frame, it can be used to process
	 * viewport related messages.
	 */
	MARSHMALLOW_GRAPHICS_EXPORT
	void Tick(float delta);

	/*
	 * Viewport::SwapBuffer is called after all rendering calls have
	 * finished. It should at least swap the display buffer.
	 */
	MARSHMALLOW_GRAPHICS_EXPORT
	void SwapBuffer(void);

} /********************************************* Graphics::Viewport Namespace */
} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
