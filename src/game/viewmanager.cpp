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

#include "game/viewmanager.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "game/iview.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Game;

ViewManager::ViewManager(void)
{
}

ViewManager::~ViewManager(void)
{
	clearViews();
}

void
ViewManager::addView(SharedView &v)
{
	v->initialize();
	m_list.push_back(v);
}

void
ViewManager::removeView(const SharedView &v)
{
	m_list.remove(v);
	v->finalize();
}

void
ViewManager::clearViews(void)
{
	ViewList::const_iterator l_i;
	ViewList::const_iterator l_c = m_list.end();

	for (l_i = m_list.begin(); l_i != l_c; ++l_i)
		(*l_i)->finalize();

	m_list.clear();
}

void
ViewManager::render(SharedScene &scene)
{
	ViewList::const_iterator l_i;
	ViewList::const_iterator l_c = m_list.end();

	for (l_i = m_list.begin(); l_i != l_c; ++l_i)
		(*l_i)->render(scene);
}

