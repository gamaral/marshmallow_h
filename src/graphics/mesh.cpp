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

#include "graphics/mesh.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/logger.h"
#include "core/type.h"

#include "graphics/itexturecoordinatedata.h"
#include "graphics/itexturedata.h"
#include "graphics/ivertexdata.h"

#include <cstring>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { /************************************ Graphics Namespace */
namespace { /******************************** Graphics::<anonymous> Namespace */

	static const char * s_scalemode_string[ITextureData::smModes] =
	    { "nearest", "linear" };

	ITextureData::ScaleMode
	StringToScaleMode(const char *str)
	{
		using namespace Graphics;

		if (!str) return(ITextureData::smDefault);

		for (int i = 0; i < ITextureData::smModes; ++i)
			if (MMSTRCASECMP(str, s_scalemode_string[i]) == 0)
				return(static_cast<ITextureData::ScaleMode>(i));

		return(ITextureData::smDefault);
	}

	const char *
	ScaleModeToString(ITextureData::ScaleMode mode)
	{
		return(s_scalemode_string[mode]);
	}

} /****************************************** Graphics::<anonymous> Namespace */

/* TODO: cleanup */
struct Mesh::Private
{
	Private(ITextureCoordinateData *tc,
	        ITextureData *t,
	        IVertexData *v,
	        int f)
	    : tcdata(tc)
	    , tdata(t)
	    , vdata(v)
	    , rotation(0)
	    , flags(static_cast<char>(f))
	{
		scale[0] = scale[1] = 1.f;
	}

	~Private(void);

	Color color;
	ITextureCoordinateData *tcdata;
	ITextureData *tdata;
	IVertexData *vdata;
	float scale[2];
	float rotation;
	char  flags;
};

Mesh::Private::~Private(void)
{
	if (flags & mfTextureCoordinateFree)
		delete tcdata;
	tcdata = 0;

	if (flags & mfTextureDataFree)
		delete tdata;
	tdata = 0;

	if (flags & mfVertexDataFree)
		delete vdata;
	vdata = 0;
}

Mesh::Mesh(ITextureCoordinateData *tc, ITextureData *t, IVertexData *v, int f)
    : PIMPL_CREATE_X(tc, t, v, f)
{
}

Mesh::~Mesh(void)
{
	PIMPL_DESTROY;
}

void
Mesh::setColor(const Graphics::Color &c)
{
	PIMPL->color = c;
}

void
Mesh::setRotation(float a)
{
	PIMPL->rotation = a;
}

void
Mesh::setScale(float x, float y)
{
	PIMPL->scale[0] = x;
	PIMPL->scale[1] = y;
}

void
Mesh::setTextureCoordinateData(ITextureCoordinateData *tc)
{
	PIMPL->tcdata = tc;
}

void
Mesh::setTextureData(ITextureData *td)
{
	PIMPL->tdata = td;
}

void
Mesh::setVertexData(IVertexData *vd)
{
	PIMPL->vdata = vd;
}

ITextureCoordinateData *
Mesh::textureCoordinateData(void) const
{
	return(PIMPL->tcdata);
}

ITextureData *
Mesh::textureData(void) const
{
	return(PIMPL->tdata);
}

IVertexData *
Mesh::vertexData(void) const
{
	return(PIMPL->vdata);
}

const Graphics::Color &
Mesh::color(void) const
{
	return(PIMPL->color);
}

float
Mesh::rotation(void) const
{
	return(PIMPL->rotation);
}

void
Mesh::scale(float &x, float &y) const
{
	x = PIMPL->scale[0];
	y = PIMPL->scale[1];
}

int
Mesh::flags(void) const
{
	return(PIMPL->flags);
}

Math::Vector2
Mesh::vertex(uint16_t i) const
{
	Math::Vector2 l_vector;
	if (!PIMPL->vdata->get(i, l_vector.x, l_vector.y))
		MMWARNING("Failed to retrieve values for vertex " << i);
	return(l_vector);
}

void
Mesh::setVertex(uint16_t i, const Math::Vector2 &v)
{
	if (!PIMPL->vdata->set(i, v.x, v.y))
		MMWARNING("Failed to assign values (" << v.x << ", " << v.y << ") to vertex " << i);
}

void
Mesh::textureCoordinate(uint16_t i, float &u, float &v) const
{
	if (!PIMPL->tcdata->get(i, u, v))
		MMWARNING("Failed to retrieve values for vertex " << i);
}

void
Mesh::setTextureCoordinate(uint16_t i, float u, float v)
{
	if (!PIMPL->tcdata->set(i, u, v))
		MMWARNING("Failed to assign values (" << u << ", " << v << ") to texture coordinate " << i);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

