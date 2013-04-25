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

#include "audio/oggcodec.h"
#include "audio/wavecodec.h"

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
audio_codec_wave_test(void)
{
	Core::SharedBufferIO l_sample_wav =
	    new Core::BufferIO(reinterpret_cast<char *>(sample_wav), sample_wav_len);

	Core::SharedBufferIO l_sample_ogg =
	    new Core::BufferIO(reinterpret_cast<char *>(sample_ogg), sample_ogg_len);

	Audio::SharedCodec l_codec = new Audio::WaveCodec;

	l_codec->open(l_sample_wav.staticCast<Core::IDataIO>());
	ASSERT_TRUE("Audio::WaveCodec::open()", l_codec->isOpen());
	l_codec->close();
	ASSERT_FALSE("Audio::WaveCodec::close()", l_codec->isOpen());

	l_codec->open(l_sample_ogg.staticCast<Core::IDataIO>());
	ASSERT_FALSE("Audio::WaveCodec::open() NON-WAVE", l_codec->isOpen());
}

void
audio_codec_ogg_test(void)
{
	Core::SharedBufferIO l_sample_ogg =
	    new Core::BufferIO(reinterpret_cast<char *>(sample_ogg), sample_ogg_len);

	Core::SharedBufferIO l_sample_wav =
	    new Core::BufferIO(reinterpret_cast<char *>(sample_wav), sample_wav_len);

	Audio::SharedCodec l_codec = new Audio::OggCodec;

	l_codec->open(l_sample_ogg.staticCast<Core::IDataIO>());
	ASSERT_TRUE("Audio::OggCodec::open()", l_codec->isOpen());
	l_codec->close();
	ASSERT_FALSE("Audio::OggCodec::close()", l_codec->isOpen());

	l_codec->open(l_sample_wav.staticCast<Core::IDataIO>());
	ASSERT_FALSE("Audio::OggCodec::open() NON-OGG", l_codec->isOpen());
}

TESTS_BEGIN
    TEST(audio_codec_wave_test)
    TEST(audio_codec_ogg_test)
TESTS_END

