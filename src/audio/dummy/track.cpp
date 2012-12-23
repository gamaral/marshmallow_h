/*
 * Copyright 2012 Marshmallow Engine. All rights reserved.
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

#include "audio/track.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/logger.h"
#include "core/shared.h"

#include "audio/icodec.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Audio { /****************************************** Audio Namespace */

struct Track::Private
{
	Private(const Audio::SharedCodec &_codec)
	    : codec(_codec)
	    , timer(0)
	    , iterations(0)
	{}

	Audio::SharedCodec codec;

	float timer;
	int iterations;
};

/********************************************************************* Track */

Track::Track(const Audio::SharedCodec &_codec)
    : m_p(new Private(_codec))
{
}

Track::~Track(void)
{
	delete m_p, m_p = 0;
}

bool
Track::play(int _iterations)
{
	m_p->iterations = _iterations;
	m_p->timer = 0.f;
	tick(0);
	return(m_p->iterations != 0);
}

void
Track::stop(bool _force)
{
	if (!m_p->iterations) return;

	m_p->iterations = _force ? 0 : 1;
}

bool
Track::isPlaying(void) const
{
	return(m_p->iterations);
}

void
Track::tick(float delta)
{
	if (m_p->iterations <= 0) return;

	m_p->timer += delta;

	if (m_p->timer > 10.f) {
		m_p->timer = 0.f;
		--m_p->iterations;
	}
}

bool
Track::isValid(void) const
{
	return(m_p->codec && m_p->codec->isOpen());
}

} /********************************************************** Audio Namespace */
MARSHMALLOW_NAMESPACE_END

