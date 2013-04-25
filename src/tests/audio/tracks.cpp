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

#include <cstring>

#include "audio/oggtrack.h"
#include "audio/wavetrack.h"

#include "core/fileio.h"
#include "core/identifier.h"
#include "core/platform.h"
#include "core/shared.h"

#include "tests/common.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

MARSHMALLOW_NAMESPACE_USE

static const Core::Identifier s_ogg_file("audio/data/sample.ogg");
static const Core::Identifier s_wav_file("audio/data/sample.wav");

void
audio_track_wave_test(void)
{
	Core::FileIO sample_wav(s_wav_file);

	Audio::WaveTrack track_wav(sample_wav);
	ASSERT_TRUE("Audio::WaveTrack::isValid()", track_wav.isValid());
	if (!track_wav.isValid())
		return;

	const int frame_size =
	    track_wav.channels() * (track_wav.depth() / 8);

	char frame[frame_size];
	ssize_t track_read = track_wav.read(frame, frame_size);
	ASSERT_EQUAL("Audio::WaveTrack::read()", track_read, frame_size);

	const bool track_rewind = track_wav.rewind();
	ASSERT_TRUE("Audio::WaveTrack::rewind()", track_rewind);

	char frame_confirm[frame_size];
	track_read = track_wav.read(frame_confirm, frame_size);
	ASSERT_EQUAL("Audio::WaveTrack::read() REPEAT", track_read, frame_size);

	/* compare frames */
	bool frame_ok = true;
	for (int i = 0; i < frame_size && frame_ok; ++i)
		frame_ok = (frame[i] == frame_confirm[i]);
	ASSERT_TRUE("Audio::WaveTrack::read() FRAME OK", frame_ok);


	bool track_seek = track_wav.seek(8);
	ASSERT_TRUE("Audio::WaveTrack::seek(8)", track_seek);

	track_read = track_wav.read(frame, frame_size);
	ASSERT_EQUAL("Audio::WaveTrack::read()", track_read, frame_size);

	track_seek = track_wav.seek(8);
	ASSERT_TRUE("Audio::WaveTrack::seek(8)", track_seek);

	track_read = track_wav.read(frame_confirm, frame_size);
	ASSERT_EQUAL("Audio::WaveTrack::read() REPEAT", track_read, frame_size);

	/* compare frames */
	frame_ok = true;
	for (int i = 0; i < frame_size && frame_ok; ++i)
		frame_ok = (frame[i] == frame_confirm[i]);
	ASSERT_TRUE("Audio::WaveTrack::read() FRAME OK", frame_ok);
}

void
audio_track_wave_fail_test(void)
{
	Core::FileIO sample_ogg(s_ogg_file);

	Audio::WaveTrack track_ogg(sample_ogg);
	ASSERT_FALSE("Audio::WaveTrack::isValid()", track_ogg.isValid());
}

void
audio_track_ogg_test(void)
{
	Core::FileIO sample_ogg(s_ogg_file);

	Audio::OggTrack track_ogg(sample_ogg);
	ASSERT_TRUE("Audio::OggTrack::isValid()", track_ogg.isValid());
	if (!track_ogg.isValid())
		return;

	const int frame_size =
	    track_ogg.channels() * (track_ogg.depth() / 8);

	char frame[frame_size];
	ssize_t track_read = track_ogg.read(frame, frame_size);
	ASSERT_EQUAL("Audio::OggTrack::read()", track_read, frame_size);

	const bool track_rewind = track_ogg.rewind();
	ASSERT_TRUE("Audio::OggTrack::rewind()", track_rewind);

	char frame_confirm[frame_size];
	track_read = track_ogg.read(frame_confirm, frame_size);
	ASSERT_EQUAL("Audio::OggTrack::read()", track_read, frame_size);

	/* compare frames */
	bool frame_ok = true;
	for (int i = 0; i < frame_size && frame_ok; ++i)
		frame_ok = (frame[i] == frame_confirm[i]);
	ASSERT_TRUE("Audio::OggTrack::read() FRAME OK", frame_ok);

	bool track_seek = track_ogg.seek(8);
	ASSERT_TRUE("Audio::OggTrack::seek(8)", track_seek);

	track_read = track_ogg.read(frame, frame_size);
	ASSERT_EQUAL("Audio::OggTrack::read()", track_read, frame_size);

	track_seek = track_ogg.seek(8);
	ASSERT_TRUE("Audio::OggTrack::seek(8)", track_seek);

	track_read = track_ogg.read(frame_confirm, frame_size);
	ASSERT_EQUAL("Audio::OggTrack::read() REPEAT", track_read, frame_size);

	/* compare frames */
	frame_ok = true;
	for (int i = 0; i < frame_size && frame_ok; ++i)
		frame_ok = (frame[i] == frame_confirm[i]);
	ASSERT_TRUE("Audio::OggTrack::read() FRAME OK", frame_ok);
}

void
audio_track_ogg_fail_test(void)
{
	Core::FileIO sample_wav(s_wav_file);
	Audio::OggTrack track_ogg(sample_wav);
	ASSERT_FALSE("Audio::OggTrack::isValid()", track_ogg.isValid());
}

TESTS_BEGIN
    TEST(audio_track_wave_test)
    TEST(audio_track_wave_fail_test)
    TEST(audio_track_ogg_test)
    TEST(audio_track_ogg_fail_test)
TESTS_END

