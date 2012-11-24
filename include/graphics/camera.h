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

#ifndef MARSHMALLOW_GRAPHICS_CAMERA_H
#define MARSHMALLOW_GRAPHICS_CAMERA_H 1

#include <core/fd.h>

#include <math/point2.h>
#include <math/size2.h>

#include <graphics/config.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */

class Transform;

/*! @brief Graphics Camera Interface */
namespace Camera { /****************************** Graphics::Camera Namespace */

	MARSHMALLOW_GRAPHICS_EXPORT
	void Reset(void);

	MARSHMALLOW_GRAPHICS_EXPORT
	void Update(void);

	MARSHMALLOW_GRAPHICS_EXPORT
	const Graphics::Transform & Transform(void);

	MARSHMALLOW_GRAPHICS_EXPORT
	void SetTransform(const Graphics::Transform &transform);

	MARSHMALLOW_GRAPHICS_EXPORT
	const Math::Point2 & Position(void);

	MARSHMALLOW_GRAPHICS_EXPORT
	void SetPosition(const Math::Point2 &position);

	MARSHMALLOW_GRAPHICS_EXPORT
	void SetPosition(float x, float y);

	MARSHMALLOW_GRAPHICS_EXPORT
	float Rotation(void);

	MARSHMALLOW_GRAPHICS_EXPORT
	void SetRotation(float rotation);

	MARSHMALLOW_GRAPHICS_EXPORT
	const Math::Size2f & Zoom(void);

	MARSHMALLOW_GRAPHICS_EXPORT
	void SetZoom(const Math::Size2f &zoom);

	MARSHMALLOW_GRAPHICS_EXPORT
	void SetZoom(float zoom);

	MARSHMALLOW_GRAPHICS_EXPORT
	float VisibleMagnitude2(void);

	MARSHMALLOW_GRAPHICS_EXPORT
	const Math::Size2f & Visiblility(void);

} /*********************************************** Graphics::Camera Namespace */
} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
