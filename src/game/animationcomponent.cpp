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

MARSHMALLOW_NAMESPACE_USE;
using namespace Game;

AnimationComponent::AnimationComponent(const Core::Identifier &i, IEntity &e)
    : ComponentBase(i, e)
    , m_render()
    , m_tileset()
    , m_timestamp(0)
    , m_loop(false)
    , m_playing(false)
    , m_current_framelist(0)
    , m_current_framerate(0)
    , m_current_frame_duration(0)
    , m_current_frame_entry(-1)
{
}

AnimationComponent::~AnimationComponent(void)
{
}

void
AnimationComponent::pushFrame(const Core::Identifier &a, int t, int d)
{
	FrameList &l_framelist = m_animation_frames[a];
	l_framelist.push_back(FrameEntry(t, d));
	if (m_current_framelist == &l_framelist)
		++m_current_frame_entries;
}

void
AnimationComponent::popFrame(const Core::Identifier &a)
{
	FrameList &l_framelist = m_animation_frames[a];
	l_framelist.pop_back();
	if (m_current_framelist == &l_framelist)
		--m_current_frame_entries;
}

void
AnimationComponent::rate(const Core::Identifier &a, float fps)
{
	m_animation_framerate[a] = fps;
}

void
AnimationComponent::play(const Core::Identifier &a, bool l)
{
	AnimationFrames::const_iterator l_frames =
	    m_animation_frames.find(a);
	if (l_frames == m_animation_frames.end()) {
		MMWARNING1("Invalid animation requested.");
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
	    m_animation_framerate.find(a);
	if (l_framerate == m_animation_framerate.end())
		m_current_framerate =
		    1.f / static_cast<float>(l_frame_count);
	else
		m_current_framerate =
		    1.f / l_framerate->second;

	m_current_frame_duration = 0;
	m_current_frame_entries = l_frames->second.size();
	m_current_frame_entry = -1;
	m_current_framelist = &l_frames->second;
	m_loop = l;
	m_timestamp = 0;
	m_playing = true;
}

void
AnimationComponent::stop(int *s)
{
	m_playing = false;
	m_current_frame_duration = 0;
	m_current_frame_entries = 0;
	m_current_frame_entry = -1;
	m_current_framelist = 0;

	if (s) m_stop_data = m_tileset->tileset()->getTextureCoordinateData(*s);

	Graphics::SharedMeshBase l_mesh =
	    m_render->mesh().staticCast<Graphics::MeshBase>();
	l_mesh->setTextureCoordinateData(m_stop_data);
}

void
AnimationComponent::update(float d)
{
	if (!m_tileset) {
		m_tileset = entity().getComponentType(TilesetComponent::Type()).
		    staticCast<TilesetComponent>();
		if (!m_tileset) return;
	}

	if (!m_render) {
		m_render = entity().getComponentType(RenderComponent::Type()).
		    staticCast<RenderComponent>();
		if (m_render)
			m_stop_data = m_render->mesh()->textureCoordinateData();
		else return;
	}

	if (m_playing) {
		m_timestamp += d;

		if (m_timestamp > m_current_framerate) {
			m_timestamp -= m_current_framerate;

			if (--m_current_frame_duration > 0)
				return;

			/* reached the end */
			if (++m_current_frame_entry >= m_current_frame_entries) {
				m_current_frame_entry = 0;

				if (!m_loop) {
					stop();
					return;
				}
			}

			Graphics::SharedMeshBase l_mesh =
			    m_render->mesh().staticCast<Graphics::MeshBase>();

			/* replace texture coordinate data */
			const FrameEntry &l_entry = (*m_current_framelist)[m_current_frame_entry];
			l_mesh->setTextureCoordinateData
			    (m_tileset->tileset()->getTextureCoordinateData(l_entry.first));
			m_current_frame_duration = l_entry.second;
		}
	}
}

bool
AnimationComponent::serialize(TinyXML::TiXmlElement &n) const
{
	MMUNUSED(n);
	/* TODO: IMPLEMENT */
	return(true);
}

bool
AnimationComponent::deserialize(TinyXML::TiXmlElement &n)
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

