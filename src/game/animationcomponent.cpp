/*
 * Copyright (c) 2011-2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
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

#include "game/animationcomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <map>
#include <vector>

#include <tinyxml2.h>

#include "core/identifier.h"
#include "core/logger.h"
#include "core/type.h"

#include "graphics/meshbase.h"
#include "graphics/tileset.h"

#include "game/ientity.h"
#include "game/rendercomponent.h"
#include "game/tilesetcomponent.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Game { /******************************************** Game Namespace */

typedef std::pair<uint16_t, int> FrameEntry;
typedef std::vector<FrameEntry> FrameList;
typedef std::map<Core::Identifier, FrameList> AnimationFrames;
typedef std::map<Core::Identifier, float> AnimationFramerates;

struct AnimationComponent::Private
{
	Private(AnimationComponent &i)
	    : _interface(i)
	    , current_framelist(0)
	    , render(0)
	    , tileset(0)
	    , current_frame_entry(0)
	    , current_frame_duration(0)
	    , current_framerate(0.f)
	    , playback_ratio(1.f)
	    , timestamp(0)
	    , loop(false)
	    , playing(false)
	{}

	void play(const Core::Identifier &animation, bool loop);
	void stop(uint16_t *tile);
	void animate(float d);

	AnimationComponent &_interface;

	AnimationFrames     animation_frames;
	AnimationFramerates animation_framerate;

	const FrameList  *current_framelist;
	RenderComponent  *render;
	TilesetComponent *tileset;
	Graphics::ITextureCoordinateData *stop_data;

	size_t current_frame_entries;
	size_t current_frame_entry;
	int    current_frame_duration;
	float  current_framerate;
	float  playback_ratio;
	float  timestamp;
	bool   loop;
	bool   playing;
};

void
AnimationComponent::Private::stop(uint16_t *s)
{
	playing = false;
	current_frame_duration = 0;
	current_frame_entries = 0;
	current_frame_entry = 0;
	current_framelist = 0;

	if (s) stop_data = tileset->tileset()->getTextureCoordinateData(*s);

	Graphics::MeshBase *l_mesh =
	    static_cast<Graphics::MeshBase *>(render->mesh());
	l_mesh->setTextureCoordinateData(stop_data);
}

void
AnimationComponent::Private::play(const Core::Identifier &a, bool l)
{
	AnimationFrames::const_iterator l_frames =
	    animation_frames.find(a);
	if (l_frames == animation_frames.end()) {
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
	    animation_framerate.find(a);
	if (l_framerate == animation_framerate.end())
		current_framerate =
		    1.f / static_cast<float>(l_frame_count);
	else
		current_framerate =
		    1.f / l_framerate->second;

	current_frame_duration = 0;
	current_frame_entries = l_frames->second.size();
	current_frame_entry = 0;
	current_framelist = &l_frames->second;
	loop = l;
	timestamp = current_framerate;
	playing = true;
}

void
AnimationComponent::Private::animate(float d)
{
	if (!tileset) {
		tileset = static_cast<TilesetComponent *>
		    (_interface.entity().getComponentType(TilesetComponent::Type()));
		if (!tileset) return;
	}

	if (!render) {
		render = static_cast<RenderComponent *>
		    (_interface.entity().getComponentType(RenderComponent::Type()));
		if (render)
			stop_data = render->mesh()->textureCoordinateData();
		else return;
	}

	if (playing) {
		timestamp += d;

		const float l_framerate = current_framerate / playback_ratio;
		if (timestamp > l_framerate) {
			timestamp -= l_framerate;

			if (--current_frame_duration > 0)
				return;

			/* reached the end */
			if (++current_frame_entry >= current_frame_entries) {
				current_frame_entry = 0;

				if (!loop) {
					stop(0);
					return;
				}
			}

			Graphics::MeshBase *l_mesh =
			    static_cast<Graphics::MeshBase *>(render->mesh());

			/* replace texture coordinate data */
			const FrameEntry &l_entry = (*current_framelist)[current_frame_entry];
			l_mesh->setTextureCoordinateData
			    (tileset->tileset()->getTextureCoordinateData(l_entry.first));
			current_frame_duration = l_entry.second;
		}
	}
}

/********************************************************* AnimationComponent */

AnimationComponent::AnimationComponent(const Core::Identifier &i, IEntity &e)
    : ComponentBase(i, e)
    , m_p(0)
{
	m_p = new Private(*this);
}

AnimationComponent::~AnimationComponent(void)
{
	delete m_p, m_p = 0;
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

float
AnimationComponent::frameRate(const Core::Identifier &a) const
{
	AnimationFramerates::const_iterator l_i =
	    m_p->animation_framerate.find(a);
	if (l_i != m_p->animation_framerate.end())
		return(l_i->second);

	/* animation not found */
	return(0.f);
}

void
AnimationComponent::setFrameRate(const Core::Identifier &a, float fps)
{
	m_p->animation_framerate[a] = fps;
}

float
AnimationComponent::playbackRatio(void) const
{
	return(m_p->playback_ratio);
}

void
AnimationComponent::setPlaybackRatio(float r)
{
	m_p->playback_ratio = r;
}

void
AnimationComponent::play(const Core::Identifier &a, bool l)
{
	m_p->play(a, l);
}

void
AnimationComponent::stop(uint16_t *s)
{
	m_p->stop(s);
}

void
AnimationComponent::update(float d)
{
	m_p->animate(d);
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

} /******************************************************* Graphics Namespace */
MARSHMALLOW_NAMESPACE_END

