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

#include "actorcollidercomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <core/identifier.h>
#include <core/logger.h>
#include <core/weak.h>

#include <math/size2.h>

#include <game/movementcomponent.h>
#include <game/positioncomponent.h>
#include <game/propertycomponent.h>
#include <game/sizecomponent.h>

#include "../common/actorentity.h"
#include "../common/doomevent.h"
#include "../common/warpevent.h"

ActorColliderComponent::ActorColliderComponent(const Core::Identifier &i, Common::ActorEntity &e)
    : Game::ColliderComponent(i, e)
    , m_enabled(false)
    , m_platform(false)
{
	bullet() = true;
	enable();
}

void
ActorColliderComponent::update(float d)
{
	m_platform = false;
	Game::ColliderComponent::update(d);
}

bool
ActorColliderComponent::collision(ColliderComponent &c, float d,
                                   const Game::CollisionData &data)
{
	MMUNUSED(d);

	if (!m_enabled || !movement()) return(false);

	const Math::Vector2 norm =
	    movement()->velocity().
	        normalized(movement()->velocity().magnitude());

	/*
	 * What happens when we collide with a platform goes here
	 */
	if (c.id().str() == "platform") {
		if (data.rect.left  < 1 && norm.x > 0) {
			position()->position().x =
				c.position()->position().x -
				    (c.size()->size().width / 2.f + size()->size().width / 2.f);
			movement()->velocity().x *= -0.4f;
		}
		else if (data.rect.right < 1 && norm.x < 0) {
			position()->position().x =
				c.position()->position().x +
				    (c.size()->size().width / 2.f + size()->size().width / 2.f);
			movement()->velocity().x *= -0.4f;
		}
		else if (data.rect.top < 1 && norm.y < 0) {
			position()->position().y =
				c.position()->position().y +
				    (c.size()->size().height / 2.f + size()->size().height / 2.f);
			m_platform = true;
			movement()->velocity().y = 0;
		}
		else if (data.rect.bottom < 1 && norm.y > 0) {
			position()->position().y =
				c.position()->position().y -
				    (c.size()->size().height / 2.f + size()->size().height / 2.f);
			movement()->velocity().y *= -0.4f;
		}
	}

	/*
	 * What happens when we collide with something bouncy
	 */
	else if (c.id().str() == "bounce") {
		if (data.rect.top < 1 && norm.y < 0)
			movement()->velocity().y = 900.f;
	}

	/*
	 * What happens when we collide with doom itself
	 */
	else if (c.id().str() == "doom") {
		/*
		 * Tell the parent entity to die! DIE! DIE!!!
		 */
		Common::DoomEvent event;
		static_cast<Common::ActorEntity &>(entity()).handleEvent(event);
	}

	/*
	 * What happens when we collide with warp
	 */
	else if (c.id().str() == "warp") {
		/*
		 * Tell the parent entity hit a warp
		 */
		Game::SharedComponent l_component =
		    c.entity().getComponent("property");
		if (l_component) {
			Game::SharedPropertyComponent l_property_component =
			    l_component.staticCast<Game::PropertyComponent>();

			const std::string &l_dest = l_property_component->get("level");

			MMDEBUG("Collision with warp pointing to " << l_dest << " level");

			Common::WarpEvent event(l_dest);
			static_cast<Common::ActorEntity &>(entity()).handleEvent(event);
		}
		else MMVERBOSE("Collision with warp going nowhere! IGNORING.");
	}

	return(true);
}

