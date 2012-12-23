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

#include <cstring>

#include "audio/track.h"
#include "audio/wavecodec.h"
#include "audio/oggcodec.h"

#include "core/platform.h"
#include "core/bufferio.h"
#include "core/identifier.h"
#include "core/shared.h"

#include "tests/common.h"

#include "sample_ogg.h"
#include "sample_wav.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

MARSHMALLOW_NAMESPACE_USE

void
audioasset_wave_test(void)
{
	Core::SharedBufferIO l_sample_wav =
	    new Core::BufferIO(reinterpret_cast<char *>(sample_wav), sample_wav_len);

	Audio::SharedCodec l_codec = new Audio::WaveCodec;

	l_codec->open(l_sample_wav.staticCast<Core::IDataIO>());
	TEST("Open WAVE Codec", l_codec->isOpen());

	Audio::Track l_track(l_codec);
	TEST("Valid WAVE Track", l_track.isValid());

	l_track.play(1);
	while(l_track.isPlaying()) {
		Core::Platform::Sleep(16);
		l_track.tick(16);
	}
	l_track.stop();
}

void
audioasset_ogg_test(void)
{
	Core::SharedBufferIO l_sample_ogg =
	    new Core::BufferIO(reinterpret_cast<char *>(sample_ogg), sample_ogg_len);

	Audio::SharedCodec l_codec = new Audio::OggCodec;

	l_codec->open(l_sample_ogg.staticCast<Core::IDataIO>());
	TEST("Open Ogg Codec", l_codec->isOpen());

	Audio::Track l_track(l_codec);
	TEST("Valid Ogg Track", l_track.isValid());

	l_track.play(1);
	while(l_track.isPlaying()) {
		Core::Platform::Sleep(16);
		l_track.tick(16);
	}
	l_track.stop();
}

int
MMain(int argc, char *argv[])
{
	MMUNUSED(argc);
	MMUNUSED(argv);

	audioasset_wave_test();
	audioasset_ogg_test();

	return(TEST_EXITCODE);
}

