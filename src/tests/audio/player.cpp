/*
 * Copyright (c) 2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
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

#include <cstring>

#include "core/fileio.h"
#include "core/identifier.h"
#include "core/platform.h"

#include "audio/backend_p.h"
#include "audio/pcm.h"
#include "audio/player.h"
#include "audio/wavetrack.h"

#include "game/config.h"

#include "tests/common.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

MARSHMALLOW_NAMESPACE_USE

static const Core::Identifier s_sample_track("sample");
static const Core::Identifier s_sample_track2("sample2");
static const Core::Identifier s_wav_file("audio/data/sample.wav");
static const Core::Identifier s_wav_file2("audio/data/sample2.wav");

void
audio_player_track_test(void)
{
	Core::FileIO sample_wav(s_wav_file);

	Audio::WaveTrack track_wav(sample_wav);
	ASSERT_TRUE("Audio::WaveTrack::isValid()", track_wav.isValid());
	if (!track_wav.isValid())
		return;

	Audio::Player player;
	player.load(s_sample_track, &track_wav);

	bool track_found = player.contains(s_sample_track);
	ASSERT_TRUE("Audio::Player::contains(sample) LOADED", track_found);

	track_found = player.contains("bogus");
	ASSERT_FALSE("Audio::Player::contains(bogus)", track_found);

	player.eject(s_sample_track);

	track_found = player.contains(s_sample_track);
	ASSERT_FALSE("Audio::Player::contains(sample) EJECTED", track_found);
}

void
audio_player_play_test(void)
{
	Core::FileIO sample_wav(s_wav_file);

	Audio::WaveTrack track_wav(sample_wav);
	ASSERT_TRUE("Audio::WaveTrack::isValid()", track_wav.isValid());
	if (!track_wav.isValid())
		return;

	Audio::Player player;
	player.load(s_sample_track, &track_wav);

	player.play(s_sample_track);
	bool track_playing = player.isPlaying(s_sample_track);
	ASSERT_TRUE("Audio::Player::isPlaying(sample) STARTED", track_playing);

	track_playing = player.isPlaying("bogus");
	ASSERT_FALSE("Audio::Player::isPlaying(bogus)", track_playing);

	player.stop(s_sample_track);
	track_playing = player.isPlaying(s_sample_track);
	ASSERT_FALSE("Audio::Player::isPlaying(sample) STOPPED", track_playing);
}

void
audio_player_tick_test(void)
{
	Audio::Backend::Initialize();

	Core::FileIO sample_wav(s_wav_file);
	Audio::WaveTrack track_wav1(sample_wav);
	ASSERT_TRUE("Audio::WaveTrack::isValid() TRACK 1", track_wav1.isValid());
	if (!track_wav1.isValid())
		return;

	Core::FileIO sample_wav2(s_wav_file2);
	Audio::WaveTrack track_wav2(sample_wav2);
	ASSERT_TRUE("Audio::WaveTrack::isValid() TRACK 2", track_wav2.isValid());
	if (!track_wav2.isValid())
		return;

	Audio::PCM pcm(track_wav1.rate(),
	               track_wav1.depth(),
	               track_wav1.channels());
	ASSERT_TRUE("Audio::PCM::isOpen()", pcm.isOpen());
	if (!pcm.isOpen())
		return;

	Audio::Player player;
	player.setPCM(&pcm);
	player.load(s_sample_track, &track_wav1);
	player.load(s_sample_track2, &track_wav2);
	player.play(s_sample_track, 2, .5f);
	player.play(s_sample_track2, 1, .5f);
	if (!player.isPlaying(s_sample_track))
		return;

	while (player.isPlaying(s_sample_track)) {
		player.tick();
		Core::Platform::Sleep(1000/MARSHMALLOW_ENGINE_FRAMERATE);
	}
	
	/* wait for pcm to flush */
	Core::Platform::Sleep(500);
	pcm.close();

	Audio::Backend::Finalize();
}

TESTS_BEGIN
    TEST(audio_player_track_test)
    TEST(audio_player_play_test)
    TEST(audio_player_tick_test)
TESTS_END

