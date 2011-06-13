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

#include "graphics/dummy/meshdata.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <cstring>

MARSHMALLOW_NAMESPACE_USE;
using namespace Graphics;
using namespace Dummy;

MeshData::MeshData(int size)
    : m_vertex(new float[size * 2]), // TODO: replace with custom allocator
      m_tcoord(new float[size * 2]), // TODO: replace with custom allocator
      m_size(size)
{
	memset(m_vertex, 0, m_size * 2);
	memset(m_tcoord, 0, m_size * 2);
}

MeshData::~MeshData(void)
{
	delete[] m_vertex;
	delete[] m_tcoord;
}

bool
MeshData::vertex(int i, float &x, float &y) const
{
	const int l_offset = (i % m_size) * 2;
	x = m_vertex[l_offset];
	y = m_vertex[l_offset + 1];
	return(true);
}

bool
MeshData::setVertex(int i, float x, float y)
{
	const int l_offset = (i % m_size) * 2;
	m_vertex[l_offset] = x;
	m_vertex[l_offset + 1] = y;
	return(true);
}

bool
MeshData::textureCoord(int i, float &u, float &v) const
{
	const int l_offset = (i % m_size) * 2;
	u = m_tcoord[l_offset];
	v = m_tcoord[l_offset + 1];
	return(true);
}

bool
MeshData::setTextureCoord(int i, float u, float v)
{
	const int l_offset = (i % m_size) * 2;
	m_tcoord[l_offset] = u;
	m_tcoord[l_offset + 1] = v;
	return(true);
}

