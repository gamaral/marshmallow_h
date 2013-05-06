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

#include "audio/player.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/global.h"
#include "core/identifier.h"
#include "core/logger.h"

#include "audio/itrack.h"
#include "audio/pcm.h"

#include "backend_p.h"

#include <map>

#include <cstring>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Audio { /****************************************** Audio Namespace */
namespace { /*********************************** Audio::<anonymous> Namespace */

	static inline int16_t
	Mixer(int16_t a, int16_t b, float gain)
	{
		return(int16_t(MMRANGE(-(1<<15), a + int(b * gain), (1<<15)-1)));
	}

	static inline char
	Mixer(char a, char b, float gain)
	{
		return(char(MMRANGE(-(1<<7), a + int(b * gain), (1<<7)-1)));
	}

} /********************************************* Audio::<anonymous> Namespace */

struct Player::Private
{
	Private();
	~Private();

	inline void setPCM(PCM *_pcm);

	inline void load(const Core::Identifier &id, ITrack *track);
	inline bool contains(const Core::Identifier &id);
	inline ITrack * eject(const Core::Identifier &id);

	inline bool play(const Core::Identifier &id, int playlist, float gain);
	inline void stop(const Core::Identifier &id);
	inline bool isPlaying(const Core::Identifier &id) const;

	typedef std::map<MMUID, ITrack *> TrackMap;
	TrackMap tracks;

	typedef std::pair<int, float> IterationGainPair;
	typedef std::map<MMUID, IterationGainPair> PlaylistMap;
	PlaylistMap playlist;

	void tick(void);

	PCM *pcm;
	char *buffer;
	char *mix;
};

Player::Private::Private()
    : pcm(0)
    , buffer(0)
    , mix(0)
{
}

Player::Private::~Private()
{
#if MARSHMALLOW_DEBUG
	if (!tracks.empty())
		MMWARNING("Player destroyed while still holding track(s)! " << tracks.size());
#endif

	/* clear buffers */
	delete [] buffer, buffer = 0;
	delete [] mix, mix = 0;
	pcm = 0;
}

void
Player::Private::setPCM(PCM *_pcm)
{
	if (pcm == _pcm) return;

	/* clean buffers */
	if (pcm) {
		delete [] buffer, buffer = 0;
		delete [] mix, mix = 0;
	}

	pcm = _pcm;

	/* create new buffers */
	if (pcm && pcm->isOpen()) {
		buffer = new char[pcm->framesMax() * pcm->frameSize()];
		mix = new char[pcm->framesMax() * pcm->frameSize()];
	}
}

void
Player::Private::load(const Core::Identifier &id, ITrack *track)
{
	tracks[id] = track;
}

bool
Player::Private::contains(const Core::Identifier &id)
{
	return(tracks.find(id) != tracks.end());
}

ITrack *
Player::Private::eject(const Core::Identifier &id)
{
	TrackMap::iterator l_i = tracks.find(id);
	if (l_i == tracks.end())
		return(0);
	
	ITrack *l_track = l_i->second;
	tracks.erase(l_i);
	return(l_track);
}

bool
Player::Private::play(const Core::Identifier &id, int iterations, float gain)
{
	if (0 == iterations) {
		stop(id);
		return(false);
	}
	else {
		playlist[id] = std::make_pair(iterations, gain);
		return(true);
	}
}

void
Player::Private::stop(const Core::Identifier &id)
{
	PlaylistMap::iterator l_i = playlist.find(id);
	if (l_i != playlist.end())
		playlist.erase(l_i);
}

bool
Player::Private::isPlaying(const Core::Identifier &id) const
{
	return(playlist.find(id) != playlist.end());
}

void
Player::Private::tick(void)
{
	if (!pcm || !pcm->isOpen()) return;

	if (!buffer || !mix) {
		buffer = new char[pcm->framesMax() * pcm->frameSize()];
		mix = new char[pcm->framesMax() * pcm->frameSize()];
	}

	const size_t l_frames_available =
	    MMMIN(pcm->framesMax(), pcm->framesAvailable());
	if (0 == l_frames_available)
		return;
	
	const size_t l_buffer_max = l_frames_available * pcm->frameSize();
	memset(mix, 0, l_buffer_max);

	PlaylistMap::iterator l_i;
	const PlaylistMap::const_iterator l_c = playlist.end();
	for (l_i = playlist.begin(); l_i != l_c;) {
		PlaylistMap::value_type l_track_i = *l_i;
		ITrack *l_track = tracks[l_track_i.first];

		size_t l_offset = 0;
		size_t l_read = 0;
		do {
			/* decode */
			l_read += l_track->read(&buffer[l_offset],
			    l_buffer_max - l_offset);

			/* mix */
			switch (l_track->depth()) {

			case 16: for (size_t l_bi = (l_offset / 2); l_bi < (l_read / 2); ++l_bi)
					reinterpret_cast<int16_t *>(mix)[l_bi] =
					    Mixer(reinterpret_cast<int16_t *>(mix)[l_bi],
						  reinterpret_cast<int16_t *>(buffer)[l_bi],
						  l_track_i.second.second);
				break;

			default: for (size_t l_bi = l_offset; l_bi < l_read; ++l_bi)
					mix[l_bi] =
					    Mixer(mix[l_bi],
						  buffer[l_bi],
						  l_track_i.second.second);
				break;

			}
			l_offset = l_read;

			/*
			 * Success! Next track.
			 */
			if (l_read == l_buffer_max)
				++l_i;

			/*
			 * Failed! Underrun occured.
			 */
	 		else {
				/* auto-rewind */
				l_track->rewind();

				/* if looping forever, continue */
				if (-1 == l_track_i.second.first)
					continue;

				/* check if we need to stop */
				else if (0 == --l_track_i.second.first) {
					playlist.erase(l_i++);
					break;
				}

				/* update playlist */
				else playlist[l_track_i.first] = l_track_i.second;
			}
		}
		while(l_read < l_buffer_max);
	}
	
	if (!pcm->write(mix, l_frames_available))
		MMERROR("Failed to write to PCM device!");
}

/********************************************************************* Player */

Player::Player(void)
    : PIMPL_CREATE
{
}

Player::~Player(void)
{
	PIMPL_DESTROY;
}

void
Player::load(const Core::Identifier &id, ITrack *track)
{
	PIMPL->load(id, track);
}

bool
Player::contains(const Core::Identifier &id)
{
	return(PIMPL->contains(id));
}

ITrack *
Player::eject(const Core::Identifier &id)
{
	return(PIMPL->eject(id));
}

bool
Player::play(const Core::Identifier &id, int playlist, float gain)
{
	return(PIMPL->play(id, playlist, gain));
}

void
Player::stop(const Core::Identifier &id)
{
	PIMPL->stop(id);
}

bool
Player::isPlaying(const Core::Identifier &id) const
{
	return(PIMPL->isPlaying(id));
}

void
Player::tick(void)
{
	PIMPL->tick();
}

PCM *
Player::pcm(void) const
{
	return(PIMPL->pcm);
}

void
Player::setPCM(PCM *_pcm)
{
	PIMPL->setPCM(_pcm);
}

} /********************************************************** Audio Namespace */
MARSHMALLOW_NAMESPACE_END

