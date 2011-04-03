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

#include "event/debuglistener.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include "core/platform.h"
#include "event/eventinterface.h"
#include "event/eventtype.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Core;
using namespace Event;

const char *DebugListener::s_name = "DebugListener";

DebugListener::DebugListener(const char *filename)
{
	m_filestream.open(filename, std::ios_base::app);
}

DebugListener::~DebugListener(void)
{
	m_filestream.close();
}

bool
DebugListener::handle(const EventInterface &event)
{
#define DL_MSGMAX 255
	char l_msg[DL_MSGMAX];
	const char  *l_name = event.type().name();
	const UID l_uid = event.type().uid();

	snprintf(l_msg, DL_MSGMAX-1, "%s: Event (%p) received of type %u [%s]",
		Platform::TimeStampToTimeData(event.timeStamp()).string,
		(void *)&event,
		l_uid,
		l_name);

	m_filestream << l_msg << std::endl;

	return false;
}

