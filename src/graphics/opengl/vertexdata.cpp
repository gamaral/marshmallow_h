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

#include "graphics/opengl/vertexdata.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <cstring>

#include "core/logger.h"
#include "graphics/opengl/extensions/vbo.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Graphics;
using namespace OpenGL;

const Core::Type VertexData::sType("Graphics::VertexData");

VertexData::VertexData(int c)
#define AXES 2
    : m_id(),
      m_data(new GLfloat[c * AXES]), // TODO: replace with custom allocator
      m_count(c),
      m_bufferId(0)
{
	memset(m_data, 0, m_count * AXES);
	buffer();
}

VertexData::~VertexData(void)
{
	unbuffer();
	delete[] m_data;
}

void
VertexData::buffer(void)
{
	if (!VBO::Supported())
		return;

	if (!isBuffered())
		VBO::GenBuffers(1, &m_bufferId);

	VBO::BindBuffer(GL_ARRAY_BUFFER_ARB, m_bufferId);
	VBO::BufferData(GL_ARRAY_BUFFER_ARB, m_count * AXES * sizeof(GLfloat), m_data, GL_STATIC_DRAW_ARB);
	VBO::BindBuffer(GL_ARRAY_BUFFER_ARB, 0);

	MMVERBOSE("Buffered data. ID: %d", m_bufferId);
}

void
VertexData::unbuffer(void)
{
	if (!VBO::Supported() || !isBuffered())
		return;

	MMVERBOSE("Unbuffered data. ID: %d", m_bufferId);

	VBO::DeleteBuffers(1, &m_bufferId);
	m_bufferId = 0;
}

bool
VertexData::get(int i, float &x, float &y) const
{
	const int l_offset = (i % m_count) * AXES;
	x = m_data[l_offset];
	y = m_data[l_offset + 1];
	return(true);
}

bool
VertexData::set(int i, float x, float y)
{
	const int l_offset = (i % m_count) * AXES;
	m_data[l_offset] = x;
	m_data[l_offset + 1] = y;

	/* update vbo object */
	if (isBuffered()) {
		VBO::BindBuffer(GL_ARRAY_BUFFER_ARB, m_bufferId);
		VBO::BufferSubData(GL_ARRAY_BUFFER_ARB, l_offset * sizeof(GLfloat), AXES * sizeof(GLfloat), &m_data[l_offset]);
		VBO::BindBuffer(GL_ARRAY_BUFFER_ARB, 0);
	}

	return(true);
}

const Core::Type &
VertexData::Type(void)
{
	return(sType);
}

