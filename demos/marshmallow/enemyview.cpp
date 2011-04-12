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

#include "enemyview.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <core/platform.h>
#include <game/ientity.h>

EnemyView::EnemyView(DifficultyLevel d)
    : ViewBase(epPhasesMax),
      m_difficulty(static_cast<int>(d))
{
}

EnemyView::~EnemyView(void)
{
}

void
EnemyView::handleEnemy(const Game::SharedEntity &e, int p)
{
	
	EnemyState state = m_states[e->id().uid()];

	/*
	 * TODO: Use player position to update enemy states and take appropriate
	 * action.
	 */

	switch (p) {
	case epUpdate:
		INFO("Update local data for Enemy %p (phase 0)", e->id().str());
	    break;
	case epExecute:
		switch (state) {
		case esIdle:
			INFO("Enemy %s moves around a little. (phase 1)", e->id().str());
			/* TODO: add move message here */
			break;
		case esTargeting:
			INFO("Enemy %s targets player.", e->id().str());
			break;
		case esShooting:
			INFO("Enemy %s shoots at player.", e->id().str());
			break;
		}
	    break;
	}

	e->kill();
}

void
EnemyView::handlePlayer(const Game::SharedEntity &e, int p)
{
	UNUSED(e);
	UNUSED(p);

	/*
	 * TODO: get player positions, etc.
	 */
}

void
EnemyView::initialize(void)
{
}

void
EnemyView::finalize(void)
{
	m_states.clear();
}

void
EnemyView::renderEntity(const Game::SharedEntity &e, int p)
{
	/*
	 * Should use type, but for this example I'm using ids
	 * static const Core::Type enemy_t("enemy");
	 */
	static const Core::Identifier enemy_id("enemy");
	static const Core::Identifier user_id("player");

	if (e->id() == enemy_id)
		handleEnemy(e, p);
	else if (e->id() == user_id)
		handlePlayer(e, p);
}

