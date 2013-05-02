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

#include "graphics/tileset.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/logger.h"

#include "math/size2.h"

#include "graphics/factory.h"
#include "graphics/itexturecoordinatedata.h"
#include "graphics/itexturedata.h"

#include <cassert>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */

/* TODO cleanup */
struct Tileset::Private
{
	Private()
	    : name()
	    , size(16, 16)
	    , tile_size(16, 16)
	    , cache(0)
	    , texture_data(0)
	    , margin(0)
	    , spacing(0)
	    , offset_col(0)
	    , offset_row(0)
	{
		ptilesize[0] = ptilesize[1] = 0;
	}

	Core::Identifier  name;
	Math::Size2i      size;
	Math::Size2i      tile_size;

	ITextureCoordinateData **cache;
	ITextureData *texture_data;

	int margin;
	int spacing;

	float *offset_col;
	float *offset_row;
	float  ptilesize[2]; // proportional tile-size
};

Tileset::Tileset()
    : PIMPL_CREATE
{
}

Tileset::~Tileset(void)
{
	/* reset will clear out cache data and return */
	PIMPL->texture_data = 0;
	reset();

	PIMPL_DESTROY;
}

void
Tileset::setName(const Core::Identifier &n)
{
	PIMPL->name = n;
}

void
Tileset::setTileSize(const Math::Size2i &s)
{
	PIMPL->tile_size = s;
	reset();
}

void
Tileset::setMargin(int m)
{
	PIMPL->margin = m;
	reset();
}

void
Tileset::setSpacing(int s)
{
	PIMPL->spacing = s;
	reset();
}

const Core::Identifier &
Tileset::name(void) const
{
	return(PIMPL->name);
}

const Math::Size2i &
Tileset::size(void) const
{
	return(PIMPL->size);
}

ITextureData *
Tileset::textureData(void) const
{
	return(PIMPL->texture_data);
}

const Math::Size2i &
Tileset::tileSize(void) const
{
	return(PIMPL->tile_size);
}

int
Tileset::spacing(void) const
{
	return(PIMPL->spacing);
}

int
Tileset::margin(void) const
{
	return(PIMPL->margin);
}

void
Tileset::setTextureData(ITextureData *ts)
{
	if (ts == PIMPL->texture_data)
		return;

	/* replace texture */
	PIMPL->texture_data = ts;

	reset();
}

ITextureCoordinateData *
Tileset::getTextureCoordinateData(uint16_t i)
{
	if (!PIMPL->texture_data || !PIMPL->texture_data->isLoaded())
		return(0);

	assert(i < PIMPL->size.area() && "index was out of bounds.");
	ITextureCoordinateData *l_cached = PIMPL->cache[i];
	if (!l_cached) {
		/* create new entry */
#define TILECOORDINATES 4
		ITextureCoordinateData *l_data =
		    Backend::Factory::CreateTextureCoordinateData(TILECOORDINATES);

		/* calculate row and column */

		const float l_left   = PIMPL->offset_col[i % PIMPL->size.width];
		const float l_top    = PIMPL->offset_row[i / PIMPL->size.width];
		const float l_right  = l_left + PIMPL->ptilesize[0];
		const float l_bottom = l_top  + PIMPL->ptilesize[1];

		l_data->set(0, l_left,  l_top);
		l_data->set(1, l_left,  l_bottom);
		l_data->set(2, l_right, l_top);
		l_data->set(3, l_right, l_bottom);

		return(PIMPL->cache[i] = l_data);
	}

	return(l_cached);
}

void
Tileset::reset(void)
{
	/* clear current tile cache */

	if (PIMPL->cache) {
		const int l_old_item_count = PIMPL->size.area();
		for (int i = 0; i < l_old_item_count; ++i)
			delete PIMPL->cache[i];
		delete[] PIMPL->cache, PIMPL->cache = 0;
	}

	delete[] PIMPL->offset_col, PIMPL->offset_col  = 0;
	delete[] PIMPL->offset_row, PIMPL->offset_row  = 0;

	PIMPL->ptilesize[0] = PIMPL->ptilesize[1] = 0;
	PIMPL->size.zero();

	if (!PIMPL->texture_data)
		return;

	/* calculate size */

	/*
	 *  In order to get an accurate Tileset Size, we need to deduct the
	 *  Margin from the Raw Texture Size. This will give us the Real Texture
	 *  Size.
	 *
	 *      i.e. REAL = RAW - MARGIN
	 *
	 *  We now need to add the Tile Spacing to both the Tile Size and the
	 *  Real Texture Size. By artificially adding spacing to the Real Tile
	 *  Size we can get an accurate Tile Count since we are compensating for
	 *  the right-most tile spacing.
	 *
	 *  REAL = REAL + SPACING
	 *  TILE = TILE + SPACING
	 *
	 *  Finally we divide the Real Tile Size by the Tile Size, giving us the
	 *  total Tile Count also known as Tileset Size.
	 *
	 */
	const Math::Size2i &l_texture_size = PIMPL->texture_data->size();
	PIMPL->size.width = (l_texture_size.width - PIMPL->margin + PIMPL->spacing)
	          / (PIMPL->tile_size.width + PIMPL->spacing);
	PIMPL->size.height = (l_texture_size.height - PIMPL->margin + PIMPL->spacing)
	          / (PIMPL->tile_size.height + PIMPL->spacing);

	/*
	 * Calculate proportional tile sizes
	 */

	PIMPL->ptilesize[0] = static_cast<float>(PIMPL->tile_size.width)
	              / static_cast<float>(l_texture_size.width);
	PIMPL->ptilesize[1] = static_cast<float>(PIMPL->tile_size.height)
	              / static_cast<float>(l_texture_size.height);

	/*
	 * Calculate tile offsets
	 */

	int l_real_tile_size;

	PIMPL->offset_col = new float[PIMPL->size.width];
	l_real_tile_size = PIMPL->tile_size.width + PIMPL->spacing;
	for (int i = 0; i < PIMPL->size.width; ++i)
		PIMPL->offset_col[i] =
		    static_cast<float>(PIMPL->margin + (i * l_real_tile_size))
		  / static_cast<float>(l_texture_size.width);

	PIMPL->offset_row = new float[PIMPL->size.height];
	l_real_tile_size = PIMPL->tile_size.height + PIMPL->spacing;
	for (int i = 0; i < PIMPL->size.height; ++i)
		PIMPL->offset_row[i] =
		    static_cast<float>(PIMPL->margin + (i * l_real_tile_size))
		  / static_cast<float>(l_texture_size.height);

	const int l_item_count = PIMPL->size.area();
	PIMPL->cache = new ITextureCoordinateData *[l_item_count];
	for (int i = 0; i < l_item_count; ++i)
		PIMPL->cache[i] = 0;
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

