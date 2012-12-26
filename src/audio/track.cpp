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

#include <cstring>

#include "core/logger.h"
#include "core/shared.h"

#include "audio/icodec.h"

#include "backend_p.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Audio { /****************************************** Audio Namespace */

struct Track::Private
{
	Private(const SharedCodec &_codec)
	    : codec(_codec)
	    , bsize(0)
	    , bdecoded(0)
	    , buffer(0)
	    , handle(0)
	    , iterations(0)
	    , persistent(false)
	    , skip_decode(false)
	{
	}

	~Private(void)
	{
		/* make sure we clsoe the PCM device */
		persistent = false;
		stop();
	}

	void PCM_open(void);
	void PCM_close(void);

	bool play(int iterations, bool persistent);
	void stop(void);

	void update(void);

	SharedCodec codec;
	size_t bsize;
	size_t bdecoded;
	char *buffer;
	Backend::PCM::Handle *handle;
	int iterations;
	bool persistent;
	bool skip_decode;
};

void
Track::Private::PCM_open(void)
{
	handle = Backend::PCM::Open(codec->rate(), codec->depth(), codec->channels());
	Backend::PCM::Buffer(handle, buffer, bsize);
}

void
Track::Private::PCM_close(void)
{
	Backend::PCM::Close(handle), handle = 0;
	bsize = 0;
	buffer = 0;
}

bool
Track::Private::play(int _iterations, bool _persistent)
{
	/* sanity checks */
	if (_iterations == 0) {
		MMDEBUG("Trying to play track with no iterations");
		if (iterations) stop();
		return(false);
	}
	else if (iterations) {
		MMERROR("Track already playing!");
		return(false);
	}

	iterations = _iterations;
	persistent = _persistent;
	skip_decode = false;
	codec->reset();

	/* open PCM device */
	if (!handle) PCM_open();

	update();

	return(true);
}

void
Track::Private::stop(void)
{
	/* sanity check */
	if (!handle) return;

	iterations = 0;

	/* close PCM device */
	if (!persistent) PCM_close();
}

void
Track::Private::update(void)
{
	/* sanity check */
	if (!handle) return;

	/* stop check */
	if (!iterations) {
		stop();
		return;
	}

	if (!skip_decode) {
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

	skip_decode = !Backend::PCM::Write(handle, bdecoded);
}

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
Track::play(int _iterations, bool _persistent)
{
	return(m_p->play(_iterations, _persistent));
}

void
Track::stop(void)
{
	m_p->stop();
}

bool
Track::isPlaying(void) const
{
	return(m_p->iterations);
}

void
Track::tick(float)
{
	m_p->update();
}

bool
Track::isValid(void) const
{
	return(m_p->codec && m_p->codec->isOpen());
}

} /********************************************************** Audio Namespace */
MARSHMALLOW_NAMESPACE_END

