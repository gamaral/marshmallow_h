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

#include "game/scenelayerbase.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <tinyxml2.h>

MARSHMALLOW_NAMESPACE_USE
using namespace Game;

struct SceneLayerBase::Private
{
	Private(const Core::Identifier &i, IScene &s, int f)
	    : id(i)
	    , scene(s)
	    , flags(f)
	    , killed(false) {}

	Core::Identifier id;
	IScene &scene;
	int flags;
	bool killed;
};

SceneLayerBase::SceneLayerBase(const Core::Identifier &i, IScene &s, int f)
    : m_p(new Private(i, s, f))
{
}

SceneLayerBase::~SceneLayerBase(void)
{
	delete m_p, m_p = 0;
}

const Core::Identifier &
SceneLayerBase::id(void) const
{
	return(m_p->id);
}

IScene &
SceneLayerBase::scene(void)
{
	return(m_p->scene);
}

int
SceneLayerBase::flags(void) const
{
	return(m_p->flags);
}

void
SceneLayerBase::kill(void)
{
	m_p->killed = true;
}

bool
SceneLayerBase::isZombie(void) const
{
	return(m_p->killed);
}

bool
SceneLayerBase::serialize(XMLElement &n) const
{
	n.SetAttribute("id", id().str().c_str());
	n.SetAttribute("type", type().str().c_str());
	return(true);
}

bool
SceneLayerBase::deserialize(XMLElement &n)
{
	MMUNUSED(n);
	return(true);
}

