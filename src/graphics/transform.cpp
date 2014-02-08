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

#include "graphics/transform.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "math/matrix4.h"
#include "math/point2.h"
#include "math/size2.h"

#include <cmath>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */

struct Transform::Private
{
	bool invalidated;

	float rotation;
	Math::Size2f scale;
	Math::Point2 translation;

	Math::Matrix4 matrix;

	void rebuildMatrix(MatrixType type);
};

void
Transform::Private::rebuildMatrix(MatrixType type)
{
	using namespace Math;

	Matrix4 l_rotate;
	Matrix4 l_scale;
	Matrix4 l_translate;

	if (Float(rotation) != Float::Zero()) {
#define DEGREE_TO_RADIAN 0.0174532925f
		const float l_rotation_rad = Float(rotation) != Float::Zero() ? rotation * DEGREE_TO_RADIAN : 0;
		l_rotate[Matrix4::m11] =  cosf(l_rotation_rad);
		l_rotate[Matrix4::m21] =  sinf(l_rotation_rad);
		l_rotate[Matrix4::m12] = -l_rotate[Matrix4::m21];
		l_rotate[Matrix4::m22] =  l_rotate[Matrix4::m11];
	}

	l_scale[Matrix4::m11] = scale.width;
	l_scale[Matrix4::m22] = scale.height;

	switch (type) {
	case Model:
		l_translate[Matrix4::m14] = translation.x;
		l_translate[Matrix4::m24] = translation.y;

		matrix = l_translate * l_scale * l_rotate;
		break;

	case View:
		l_translate[Matrix4::m14] = -translation.x;
		l_translate[Matrix4::m24] = -translation.y;

		matrix = l_scale * l_rotate * l_translate;
		break;
	default: break;
	}

	invalidated = false;
}

Transform::Transform(void)
    : PIMPL_CREATE
{
	PIMPL->invalidated = true;
	PIMPL->rotation = .0f;
	PIMPL->scale = Math::Size2f::Identity();
	PIMPL->translation = Math::Point2::Zero();
}

Transform::Transform(const Transform &other)
    : PIMPL_CREATE
{
	PIMPL->invalidated = other.PIMPL->invalidated;
	PIMPL->rotation = other.PIMPL->rotation;
	PIMPL->scale = other.PIMPL->scale;
	PIMPL->translation = other.PIMPL->translation;
	PIMPL->matrix = other.PIMPL->matrix;
}

Transform::~Transform(void)
{
	PIMPL_DESTROY;
}

float
Transform::rotation(void) const
{
	return(PIMPL->rotation);
}

void
Transform::setRotation(float value)
{
	PIMPL->rotation = value;
	PIMPL->invalidated = true;
}

const Math::Size2f &
Transform::scale(void) const
{
	return(PIMPL->scale);
}

void
Transform::setScale(const Math::Size2f &value)
{
	PIMPL->scale = value;
	PIMPL->invalidated = true;
}

void
Transform::setScale(float w, float h)
{
	PIMPL->scale.set(w, h);
	PIMPL->invalidated = true;
}

const Math::Point2 &
Transform::translation(void) const
{
	return(PIMPL->translation);
}

void
Transform::setTranslation(const Math::Point2 &value)
{
	PIMPL->translation = value;
	PIMPL->invalidated = true;
}

void
Transform::setTranslation(float x, float y)
{
	PIMPL->translation.set(x, y);
	PIMPL->invalidated = true;
}

const Math::Matrix4 &
Transform::matrix(MatrixType type) const
{
	if (PIMPL->invalidated)
		PIMPL->rebuildMatrix(type);

	return(PIMPL->matrix);
}

Transform &
Transform::operator =(const Transform &rhs)
{
	if (this != &rhs) {
		PIMPL->invalidated = rhs.PIMPL->invalidated;
		PIMPL->rotation = rhs.PIMPL->rotation;
		PIMPL->scale = rhs.PIMPL->scale;
		PIMPL->translation = rhs.PIMPL->translation;
		PIMPL->matrix = rhs.PIMPL->matrix;
	}
	return(*this);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

