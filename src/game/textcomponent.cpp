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

#include "game/textcomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/logger.h"
#include "core/type.h"

#include "math/point2.h"

#include "graphics/factory.h"
#include "graphics/itexturecoordinatedata.h"
#include "graphics/itexturedata.h"
#include "graphics/itileset.h"
#include "graphics/ivertexdata.h"
#include "graphics/painter.h"
#include "graphics/quadmesh.h"

#include "game/ientity.h"
#include "game/positioncomponent.h"
#include "game/tilesetcomponent.h"

#include <cassert>
#include <string>
#include <vector>

#define MIN_CHAR 33
#define MAX_CHAR 126

MARSHMALLOW_NAMESPACE_BEGIN
namespace { /************************************ Game::<anonymous> Namespace */

	static float
	GetLineLength(const std::string &str, size_t offset)
	{
		float curlen = 0;
		const size_t c = str.length();

		for (size_t i = offset; i < c; ++i) {
			if ('\n' == str[i])
				return(curlen);
			else ++curlen;
		}

		return(curlen);
	}

} /********************************************** Game::<anonymous> Namespace */

namespace Game { /******************************************** Game Namespace */

struct TextComponent::Private
{
	Private(void)
	    : position(0)
	    , tileset(0)
	    , alignment(Center)
	    , scale(1.f)
	    , tile_offset(0)
	    , invalidated(true)
	{}

	void rebuild(void);
	void render(void);

	std::vector<Graphics::IMesh *> mesh;

	PositionComponent *position;
	TilesetComponent *tileset;

	Graphics::Color color;

	std::string text;
	Alignment alignment;

	float scale;
	uint16_t tile_offset;
	bool invalidated;
};

void
TextComponent::Private::rebuild(void)
{
	mesh.clear();
	mesh.resize(text.size());

	if (!tileset) {
		MMWARNING("No tileset assigned.");
		return;
	}

	Graphics::ITileset *l_ts = tileset->tileset();

	/* create vertex data */

	/* TODO: this needs to be kept around, only replaced when font size
	 *       changes.
	 */
	Graphics::IVertexData *l_vdata =
	    Graphics::Factory::CreateVertexData(MARSHMALLOW_QUAD_VERTEXES);
	{
		float l_width  = float(l_ts->tileSize().width)  * scale;
		float l_height = float(l_ts->tileSize().height) * scale;

		l_vdata->set(0, 0,       0);
		l_vdata->set(1, 0,       -l_height);
		l_vdata->set(2, l_width, 0);
		l_vdata->set(3, l_width, -l_height);
	}

	/* create characters */

	/* TODO: find line-breaks to determine line length for center
	 *       alignment, also add right alignment.
	 */
	char l_char;
	const size_t l_text_count = text.size();
	for (uint16_t i = 0; i < l_text_count; ++i) {
		l_char = text[i];
		if (MIN_CHAR <= l_char && MAX_CHAR >= l_char) {
			Graphics::ITextureCoordinateData *l_tdata =
				l_ts->getTextureCoordinateData(static_cast<uint16_t>
				    (tile_offset + (l_char - MIN_CHAR)));

			mesh[i] = new Graphics::QuadMesh(l_tdata,
			                                 l_ts->textureData(),
			                                 l_vdata,
			                                 Graphics::QuadMesh::None);
		}
	}

	invalidated = false;
}

void
TextComponent::Private::render(void)
{
	if (invalidated) return;

	/* if no position component, abort! */
	if (!position) {
		MMWARNING("No position component found!");
		return;
	}

	/*
	 * We should be holding a valid tileset by this point.
	 */
	assert(tileset && "Invalid tileset! We shouldn't have gotten this far.");
	Graphics::ITileset *l_ts = tileset->tileset();

	/* render characters */

	char l_char;
	const float l_char_size = float(l_ts->tileSize().width) * scale;
	const size_t l_text_count = text.size();

	Math::Point2 l_point(position->position());
	switch(alignment) {
	case Left: break;
	case Center:
		l_point.x -= (l_char_size * GetLineLength(text, 0)) / 2.f;
	break;
	case Right:
		l_point.x -= l_char_size * GetLineLength(text, 0);
	break;
	}

	for (size_t i = 0; i < l_text_count; ++i) {
		l_char = text[i];

		/* render valid characters */
		if (MIN_CHAR <= l_char && MAX_CHAR >= l_char) {
			Graphics::QuadMesh *l_mesh =
			    static_cast<Graphics::QuadMesh *>(mesh[i]);
			l_mesh->setColor(color);
			Graphics::Painter::Draw(*l_mesh, l_point);
			l_point.x += l_char_size;
		}

		/* handle line break */
		else if ('\n' == l_char) {
			l_point.x = position->position().x;

			switch(alignment) {
			case Left: break;
			case Center:
				l_point.x -=
				    (l_char_size * GetLineLength(text, i + 1)) / 2.f;
			break;
			case Right:
				l_point.x -=
				    l_char_size * GetLineLength(text, i + 1);
			break;
			};

			l_point.y +=
			    float(l_ts->tileSize().height) * -scale;
		}

		/* skip unknown character */
		else l_point.x += l_char_size;
	}
}

/******************************************************************************/

TextComponent::TextComponent(const Core::Identifier &i, Game::IEntity *e)
    : Component(i, e)
    , PIMPL_CREATE
{
}

TextComponent::~TextComponent(void)
{
	PIMPL_DESTROY;
}

const std::string &
TextComponent::text(void) const
{
	return(PIMPL->text);
}

const Graphics::Color &
TextComponent::color(void) const
{
	return(PIMPL->color);
}

TextComponent::Alignment
TextComponent::alignment(void) const
{
	return(PIMPL->alignment);
}

void
TextComponent::setAlignment(Alignment a)
{
	PIMPL->alignment = a;
	PIMPL->invalidated = true;
	PIMPL->rebuild();
}

void
TextComponent::setText(const std::string &t)
{
	PIMPL->text = t;
	PIMPL->invalidated = true;
	PIMPL->rebuild();
}

void
TextComponent::setColor(const Graphics::Color &c)
{
	PIMPL->color = c;
}

float
TextComponent::scale(void) const
{
	return(PIMPL->scale);
}

void
TextComponent::setScale(float s)
{
	PIMPL->scale = s;
	PIMPL->invalidated = true;
	PIMPL->rebuild();
}

uint16_t
TextComponent::tileOffset(void) const
{
	return(PIMPL->tile_offset);
}

void
TextComponent::setTileOffset(uint16_t o)
{
	PIMPL->tile_offset = o;
	PIMPL->invalidated = true;
	PIMPL->rebuild();
}

void
TextComponent::update(float delta)
{
	Component::update(delta);

	if (!PIMPL->position)
	    PIMPL->position = static_cast<PositionComponent *>
	        (entity()->getComponentType(PositionComponent::Type()));

	if (!PIMPL->tileset)
	    PIMPL->tileset = static_cast<TilesetComponent *>
	        (entity()->getComponentType(TilesetComponent::Type()));

	if (PIMPL->invalidated)
	    PIMPL->rebuild();
}

void
TextComponent::render(void)
{
	Component::render();
	PIMPL->render();
}

const Core::Type &
TextComponent::Type(void)
{
	static const Core::Type s_type("Game::TextComponent");
	return(s_type);
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

