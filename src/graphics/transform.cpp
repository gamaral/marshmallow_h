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

MARSHMALLOW_NAMESPACE_USE
using namespace Graphics;

struct Transform::Private
{
	float rotation;
	Math::Size2f scale;
	Math::Point2 translation;

	Math::Matrix4 matrix;

	void updateMatrix(void);
};

void
Transform::Private::updateMatrix(void)
{
	Math::Matrix4 l_translate;
	l_translate[12] = translation.x();
	l_translate[13] = translation.y();

	Math::Matrix4 l_rotate;
	if (rotation != 0) {
#define DEGREE_TO_RADIAN 0.0174532925f
		float l_rotation_rad = rotation * DEGREE_TO_RADIAN;
		l_rotate[0] = cosf(l_rotation_rad);
		l_rotate[1] = sinf(l_rotation_rad);
		l_rotate[4] = -l_rotate[1];
		l_rotate[5] =  l_rotate[0];
	}

	Math::Matrix4 l_scale;
	l_scale[0] = scale.width();
	l_scale[5] = scale.height();

	matrix = l_scale * l_translate * l_rotate;
}

Transform::Transform(void)
    : m_p(new Private)
{
	m_p->rotation = 0.f;
	m_p->scale = Math::Size2f::Identity();
	m_p->updateMatrix();
}

Transform::Transform(const Transform &other)
    : m_p(new Private)
{
	m_p->rotation = 0.f;
	m_p->scale = other.m_p->scale;
	m_p->translation = other.m_p->translation;
	m_p->matrix = other.m_p->matrix;
}

Transform::~Transform(void)
{
	delete m_p;
	m_p = 0;
}

float
Transform::rotation(void) const
{
	return(m_p->rotation);
}

void
Transform::setRotation(float value)
{
	m_p->rotation = value;
	m_p->updateMatrix();
}

const Math::Size2f &
Transform::scale(void) const
{
	return(m_p->scale);
}

void
Transform::setScale(const Math::Size2f &value)
{
	m_p->scale = value;
	m_p->updateMatrix();
}

const Math::Point2 &
Transform::translation(void) const
{
	return(m_p->translation);
}

void
Transform::setTranslation(const Math::Point2 &value)
{
	m_p->translation = value;
	m_p->updateMatrix();
}

const Math::Matrix4 &
Transform::matrix(void) const
{
	return(m_p->matrix);
}

Transform &
Transform::operator =(const Transform& rhs)
{
	if (this != &rhs) {
		m_p->rotation = rhs.m_p->rotation;
		m_p->scale = rhs.m_p->scale;
		m_p->translation = rhs.m_p->translation;
		m_p->matrix = rhs.m_p->matrix;
	}
	return(*this);
}

