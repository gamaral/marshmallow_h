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

#include "texturedata.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/logger.h"

#include "headers.h"

#include <png.h>

#include <cstdlib>

namespace {

struct Texture
{
	uint32_t width;
	uint32_t height;
	uint8_t  depth;
	uint8_t  components;

	uint8_t *pixels;
};

bool
LoadTexturePNG(const std::string &filename, Texture &data)
{
	FILE *png_file;
	if ((png_file = fopen(filename.c_str(), "rb")) == 0)
		return(false);

	unsigned char header[8];
	if (fread(header, 1, 8, png_file) != 8 ||
	    png_sig_cmp(header, 0, 8) != 0) {
		fclose(png_file);
		return(false);
	}

	png_structp png_ptr;
	if ((png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0)) == 0) {
		fclose(png_file);
		return(false);
	}

	png_infop info_ptr;
	if ((info_ptr = png_create_info_struct(png_ptr)) == 0) {
		fclose(png_file);
		png_destroy_read_struct(&png_ptr, 0, 0);
		return(false);
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		fclose(png_file);
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		return(false);
	}

	png_init_io(png_ptr, png_file);
	png_set_sig_bytes(png_ptr, 8);

	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, 0);

	data.width  = png_get_image_width(png_ptr, info_ptr);
	data.height = png_get_image_height(png_ptr, info_ptr);
	data.depth  = png_get_bit_depth(png_ptr, info_ptr);
	png_byte color_type = png_get_color_type(png_ptr, info_ptr);

	if((color_type & PNG_COLOR_TYPE_RGBA) == PNG_COLOR_TYPE_RGBA)
		data.components = 4;
	else if((color_type & PNG_COLOR_TYPE_RGB) == PNG_COLOR_TYPE_RGB)
		data.components = 3;
	else {
		fclose(png_file);
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		return(false);
	}

	png_size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	data.pixels = new uint8_t[row_bytes * data.height];

	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

	for (unsigned int i = 0; i < data.height; ++i)
		memcpy(data.pixels + (row_bytes * i), row_pointers[i], row_bytes);

	png_destroy_read_struct(&png_ptr, &info_ptr, 0);
	fclose(png_file);

	return(true);
}

void
UnloadTexture(Texture &data)
{
	data.width = 0;
	data.height = 0;
	data.depth = 0;
	data.components = 0;
	delete [] data.pixels, data.pixels = 0;
}

}

MARSHMALLOW_NAMESPACE_USE
using namespace Graphics::OpenGL;
using namespace Graphics;

TextureData::TextureData(void)
    : m_id(),
      m_size(),
      m_texture_id(0)
{
}

TextureData::~TextureData(void)
{
	unload();
}

bool
TextureData::load(const Core::Identifier &i)
{
	if (m_texture_id) {
		MMERROR("Load texture asset called on active texture.");
		return(false);
	}

	Texture tdata;
	if (!LoadTexturePNG(i.str(), tdata)) {
		MMERROR("Failed to load texture: " << id().str());
		return(false);
	}

	GLint l_format;
	switch (tdata.components) {
	case 4: l_format = GL_RGBA; break;
	case 3: l_format = GL_RGB;  break;
	default:
		MMERROR("Unknown format for texture: " << id().str());
		return(false);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &m_texture_id);
	glBindTexture(GL_TEXTURE_2D, m_texture_id);

	m_size = Math::Size2i(tdata.width, tdata.height);
	glTexImage2D(GL_TEXTURE_2D, 0, l_format, tdata.width, tdata.height, 0, l_format, GL_UNSIGNED_BYTE, tdata.pixels);
	UnloadTexture(tdata);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	m_id = i;

	MMINFO("Texture loaded.");

	return(true);
}

void
TextureData::unload(void)
{
	if (m_texture_id)
		glDeleteTextures(1, &m_texture_id);

	m_size = Math::Size2i();
	m_texture_id = 0;

	MMINFO("Texture unloaded.");
}

const Core::Type &
TextureData::Type(void)
{
	static const Core::Type sType("Graphics::OpenGL::TextureData");
	return(sType);
}

