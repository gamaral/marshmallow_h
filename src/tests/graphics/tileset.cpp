/*
 * Copyright 2011 Marshmallow Engine. All rights reserved.
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

#include "core/identifier.h"
#include "graphics/dummy/texturedata.h"
#include "graphics/itexturecoordinatedata.h"
#include "graphics/tileset.h"
#include "math/size2.h"
#include "../common.h"

#include <iostream>

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

MARSHMALLOW_NAMESPACE_USE;

void
tileset_fixed_test(void)
{
	Graphics::Dummy::SharedTextureData l_textureData = new Graphics::Dummy::TextureData;
	l_textureData->load("128x128");
	l_textureData->setSize(Math::Size2i(128, 128));

	Graphics::Tileset l_tileset;
	l_tileset.setTileSize(Math::Size2i(16, 16));
	l_tileset.setTextureData(l_textureData.staticCast<Graphics::ITextureData>());

	Graphics::SharedTextureCoordinateData tcd;
	float l_u, l_v;

	tcd = l_tileset.getTextureCoordinateData(0);
	tcd->get(0, l_u, l_v);
	TEST("TILESET 128-16 FIXED INDEX 0 TEST 0",
	    (l_u == 0.000625f) && (l_v == 0.000625f));
	tcd->get(1, l_u, l_v);
	TEST("TILESET 128-16 FIXED INDEX 0 TEST 1",
	    (l_u == 0.000625f) && (l_v == 0.124375f));
	tcd->get(2, l_u, l_v);
	TEST("TILESET 128-16 FIXED INDEX 0 TEST 2",
	    (l_u == 0.124375f) && (l_v == 0.000625f));
	tcd->get(3, l_u, l_v);
	TEST("TILESET 128-16 FIXED INDEX 0 TEST 3",
	    (l_u == 0.124375f) && (l_v == 0.124375f));

	tcd = l_tileset.getTextureCoordinateData(63);
	tcd->get(0, l_u, l_v);
	TEST("TILESET 128-16 FIXED INDEX 63 TEST 0",
	    (l_u == 0.875625f) && (l_v == 0.875625f));
	tcd->get(1, l_u, l_v);
	TEST("TILESET 128-16 FIXED INDEX 63 TEST 1",
	    (l_u == 0.875625f) && (l_v == 0.999375f));
	tcd->get(2, l_u, l_v);
	TEST("TILESET 128-16 FIXED INDEX 63 TEST 2",
	    (l_u == 0.999375f) && (l_v == 0.875625f));
	tcd->get(3, l_u, l_v);
	TEST("TILESET 128-16 FIXED INDEX 63 TEST 3",
	    (l_u == 0.999375f) && (l_v == 0.999375f));
}

void
tileset_spacing_test(void)
{
	Graphics::Dummy::SharedTextureData l_textureData = new Graphics::Dummy::TextureData;
	l_textureData->load("126x126");
	l_textureData->setSize(Math::Size2i(126, 126));

	Graphics::Tileset l_tileset;
	l_tileset.setTileSize(Math::Size2i(14, 14));
	l_tileset.setSpacing(2);
	l_tileset.setTextureData(l_textureData.staticCast<Graphics::ITextureData>());

	Graphics::SharedTextureCoordinateData tcd;
	float l_u, l_v;

	tcd = l_tileset.getTextureCoordinateData(0);
	tcd->get(0, l_u, l_v);
	TEST("TILESET 128-14 SPACING 2 INDEX 0 TEST 0",
	    (l_u == 0.000714285707f) && (l_v == 0.000714285707f));
	tcd->get(1, l_u, l_v);
	TEST("TILESET 128-14 SPACING 2 INDEX 0 TEST 1",
	    (l_u == 0.000714285707f) && (l_v == 0.110396832f));
	tcd->get(2, l_u, l_v);
	TEST("TILESET 128-14 SPACING 2 INDEX 0 TEST 2",
	    (l_u == 0.110396832f) && (l_v == 0.000714285707f));
	tcd->get(3, l_u, l_v);
	TEST("TILESET 128-14 SPACING 2 INDEX 0 TEST 3",
	    (l_u == 0.110396832f) && (l_v == 0.110396832f));

	tcd = l_tileset.getTextureCoordinateData(63);
	tcd->get(0, l_u, l_v);
	TEST("TILESET 128-14 SPACING 2 INDEX 63 TEST 0",
	    (l_u == 0.889603198f) && (l_v == 0.889603198f));
	tcd->get(1, l_u, l_v);
	TEST("TILESET 128-14 SPACING 2 INDEX 63 TEST 1",
	    (l_u == 0.889603198f) && (l_v == 0.999285698f));
	tcd->get(2, l_u, l_v);
	TEST("TILESET 128-14 SPACING 2 INDEX 63 TEST 2",
	    (l_u == 0.999285698f) && (l_v == 0.889603198f));
	tcd->get(3, l_u, l_v);
	TEST("TILESET 128-14 SPACING 2 INDEX 63 TEST 3",
	    (l_u == 0.999285698f) && (l_v == 0.999285698f));
}

void
tileset_margin_test(void)
{
	Graphics::Dummy::SharedTextureData l_textureData = new Graphics::Dummy::TextureData;
	l_textureData->load("130x130");
	l_textureData->setSize(Math::Size2i(130, 130));

	Graphics::Tileset l_tileset;
	l_tileset.setTileSize(Math::Size2i(16, 16));
	l_tileset.setMargin(2);
	l_tileset.setTextureData(l_textureData.staticCast<Graphics::ITextureData>());

	Graphics::SharedTextureCoordinateData tcd;
	float l_u, l_v;

	tcd = l_tileset.getTextureCoordinateData(0);
	tcd->get(0, l_u, l_v);
	TEST("TILESET 130-16 MARGIN 2 INDEX 0 TEST 0",
	    (l_u == 0.0160096157f) && (l_v == 0.0160096157f));
	tcd->get(1, l_u, l_v);
	TEST("TILESET 130-16 MARGIN 2 INDEX 0 TEST 1",
	    (l_u == 0.0160096157f) && (l_v == 0.137836546f));
	tcd->get(2, l_u, l_v);
	TEST("TILESET 130-16 MARGIN 2 INDEX 0 TEST 2",
	    (l_u == 0.137836546f) && (l_v == 0.0160096157f));
	tcd->get(3, l_u, l_v);
	TEST("TILESET 130-16 MARGIN 2 INDEX 0 TEST 3",
	    (l_u == 0.137836546f) && (l_v == 0.137836546f));

	tcd = l_tileset.getTextureCoordinateData(63);
	tcd->get(0, l_u, l_v);
	TEST("TILESET 130-16 MARGIN 2 INDEX 63 TEST 0",
	    (l_u == 0.877548099f) && (l_v == 0.877548099f));
	tcd->get(1, l_u, l_v);
	TEST("TILESET 130-16 MARGIN 2 INDEX 63 TEST 1",
	    (l_u == 0.877548099f) && (l_v == 0.999374986f));
	tcd->get(2, l_u, l_v);
	TEST("TILESET 130-16 MARGIN 2 INDEX 63 TEST 2",
	    (l_u == 0.999374986f) && (l_v == 0.877548099f));
	tcd->get(3, l_u, l_v);
	TEST("TILESET 130-16 MARGIN 2 INDEX 63 TEST 3",
	    (l_u == 0.999374986f) && (l_v == 0.999374986f));
}

int
MMain(int argc, char *argv[])
{
	UNUSED(argc);
	UNUSED(argv);

	tileset_fixed_test();
	tileset_spacing_test();
	tileset_margin_test();

	return(TEST_EXITCODE);
}

