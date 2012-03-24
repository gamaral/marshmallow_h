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

#include "game/animationcomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"

#include "graphics/meshbase.h"
#include "graphics/tileset.h"

#include "game/ientity.h"
#include "game/rendercomponent.h"
#include "game/tilesetcomponent.h"

#include <tinyxml2.h>

#include <map>
#include <vector>

MARSHMALLOW_NAMESPACE_USE
using namespace Game;

/******************************************************************************/

namespace {
	typedef std::pair<uint16_t, int> FrameEntry;
	typedef std::vector<FrameEntry> FrameList;
	typedef std::map<Core::Identifier, FrameList> AnimationFrames;
	typedef std::map<Core::Identifier, float> AnimationFramerates;
} // namespace

/******************************************************************************/

struct AnimationComponent::Private
{
	AnimationFrames     animation_frames;
	AnimationFramerates animation_framerate;
	Graphics::SharedTextureCoordinateData stop_data;

	WeakRenderComponent  render;
	WeakTilesetComponent tileset;

	float timestamp;
	bool  loop;
	bool  playing;

	const FrameList *current_framelist;
	float  current_framerate;
	int    current_frame_duration;
	size_t current_frame_entries;
	size_t current_frame_entry;
};

AnimationComponent::AnimationComponent(const Core::Identifier &i, IEntity &e)
    : ComponentBase(i, e)
    , m_p(new Private)
{
	m_p->timestamp = 0;
	m_p->loop = false;
	m_p->playing = false;
	m_p->current_framelist = 0;
	m_p->current_framerate = 0;
	m_p->current_frame_duration = 0;
	m_p->current_frame_entry = 0;
}

AnimationComponent::~AnimationComponent(void)
{
	delete m_p;
	m_p = 0;
}

void
AnimationComponent::pushFrame(const Core::Identifier &a, uint16_t t, int d)
{
	FrameList &l_framelist = m_p->animation_frames[a];
	l_framelist.push_back(FrameEntry(t, d));
	if (m_p->current_framelist == &l_framelist)
		++m_p->current_frame_entries;
}

void
AnimationComponent::popFrame(const Core::Identifier &a)
{
	FrameList &l_framelist = m_p->animation_frames[a];
	l_framelist.pop_back();
	if (m_p->current_framelist == &l_framelist)
		--m_p->current_frame_entries;
}

void
AnimationComponent::rate(const Core::Identifier &a, float fps)
{
	m_p->animation_framerate[a] = fps;
}

void
AnimationComponent::play(const Core::Identifier &a, bool l)
{
	AnimationFrames::const_iterator l_frames =
	    m_p->animation_frames.find(a);
	if (l_frames == m_p->animation_frames.end()) {
		MMWARNING("Invalid animation requested.");
		return;
	}

	/* get frame count */
	int l_frame_count = 0;
	FrameList::const_iterator l_entry;
	for (l_entry  = l_frames->second.begin();
	     l_entry != l_frames->second.end();
	   ++l_entry)
		l_frame_count += l_entry->second;

	/* get framerate */
	AnimationFramerates::const_iterator l_framerate =
	    m_p->animation_framerate.find(a);
	if (l_framerate == m_p->animation_framerate.end())
		m_p->current_framerate =
		    1.f / static_cast<float>(l_frame_count);
	else
		m_p->current_framerate =
		    1.f / l_framerate->second;

	m_p->current_frame_duration = 0;
	m_p->current_frame_entries = l_frames->second.size();
	m_p->current_frame_entry = 0;
	m_p->current_framelist = &l_frames->second;
	m_p->loop = l;
	m_p->timestamp = m_p->current_framerate;
	m_p->playing = true;
}

void
AnimationComponent::stop(uint16_t *s)
{
	m_p->playing = false;
	m_p->current_frame_duration = 0;
	m_p->current_frame_entries = 0;
	m_p->current_frame_entry = 0;
	m_p->current_framelist = 0;

	if (s) m_p->stop_data = m_p->tileset->tileset()->getTextureCoordinateData(*s);

	Graphics::SharedMeshBase l_mesh =
	    m_p->render->mesh().staticCast<Graphics::MeshBase>();
	l_mesh->setTextureCoordinateData(m_p->stop_data);
}

void
AnimationComponent::update(float d)
{
	if (!m_p->tileset) {
		m_p->tileset = entity().getComponentType(TilesetComponent::Type()).
		    staticCast<TilesetComponent>();
		if (!m_p->tileset) return;
	}

	if (!m_p->render) {
		m_p->render = entity().getComponentType(RenderComponent::Type()).
		    staticCast<RenderComponent>();
		if (m_p->render)
			m_p->stop_data = m_p->render->mesh()->textureCoordinateData();
		else return;
	}

	if (m_p->playing) {
		m_p->timestamp += d;

		if (m_p->timestamp > m_p->current_framerate) {
			m_p->timestamp -= m_p->current_framerate;

			if (--m_p->current_frame_duration > 0)
				return;

			/* reached the end */
			if (++m_p->current_frame_entry >= m_p->current_frame_entries) {
				m_p->current_frame_entry = 0;

				if (!m_p->loop) {
					stop();
					return;
				}
			}

			Graphics::SharedMeshBase l_mesh =
			    m_p->render->mesh().staticCast<Graphics::MeshBase>();

			/* replace texture coordinate data */
			const FrameEntry &l_entry = (*m_p->current_framelist)[m_p->current_frame_entry];
			l_mesh->setTextureCoordinateData
			    (m_p->tileset->tileset()->getTextureCoordinateData(l_entry.first));
			m_p->current_frame_duration = l_entry.second;
		}
	}
}

bool
AnimationComponent::serialize(XMLElement &n) const
{
	MMUNUSED(n);
	/* TODO: IMPLEMENT */
	return(true);
}

bool
AnimationComponent::deserialize(XMLElement &n)
{
	MMUNUSED(n);
	/* TODO: IMPLEMENT */
	return(false);
}

const Core::Type &
AnimationComponent::Type(void)
{
	static const Core::Type s_type("Game::AnimationComponent");
	return(s_type);
}

