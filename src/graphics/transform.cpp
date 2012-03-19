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

#include "math/pair.h"
#include "math/point2.h"

MARSHMALLOW_NAMESPACE_USE
using namespace Graphics;

struct Transform::Private
{
	float rotation;
	Math::Pair scale;
	Math::Point2 translation;
};

Transform::Transform(void)
    : m_p(new Private)
{
	m_p->rotation = 0.f;
}

Transform::Transform(const Transform &other)
    : m_p(new Private)
{
	m_p->rotation = 0.f;
	m_p->scale = other.m_p->scale;
	m_p->translation = other.m_p->translation;
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

const Math::Pair &
Transform::scale(void) const
{
	return(m_p->scale);
}

const Math::Point2 &
Transform::translation(void) const
{
	return(m_p->translation);
}

Transform &
Transform::operator =(const Transform& rhs)
{
	if (this != &rhs) {
		m_p->rotation = rhs.m_p->rotation;
		m_p->scale = rhs.m_p->scale;
		m_p->translation = rhs.m_p->translation;
	}
	return(*this);
}

void
Transform::setScale(const Math::Pair &value)
{
	m_p->scale = value;
}

void
Transform::setTranslation(const Math::Point2 &value)
{
	m_p->translation = value;
}

void
Transform::setRotation(float value)
{
	m_p->rotation = value;
}

