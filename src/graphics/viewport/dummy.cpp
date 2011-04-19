/*
 * Copyright 2011 Marshmallow Engine. All rights reserved.
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

#include "graphics/viewport.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/platform.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Graphics;

const char *Viewport::Name("DUMMY");

struct Viewport::Internal
{
};

bool
Viewport::Initialize(int w, int h, int d, bool f)
{
	INFO("Dummy viewport initialized a %d bit %dx%d display (%s)", d, w, h, f ? "FULLSCREEN" : "WINDOWED");
	return(true);
}

void
Viewport::Finalize(void)
{
	INFO1("Dummy viewport finalized");
}

bool
Viewport::Redisplay(int w, int h, int d, bool f)
{
	INFO("Dummy viewport redisplayed using a %d bit %dx%d display (%s)", d, w, h, f ? "FULLSCREEN" : "WINDOWED");
	return(true);
}

void
Viewport::Tick(TIME t)
{
	UNUSED(t);
	INFO1("Dummy viewport ticked");
}

void
Viewport::SwapBuffer(void)
{
	INFO1("Dummy viewport swapped imaginary buffer");
}

const Math::Size2
Viewport::Size(void)
{
	return(Math::Size2());
}

