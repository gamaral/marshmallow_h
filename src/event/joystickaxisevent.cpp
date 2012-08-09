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

#include "event/joystickaxisevent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/platform.h"

MARSHMALLOW_NAMESPACE_USE
using namespace Event;

struct JoystickAxisEvent::Private
{
	int min;
	int max;
	int fuzz;
	int flat;
};

JoystickAxisEvent::JoystickAxisEvent(
    Input::Joystick::Axis axis_,
    int value_,
    int min_,
    int max_,
    int fuzz_,
    int flat_,
    size_t source_,
    MMTIME timestamp_)
    : InputEvent(JoystickType, axis_, value_, source_, timestamp_)
    , m_p(new Private)
{
	m_p->min = min_;
	m_p->max = max_;
	m_p->fuzz = fuzz_;
	m_p->flat = flat_;
}

JoystickAxisEvent::~JoystickAxisEvent(void)
{
	delete m_p, m_p = 0;
}

int
JoystickAxisEvent::min(void) const
{
	return(m_p->min);
}

int
JoystickAxisEvent::max(void) const
{
	return(m_p->max);
}

int
JoystickAxisEvent::flat(void) const
{
	return(m_p->flat);
}

int
JoystickAxisEvent::fuzz(void) const
{
	return(m_p->fuzz);
}

const Core::Type &
JoystickAxisEvent::Type(void)
{
	static const Core::Type s_type("Event::JoystickAxisEvent");
	return(s_type);
}

