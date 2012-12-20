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

#include "audio/track.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <cstring>

#include "core/identifier.h"
#include "core/logger.h"
#include "core/shared.h"

MARSHMALLOW_NAMESPACE_BEGIN
namespace Audio { /****************************************** Audio Namespace */

struct Track::Private
{
	Private(const Core::Identifier &_id, const Core::SharedDataIO &_dio)
	    : id(_id)
	    , dio(_dio)
	    , channels(0)
	    , depth(0)
	    , rate(0)
	    , loaded(false)
	{}

	inline bool load(void);
	inline void unload(void);

	Core::Identifier id;
	Core::SharedDataIO dio;
	unsigned int channels;
	unsigned int depth;
	unsigned int rate;
	bool loaded;
};

bool
Track::Private::load(void)
{
	if (!dio->isOpen() && !dio->open(Core::DIOReadOnly)) {
		MMERROR("Failed to open audio stream for " << id.str());
		return(false);
	}

	char l_ident[5] = { 0, 0, 0, 0, 0 };

	/*
	 * Read identifier used for format detection
	 */
	dio->seek(0, Core::DIOStart);
	dio->read(l_ident, 4);

	/*
	 * Ogg file
	 */
	if (0 == strcmp(l_ident, "OggS")) {
		MMDEBUG("Detected OGG file " << id.str());
		/* TODO(gamaral) IMPLEMENT */
		loaded = false;
	}

	/*
	 * RIFF/WAV file
	 */
	else if (0 == strcmp(l_ident, "RIFF")) {
		MMDEBUG("Detected RIFF file " << id.str());

		dio->seek(4, Core::DIOCurrent);
		dio->read(l_ident, 4);

		if (0 == strcmp(l_ident, "WAVE")) {
			MMDEBUG("Confirmed WAVE file " << id.str());
			loaded = true;
		}

		dio->seek(-8, Core::DIOCurrent);
	}


	return(loaded);
}

void
Track::Private::unload(void)
{
	loaded = false;
}

/***************************************************************** Track */

Track::Track(const Core::Identifier &_id, const Core::SharedDataIO &_dio)
    : m_p(new Private(_id, _dio))
{
}

Track::~Track(void)
{
	delete m_p, m_p = 0;
}

unsigned int
Track::channels(void) const
{
	return(m_p->channels);
}

unsigned int
Track::depth(void) const
{
	return(m_p->depth);
}

unsigned int
Track::rate(void) const
{
	return(m_p->rate);
}

bool
Track::load(void)
{
	return(m_p->load());
}

void
Track::unload(void)
{
	m_p->unload();
}

bool
Track::isLoaded(void) const
{
	return(m_p->loaded);
}

const Core::Identifier &
Track::id(void) const
{
	return(m_p->id);
}

const Core::Type &
Track::type(void) const
{
	static const Core::Type s_type("Audio::Track");
	return(s_type);
}

} /********************************************************** Audio Namespace */
MARSHMALLOW_NAMESPACE_END

