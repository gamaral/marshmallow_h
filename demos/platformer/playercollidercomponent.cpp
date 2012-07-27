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

#include "playercollidercomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <game/movementcomponent.h>
#include <game/positioncomponent.h>
#include <game/sizecomponent.h>

PlayerColliderComponent::PlayerColliderComponent(const Core::Identifier &i, Game::IEntity &e)
    : ColliderComponent(i, e)
    , m_platform(false)
{
	/* high velocity jumps may cause indentation */
	bullet() = true;
}

void
PlayerColliderComponent::update(float d)
{
	m_platform = false;
	ColliderComponent::update(d);
}

bool
PlayerColliderComponent::collision(ColliderComponent &c, float d,
                                   const Game::CollisionData &data)
{
	MMUNUSED(d);

	if (!movement()) return(false);

	const Math::Vector2 norm = movement()->velocity().normalized();

	if (c.id().str() == "platform") {
		if (data.rect.left  < 1 && norm.x() > 0) {
			position()->position()[0] =
				c.position()->position()[0] -
				    (c.size()->size().width() / 2.f + size()->size().width() / 2.f);
			movement()->velocity()[0] *= -0.4f;
		}
		else if (data.rect.right < 1 && norm.x() < 0) {
			position()->position()[0] =
				c.position()->position()[0] +
				    (c.size()->size().width() / 2.f + size()->size().width() / 2.f);
			movement()->velocity()[0] *= -0.4f;
		}
		else if (data.rect.top < 1 && norm.y() < 0) {
			position()->position()[1] =
				c.position()->position()[1] +
				    (c.size()->size().height() / 2.f + size()->size().height() / 2.f);
			m_platform = true;
			movement()->velocity()[1] = 0;
		}
		else if (data.rect.bottom < 1 && norm.y() > 0) {
			position()->position()[1] =
				c.position()->position()[1] -
				    (c.size()->size().height() / 2.f + size()->size().height() / 2.f);
			movement()->velocity()[1] *= -0.4f;
		}
	}
	else if (c.id().str() == "bounce") {
		if (data.rect.top < 1 && norm.y() < 0) {
			movement()->velocity()[1] = 900.f;
		}
	}
	return(true);
}

