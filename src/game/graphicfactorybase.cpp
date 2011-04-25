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

#include "game/graphicfactorybase.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "graphics/linegraphic.h"
#include "graphics/pointgraphic.h"
#include "graphics/polygongraphic.h"
#include "graphics/quadgraphic.h"
#include "graphics/trianglegraphic.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Game;

IGraphicFactory *GraphicFactoryBase::s_instance(0);

GraphicFactoryBase::GraphicFactoryBase(void)
{
	if (!s_instance) s_instance = this;
}

GraphicFactoryBase::~GraphicFactoryBase(void)
{
	if (s_instance == this) s_instance = 0;
}

Graphics::SharedGraphic
GraphicFactoryBase::createGraphic(const Core::Type &type) const
{
	if (Graphics::QuadGraphic::Type == type)
		return(new Graphics::QuadGraphic);
	else if (Graphics::PointGraphic::Type == type)
		return(new Graphics::PointGraphic);
	else if (Graphics::PolygonGraphic::Type == type)
		return(new Graphics::PolygonGraphic);
	else if (Graphics::TriangleGraphic::Type == type)
		return(new Graphics::TriangleGraphic);
	else if (Graphics::LineGraphic::Type == type)
		return(new Graphics::LineGraphic);
	return(Graphics::SharedGraphic());
}

