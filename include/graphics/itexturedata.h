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

#ifndef MARSHMALLOW_GRAPHICS_ITEXTUREDATA_H
#define MARSHMALLOW_GRAPHICS_ITEXTUREDATA_H 1

#include <core/iasset.h>

#include <core/fd.h>
#include <graphics/config.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Math { /******************************************** Math Namespace */
	template <typename T> class Size2;
	typedef Size2<int> Size2i;
} /*********************************************************** Math Namespace */

namespace Graphics { /************************************ Graphics Namespace */

	/*! @brief Graphics Texture Data Interface */
	struct MARSHMALLOW_GRAPHICS_EXPORT
	ITextureData : public Core::IAsset
	{
		virtual ~ITextureData(void);

		enum ScaleMode
		{
			smNearest,
			smLinear,
			smModes,
			smDefault = smNearest
		};

		virtual bool load(const Core::Identifier &id,
		                  ScaleMode min = smDefault,
		                  ScaleMode mag = smDefault) = 0;
		virtual void unload(void) = 0;
		virtual bool isLoaded(void) const = 0;

		virtual const Math::Size2i & size(void) const = 0;
	};
	typedef Core::Shared<ITextureData> SharedTextureData;
	typedef Core::Weak<ITextureData> WeakTextureData;

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
