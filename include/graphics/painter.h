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
	struct Point2;
} /*********************************************************** Math Namespace */

namespace Graphics { /************************************ Graphics Namespace */

	class Color;
	struct IMesh;

/*! @brief Graphics Painter Interface */
namespace Painter { /**************************** Graphics::Painter Namespace */

	/*!
	 * Get current background color.
	 */
	MARSHMALLOW_GRAPHICS_EXPORT
	const Color & BackgroundColor(void);

	/*!
	 * Change reset background color.
	 */
	MARSHMALLOW_GRAPHICS_EXPORT
	void SetBackgroundColor(const Color &color);

	/*!
	 * Returns the current matrix.
	 */
	MARSHMALLOW_GRAPHICS_EXPORT
	Math::Matrix4 & Matrix(void);

	/*!
	 * Replace current matrix with the Identity Matrix.
	 */
	MARSHMALLOW_GRAPHICS_EXPORT
	void LoadIdentity(void);

	/*!
	 * Replace current matrix with the Projection Matrix (sans View Matrix).
	 */
	MARSHMALLOW_GRAPHICS_EXPORT
	void LoadProjection(void);

	/*!
	 * Replace current matrix with the View Projection Matrix.
	 */
	MARSHMALLOW_GRAPHICS_EXPORT
	void LoadViewProjection(void);

	/*!
	 * Push current matrix to the top of the matrix stack.
	 */
	MARSHMALLOW_GRAPHICS_EXPORT
	void PushMatrix(void);

	/*!
	 * Replace current matrix with the one at the top of the matrix stack.
	 */
	MARSHMALLOW_GRAPHICS_EXPORT
	void PopMatrix(void);

	/*!
	 * Draw a single mesh.
	 */
	MARSHMALLOW_GRAPHICS_EXPORT
	void Draw(const IMesh &mesh, const Math::Point2 &origin);

	/*!
	 * Draw a single mesh at multiple origin points.
	 */
	MARSHMALLOW_GRAPHICS_EXPORT
	void Draw(const IMesh &mesh, const Math::Point2 *origins, size_t count = 1);

} /********************************************** Graphics::Painter Namespace */
} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
