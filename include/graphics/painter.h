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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef MARSHMALLOW_GRAPHICS_PAINTER_H
#define MARSHMALLOW_GRAPHICS_PAINTER_H 1

#include <core/environment.h>
#include <core/global.h>
#include <core/namespace.h>

#include <graphics/config.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Math { /******************************************** Math Namespace */
	class Matrix4;
	class Point2;
} /*********************************************************** Math Namespace */

namespace Graphics { /************************************ Graphics Namespace */

	class Color;
	struct IMesh;

/*! @brief Graphics Painter Interface */
namespace Painter { /**************************** Graphics::Painter Namespace */

	MARSHMALLOW_GRAPHICS_EXPORT
	void Initialize(void);

	MARSHMALLOW_GRAPHICS_EXPORT
	void Finalize(void);

	MARSHMALLOW_GRAPHICS_EXPORT
	void Render(void);

	MARSHMALLOW_GRAPHICS_EXPORT
	void Reset(void);

	MARSHMALLOW_GRAPHICS_EXPORT
	const Color & BackgroundColor(void);

	MARSHMALLOW_GRAPHICS_EXPORT
	void SetBackgroundColor(const Color &color);

	MARSHMALLOW_GRAPHICS_EXPORT
	Math::Matrix4 & Matrix(void);

	MARSHMALLOW_GRAPHICS_EXPORT
	void LoadIdentity(void);

	MARSHMALLOW_GRAPHICS_EXPORT
	void LoadProjection(void);

	MARSHMALLOW_GRAPHICS_EXPORT
	void PushMatrix(void);

	MARSHMALLOW_GRAPHICS_EXPORT
	void PopMatrix(void);

	/*!
	 * Draw a single mesh
	 */
	MARSHMALLOW_GRAPHICS_EXPORT
	void Draw(const IMesh &mesh, const Math::Point2 &origin);

	/*!
	 * Draw a single mesh in multiple origins
	 */
	MARSHMALLOW_GRAPHICS_EXPORT
	void Draw(const IMesh &mesh, const Math::Point2 *origins, int count = 1);

} /********************************************** Graphics::Painter Namespace */
} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
