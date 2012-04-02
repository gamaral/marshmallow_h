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

#include "texturedata.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/logger.h"

#ifdef WITH_SDL_IMAGE
#  include <SDL_image.h>
#else
#  include <SDL_video.h>
#endif

MARSHMALLOW_NAMESPACE_USE
using namespace Graphics;
using namespace SDL;

TextureData::TextureData(void)
    : m_id(),
      m_size(),
      m_surface(0)
{
}

TextureData::~TextureData(void)
{
	unload();
}

bool
TextureData::load(const Core::Identifier &i)
{
	if (m_surface) {
		MMERROR("Load texture asset called on active texture.");
		return(false);
	}

	SDL_Surface *l_surface = 0;
#ifdef WITH_SDL_IMAGE
	if((l_surface = IMG_Load(i)) == 0)
#else
	if((l_surface = SDL_LoadBMP(i)) == 0)
#endif
	{
		MMERROR("Failed to load texture (" << i.str() << ").");
		return(false);
	}
	
#ifdef WITH_SDL_IMAGE
	m_surface = SDL_DisplayFormatAlpha(l_surface);
#else
	m_surface = SDL_DisplayFormat(l_surface);
#endif
	SDL_FreeSurface(l_surface);
	
	m_size = Math::Size2i(m_surface->w, m_surface->h);

	MMINFO("Texture loaded.");

	return(true);
}

void
TextureData::unload(void)
{
	if (m_surface)
		SDL_FreeSurface(m_surface);

	m_size = Math::Size2i::Zero();
	m_surface = 0;

	MMINFO("Texture unloaded.");
}

const Core::Type &
TextureData::Type(void)
{
	static const Core::Type sType("Graphics::TextureData");
	return(sType);
}

