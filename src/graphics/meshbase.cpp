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

#include "graphics/meshbase.h"

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

static const char * s_scalemode_string[Graphics::ITextureData::smModes] =
    { "nearest", "linear" };

Graphics::ITextureData::ScaleMode
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
ScaleModeToString(Graphics::ITextureData::ScaleMode mode)
{
	return(s_scalemode_string[mode]);
}

} /****************************************** Graphics::<anonymous> Namespace */

/* TODO: cleanup */
struct MeshBase::Private
{
	Color color;
	ITextureCoordinateData *tcdata;
	ITextureData *tdata;
	IVertexData *vdata;
	float scale[2];
	float rotation;
};

MeshBase::MeshBase(ITextureCoordinateData *tc, ITextureData *t, IVertexData *v)
    : PIMPL_CREATE
{
	PIMPL->tcdata = tc;
	PIMPL->tdata = t;
	PIMPL->vdata = v;
	PIMPL->rotation = 0;
	PIMPL->scale[0] = PIMPL->scale[1] = 1.f;
}

MeshBase::~MeshBase(void)
{
	PIMPL_DESTROY;
}

void
MeshBase::setColor(const Graphics::Color &c)
{
	PIMPL->color = c;
}

void
MeshBase::setRotation(float a)
{
	PIMPL->rotation = a;
}

void
MeshBase::setScale(float x, float y)
{
	PIMPL->scale[0] = x;
	PIMPL->scale[1] = y;
}

void
MeshBase::setTextureCoordinateData(ITextureCoordinateData *tc)
{
	PIMPL->tcdata = tc;
}

void
MeshBase::setTextureData(ITextureData *td)
{
	PIMPL->tdata = td;
}

void
MeshBase::setVertexData(IVertexData *vd)
{
	PIMPL->vdata = vd;
}

ITextureCoordinateData *
MeshBase::textureCoordinateData(void) const
{
	return(PIMPL->tcdata);
}

ITextureData *
MeshBase::textureData(void) const
{
	return(PIMPL->tdata);
}

IVertexData *
MeshBase::vertexData(void) const
{
	return(PIMPL->vdata);
}

const Graphics::Color &
MeshBase::color(void) const
{
	return(PIMPL->color);
}

float
MeshBase::rotation(void) const
{
	return(PIMPL->rotation);
}

void
MeshBase::scale(float &x, float &y) const
{
	x = PIMPL->scale[0];
	y = PIMPL->scale[1];
}

Math::Vector2
MeshBase::vertex(uint16_t i) const
{
	Math::Vector2 l_vector;
	if (!PIMPL->vdata->get(i, l_vector.x, l_vector.y))
		MMWARNING("Failed to retrieve values for vertex " << i);
	return(l_vector);
}

void
MeshBase::setVertex(uint16_t i, const Math::Vector2 &v)
{
	if (!PIMPL->vdata->set(i, v.x, v.y))
		MMWARNING("Failed to assign values (" << v.x << ", " << v.y << ") to vertex " << i);
}

void
MeshBase::textureCoordinate(uint16_t i, float &u, float &v) const
{
	if (!PIMPL->tcdata->get(i, u, v))
		MMWARNING("Failed to retrieve values for vertex " << i);
}

void
MeshBase::setTextureCoordinate(uint16_t i, float u, float v)
{
	if (!PIMPL->tcdata->set(i, u, v))
		MMWARNING("Failed to assign values (" << u << ", " << v << ") to texture coordinate " << i);
}

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

