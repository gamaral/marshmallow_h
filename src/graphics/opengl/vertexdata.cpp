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

#include "vertexdata.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <cstring>

#include "core/logger.h"

MARSHMALLOW_NAMESPACE_USE
using namespace Graphics;
using namespace OpenGL;

VertexData::VertexData(uint16_t c)
#define AXES 2
    : m_id()
    , m_data(new GLfloat[c * AXES]) // TODO: replace with custom allocator
    , m_count(c)
    , m_bufferId(0)
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
	if (!isBuffered())
		glGenBuffers(1, &m_bufferId);

	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	glBufferData(GL_ARRAY_BUFFER, m_count * AXES * sizeof(GLfloat), m_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	MMVERBOSE("Buffered data. ID: " << m_bufferId << ".");
}

void
VertexData::unbuffer(void)
{
	if (!isBuffered())
		return;

	MMVERBOSE("Unbuffered data. ID: " << m_bufferId << ".");

	glDeleteBuffers(1, &m_bufferId);
	m_bufferId = 0;
}

bool
VertexData::get(uint16_t i, float &x, float &y) const
{
	const uint16_t l_offset = static_cast<uint16_t>((i % m_count) * AXES);
	x = m_data[l_offset];
	y = m_data[l_offset + 1];
	return(true);
}

bool
VertexData::set(uint16_t i, float x, float y)
{
	const uint16_t l_offset = static_cast<uint16_t>((i % m_count) * AXES);
	m_data[l_offset] = x;
	m_data[l_offset + 1] = y;

	/* update vbo object */
	if (isBuffered()) {
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
		glBufferSubData(GL_ARRAY_BUFFER, l_offset * sizeof(GLfloat), AXES * sizeof(GLfloat), &m_data[l_offset]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	return(true);
}

const Core::Type &
VertexData::Type(void)
{
	static const Core::Type sType("Graphics::OpenGL::VertexData");
	return(sType);
}

