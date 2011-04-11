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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <EASTL/hash_map.h>
#include <EASTL/list.h>
using namespace eastl;

#include <game/viewbase.h>
#include <math/vector2.h>

MARSHMALLOW_NAMESPACE_USE;

class EnemyView : public Game::ViewBase
{
	enum EnemyPhases { epUpdate, epExecute, epPhasesMax };
	enum EnemyState { esIdle = 0, esTargeting, esShooting };

	int m_difficulty;
	hash_map<UID, EnemyState> m_states;

	Math::Vector2 m_playerLocation;

public:
	enum DifficultyLevel { dlNovice = 0, dlNormal, dlHard, dlBattleToads };

	EnemyView(DifficultyLevel difficulty = dlNovice);
	virtual ~EnemyView(void);

	void handleEnemy(const Game::SharedEntity &entity, int phase);
	void handlePlayer(const Game::SharedEntity &entity, int phase);

public: /* VIRTUAL */

	VIRTUAL void initialize(void);
	VIRTUAL void finalize(void);

	VIRTUAL void renderEntity(const Game::SharedEntity &entity, int phase);
};

