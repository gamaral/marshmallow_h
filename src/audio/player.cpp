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

#include <cstring>

#include "core/logger.h"
#include "core/weak.h"

#include "audio/icodec.h"
#include "audio/pcm.h"

#include "backend_p.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Audio { /****************************************** Audio Namespace */

struct Player::Private
{
	Private(void)
	    : bsize(0)
	    , bdecoded(0)
	    , buffer(0)
	    , iterations(0)
	    , skip_decode(false)
	{
	}

	~Private(void)
	{
		/* make sure we clsoe the PCM device */
		stop();
	}

	bool play(int iterations);
	void stop(void);

	void update(void);

	WeakPCM pcm;
	SharedCodec codec;
	size_t bsize;
	size_t bdecoded;
	char *buffer;
	int iterations;
	bool skip_decode;
};

bool
Player::Private::play(int _iterations)
{
	/* sanity checks */
	if (_iterations == 0) {
		MMDEBUG("Trying to play player with no iterations");
		if (iterations) stop();
		return(false);
	}
	else if (iterations) {
		MMERROR("Player already playing!");
		return(false);
	}

	iterations = _iterations;
	skip_decode = false;
	codec->reset();

	bsize = pcm->bufferSize();
	buffer = new char[bsize];

	update();

	return(true);
}

void
Player::Private::stop(void)
{
	delete buffer, buffer = 0;
	bsize = 0;
	iterations = 0;
}

void
Player::Private::update(void)
{
	if (!buffer) return;

	/* sanity checks */

	if (!pcm) {
		MMDEBUG("Trying to play on an invalid PCM, stopping!");
		stop();
		return;
	}

	if (!pcm->isOpen()) {
		MMDEBUG("Trying to play on a closed PCM, stopping!");
		stop();
		return;
	}

	/* stop check */

	if (!skip_decode) {

		if (!iterations) {
			stop();
			return;
		}

		memset(buffer, 0, bsize);
		size_t l_read = codec->read(buffer, bsize);

		/*
		 * Reaching the end, check if we need to loop
		 */
		if (l_read < bsize && (iterations == -1 || --iterations)) {
			codec->reset();
			l_read += codec->read(buffer + l_read, bsize - l_read);
		}
		bdecoded = l_read;
	}
	else skip_decode = false;

	skip_decode = !pcm->mix(buffer, bdecoded);
}

/********************************************************************* Player */

Player::Player(void)
    : m_p(new Private)
{
}

Player::Player(const Audio::WeakPCM &_pcm, const Audio::SharedCodec &_codec)
    : m_p(new Private)
{
	setPCM(_pcm);
	setCodec(_codec);
}

Player::~Player(void)
{
	delete m_p, m_p = 0;
}

const Audio::WeakPCM &
Player::pcm(void) const
{
	return(m_p->pcm);
}

void
Player::setPCM(const Audio::WeakPCM &_pcm)
{
	if (isPlaying()) {
		MMERROR("Tried to replace PCM of a playing player!");
		return;
	}

	m_p->pcm = _pcm;
}

const Audio::SharedCodec &
Player::codec(void) const
{
	return(m_p->codec);
}

void
Player::setCodec(const Audio::SharedCodec &_codec)
{
	if (isPlaying()) {
		MMERROR("Tried to replace codec of a playing player!");
		return;
	}

	m_p->codec = _codec;
}

bool
Player::play(int _iterations)
{
	if (!isValid()) {
		MMWARNING("Tried to play invalid player!");
		return(false);
	}
	return(m_p->play(_iterations));
}

void
Player::stop(void)
{
	if (!isValid()) {
		MMWARNING("Tried to stop an invalid player!");
		return;
	}
	m_p->stop();
}

bool
Player::isPlaying(void) const
{
	return(m_p->iterations);
}

void
Player::tick(float)
{
	m_p->update();
}

bool
Player::isValid(void) const
{
	return(m_p->pcm && m_p->codec && m_p->pcm->isOpen() && m_p->codec->isOpen());
}

} /********************************************************** Audio Namespace */
MARSHMALLOW_NAMESPACE_END

