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

#include "graphics/painter.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"
#include "core/platform.h"
#include "core/type.h"

#include "math/point2.h"

#include "graphics/quadmesh.h"
#include "graphics/transform.h"
#include "graphics/viewport.h"

#include "texturecoordinatedata.h"
#include "texturedata.h"
#include "vertexdata.h"

#include <SDL_video.h>

MARSHMALLOW_NAMESPACE_USE
using namespace Graphics::SDL;
using namespace Graphics;

/******************************************************************************/

namespace
{
	static Core::Identifier s_last_texture_id;

	void
	DrawQuadMesh(SDL_Surface *s, const QuadMesh &g, const Math::Point2 &o)
	{
		SDL::SharedVertexData l_vdata =
			g.vertexData()
			    .staticCast<SDL::VertexData>();

		if (!l_vdata) {
			MMVERBOSE("Ignoring mesh without vertex data.");
			return;
		}

		SDL::SharedTextureData l_tdata =
			g.textureData()
			    .staticCast<SDL::TextureData>();

		SDL::SharedTextureCoordinateData l_tcdata =
			g.textureCoordinateData()
			    .staticCast<SDL::TextureCoordinateData>();

		SDL_Rect l_drect;

		/* build destination rect */
		if (l_vdata && !l_vdata->asRect(o, l_drect)) {
			MMVERBOSE("Ignoring mesh with invalid vertex data.");
			return;
		}

		/* blit texture */
		if (l_tdata && l_tdata->isLoaded()) {
			SDL_Surface *l_img = l_tdata->surface();
			SDL_Rect l_srect;

			/* build source rect */
			if (l_tcdata && !l_tcdata->asRect(l_img->w, l_img->h, l_srect)) {
				MMVERBOSE("Ignoring mesh with invalid texture coordinate data.");
				return;
			}

			while (SDL_BlitSurface(l_img, l_tcdata ? &l_srect : 0, s, &l_drect) == -2) {
				while (SDL_LockSurface(l_img) < 0)
					Core::Platform::Sleep(10);
				SDL_UnlockSurface(l_img);
			}
		}
		/* draw rect */
		else {
			const Color &l_color = g.color();
			SDL_FillRect(s, &l_drect,
			    SDL_MapRGBA(s->format,
			                static_cast<Uint8>(static_cast<int>(l_color.red() * 255) % 256),
			                static_cast<Uint8>(static_cast<int>(l_color.green() * 255) % 256),
			                static_cast<Uint8>(static_cast<int>(l_color.blue() * 255) % 256),
			                static_cast<Uint8>(static_cast<int>(l_color.alpha() * 255) % 256)));
		}
	}
} // namespace

extern SDL_Surface * MM_SDL_Surface(void);

/******************************************************************************/

void
Painter::Initialize(void)
{
	SDL_Surface *l_surface = MM_SDL_Surface();

	/* TODO: Make color key configurable. */
	SDL_SetColorKey(l_surface, SDL_SRCCOLORKEY, SDL_MapRGB(l_surface->format, 0xFF, 0x00, 0xFF));
}

void
Painter::Finalize(void)
{
}

void
Painter::Draw(const IMesh &m, const Math::Point2 &o)
{
	if (m.rotation() > 0)
		MMWARNING("Tried to render mesh with an angle value using SDL!");

	SDL_Surface *l_surface = MM_SDL_Surface();

	/* actually draw graphic */
	if (QuadMesh::Type() == m.type())
		DrawQuadMesh(l_surface, static_cast<const QuadMesh &>(m), o - Viewport::Camera().translation());
	else MMWARNING("Unknown mesh type");
}

