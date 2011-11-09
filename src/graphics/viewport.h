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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef GRAPHICS_VIEWPORT_H
#define GRAPHICS_VIEWPORT_H 1

#include "core/type.h"

#include "math/point2.h"
#include "math/size2.h"
#include "math/triplet.h"

#include "graphics/config.h"

MARSHMALLOW_NAMESPACE_BEGIN

namespace Graphics
{

	class Transform;

	/*! @brief Graphics Viewport Interface */
	namespace Viewport
	{
		GRAPHICS_EXPORT
		const Core::Type & Type(void);

		GRAPHICS_EXPORT
		bool Initialize(int width = DEFAULT_VIEWPORT_WIDTH,
		               int height = DEFAULT_VIEWPORT_HEIGHT,
		                int depth = DEFAULT_VIEWPORT_DEPTH,
		          bool fullscreen = DEFAULT_VIEWPORT_FULLSCREEN);

		GRAPHICS_EXPORT
		void Finalize(void);

		GRAPHICS_EXPORT
		bool Redisplay(int width = DEFAULT_VIEWPORT_WIDTH,
		              int height = DEFAULT_VIEWPORT_HEIGHT,
		               int depth = DEFAULT_VIEWPORT_DEPTH,
		         bool fullscreen = DEFAULT_VIEWPORT_FULLSCREEN);

		GRAPHICS_EXPORT
		void Tick(void);

		GRAPHICS_EXPORT
		void SwapBuffer(void);

		GRAPHICS_EXPORT
		const Graphics::Transform & Camera(void);

		GRAPHICS_EXPORT
		void SetCamera(const Graphics::Transform &camera);

		GRAPHICS_EXPORT
		float Radius2(void);

		GRAPHICS_EXPORT
		const Math::Size2f & ScaledSize(void);

		GRAPHICS_EXPORT
		const Math::Size2f & Size(void);

		GRAPHICS_EXPORT
		const Math::Size2i & WindowSize(void);

		GRAPHICS_EXPORT
		void SwapControl(bool sync = true);
	}

}

MARSHMALLOW_NAMESPACE_END

#endif
