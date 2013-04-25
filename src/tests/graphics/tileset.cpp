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

#include <iostream>

#include "core/identifier.h"
#include "core/shared.h"

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
	ASSERT_TRUE("TILESET 128-16 FIXED INDEX 0 TEST 0",
	    (l_u == .0f) && (l_v == .0f));
	tcd->get(1, l_u, l_v);
	ASSERT_TRUE("TILESET 128-16 FIXED INDEX 0 TEST 1",
	    (l_u == .0f) && (l_v == .125f));
	tcd->get(2, l_u, l_v);
	ASSERT_TRUE("TILESET 128-16 FIXED INDEX 0 TEST 2",
	    (l_u == .125f) && (l_v == .0f));
	tcd->get(3, l_u, l_v);
	ASSERT_TRUE("TILESET 128-16 FIXED INDEX 0 TEST 3",
	    (l_u == .125f) && (l_v == .125f));

	tcd = l_tileset.getTextureCoordinateData(63);
	tcd->get(0, l_u, l_v);
	ASSERT_TRUE("TILESET 128-16 FIXED INDEX 63 TEST 0",
	    (l_u == .875f) && (l_v == .875f));
	tcd->get(1, l_u, l_v);
	ASSERT_TRUE("TILESET 128-16 FIXED INDEX 63 TEST 1",
	    (l_u == .875f) && (l_v == 1.f));
	tcd->get(2, l_u, l_v);
	ASSERT_TRUE("TILESET 128-16 FIXED INDEX 63 TEST 2",
	    (l_u == 1.f) && (l_v == .875f));
	tcd->get(3, l_u, l_v);
	ASSERT_TRUE("TILESET 128-16 FIXED INDEX 63 TEST 3",
	    (l_u == 1.f) && (l_v == 1.f));
}

TESTS_BEGIN
	TEST(tileset_fixed_test)
TESTS_END

