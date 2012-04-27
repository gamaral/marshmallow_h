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

#ifndef MARSHMALLOW_GRAPHICS_TRANSFORM_H
#define MARSHMALLOW_GRAPHICS_TRANSFORM_H 1

#include <core/iserializable.h>

#include <core/fd.h>

MARSHMALLOW_NAMESPACE_BEGIN

namespace Math
{
	class Tuple2;
	typedef Tuple2 Pair;

	class Point2;

	class Matrix4;
}

namespace Graphics
{
	/*! @brief Graphics Transform */
	class MARSHMALLOW_GRAPHICS_EXPORT
	Transform
	{
		struct Private;
		Private *m_p;
	public:
		Transform(void);
		Transform(const Transform &other);
		virtual ~Transform(void);
		
		Transform & operator =(const Transform& rhs);

		float rotation(void) const;
		void setRotation(float value);

		const Math::Pair & scale(void) const;
		void setScale(const Math::Pair &value);

		const Math::Point2 & translation(void) const;
		void setTranslation(const Math::Point2 &value);

		const Math::Matrix4 & matrix(void) const;
	};
	typedef Core::Shared<Transform> SharedTransform;
	typedef Core::Weak<Transform> WeakTransform;
}

MARSHMALLOW_NAMESPACE_END

#endif
