/*
 * Copyright (c) 2012-2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
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

#include "game/propertycomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/identifier.h"
#include "core/type.h"

#include <tinyxml2.h>

#include <map>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

struct PropertyComponent::Private
{
	std::map<Core::Identifier, std::string> data;
};

PropertyComponent::PropertyComponent(const Core::Identifier &i, IEntity &e)
    : ComponentBase(i, e)
    , PIMPL_CREATE
{
}

PropertyComponent::~PropertyComponent(void)
{
	PIMPL_DESTROY;
}

std::string
PropertyComponent::get(const Core::Identifier &i) const
{
	std::map<Core::Identifier, std::string>::iterator l_it;
	l_it = PIMPL->data.find(i);

	if (l_it != PIMPL->data.end()) return(l_it->second);
	else return(std::string());
}

void
PropertyComponent::set(const Core::Identifier &i, const std::string &v)
{
	PIMPL->data[i] = v;
}

bool
PropertyComponent::serialize(XMLElement &n) const
{
	if (!ComponentBase::serialize(n))
	    return(false);

	/* TODO(gamaral) fill in serialization code */
	return(true);
}

bool
PropertyComponent::deserialize(XMLElement &n)
{
	if (!ComponentBase::deserialize(n))
	    return(false);

	/* TODO(gamaral) fill in deserialization code */
	return(true);
}

const Core::Type &
PropertyComponent::Type(void)
{
	static const Core::Type s_type("Game::PropertyComponent");
	return(s_type);
}

} /*********************************************************** Game Namespace */
MARSHMALLOW_NAMESPACE_END

