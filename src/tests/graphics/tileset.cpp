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

#include <iostream>

#include "core/identifier.h"

#include "math/size2.h"

#include "graphics/dummy/texturedata.h"
#include "graphics/itexturecoordinatedata.h"
#include "graphics/tileset.h"

#include "tests/common.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

MARSHMALLOW_NAMESPACE_USE

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
	    (l_u == 0.000040f) && (l_v == 0.000040f));
	tcd->get(1, l_u, l_v);
	TEST("TILESET 128-16 FIXED INDEX 0 TEST 1",
	    (l_u == 0.000040f) && (l_v == 0.124960f));
	tcd->get(2, l_u, l_v);
	TEST("TILESET 128-16 FIXED INDEX 0 TEST 2",
	    (l_u == 0.124960f) && (l_v == 0.000040f));
	tcd->get(3, l_u, l_v);
	TEST("TILESET 128-16 FIXED INDEX 0 TEST 3",
	    (l_u == 0.124960f) && (l_v == 0.124960f));

	tcd = l_tileset.getTextureCoordinateData(63);
	tcd->get(0, l_u, l_v);
	TEST("TILESET 128-16 FIXED INDEX 63 TEST 0",
	    (l_u == 0.875040f) && (l_v == 0.875040f));
	tcd->get(1, l_u, l_v);
	TEST("TILESET 128-16 FIXED INDEX 63 TEST 1",
	    (l_u == 0.875040f) && (l_v == 0.999960f));
	tcd->get(2, l_u, l_v);
	TEST("TILESET 128-16 FIXED INDEX 63 TEST 2",
	    (l_u == 0.999960f) && (l_v == 0.875040f));
	tcd->get(3, l_u, l_v);
	TEST("TILESET 128-16 FIXED INDEX 63 TEST 3",
	    (l_u == 0.999960f) && (l_v == 0.999960f));
}

int
MMain(int argc, char *argv[])
{
	MMUNUSED(argc);
	MMUNUSED(argv);

	tileset_fixed_test();

	return(TEST_EXITCODE);
}

