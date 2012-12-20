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

#include "graphics/camera.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include "graphics/backend.h"
#include "graphics/transform.h"

#include <cmath>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */
namespace { /******************************** Graphics::<anonymous> Namespace */

struct CameraData
{
	Graphics::Transform transform;
	Math::Size2f visibility;
	float magnitude2;
} s_data;

void
UpdateMagnitude2(void)
{
	s_data.magnitude2 =
	    powf(s_data.visibility.width  / 2.f, 2.f) +
	    powf(s_data.visibility.height / 2.f, 2.f);
}

void
UpdateVisibility(void)
{
	using namespace Graphics;
	const Math::Size2f &l_size = Backend::Size();
	s_data.visibility = l_size / s_data.transform.scale();
}

} /****************************************** Graphics::<anonymous> Namespace */

void
Camera::Reset(void)
{
	s_data.transform.setRotation(.0f);
	s_data.transform.setScale(Math::Size2f::Identity());
	s_data.transform.setTranslation(Math::Point2::Zero());
	s_data.visibility = Backend::Size();
	UpdateMagnitude2();
}

void
Camera::Update(void)
{
	UpdateVisibility();
	UpdateMagnitude2();
}

const Graphics::Transform &
Camera::Transform(void)
{
	return(s_data.transform);
}

void
Camera::SetTransform(const Graphics::Transform &transform)
{
	s_data.transform = transform;
	Update();
}

const Math::Point2 &
Camera::Position(void)
{
	return(s_data.transform.translation());
}

void
Camera::SetPosition(const Math::Point2 &position)
{
	s_data.transform.setTranslation(position);
}

void
Camera::SetPosition(float x, float y)
{
	s_data.transform.setTranslation(x, y);
}

float
Camera::Rotation(void)
{
	return(s_data.transform.rotation());
}

void
Camera::SetRotation(float rotation)
{
	s_data.transform.setRotation(rotation);
}

const Math::Size2f &
Camera::Zoom(void)
{
	return(s_data.transform.scale());
}

void
Camera::SetZoom(const Math::Size2f &zoom)
{
	s_data.transform.setScale(zoom);
	Update();
}

void
Camera::SetZoom(float zoom)
{
	s_data.transform.setScale(zoom, zoom);
	Update();
}

float
Camera::VisibleMagnitude2(void)
{
	return(s_data.magnitude2);
}

const Math::Size2f &
Camera::Visiblility(void)
{
	return(s_data.visibility);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

