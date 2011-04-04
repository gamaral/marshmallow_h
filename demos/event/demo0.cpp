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

#include <cstdio>
#include <iostream>

#include "core/shared.h"
#include "event/debuglistener.h"
#include "event/eventbase.h"
#include "event/eventtype.h"

MARSHMALLOW_NAMESPACE_USE;
using namespace Core;
using namespace Event;

int
main(void)
{
	EventBase event1;
	EventBase event2;
	SharedEventInterface event3(new EventBase);
	SharedEventInterface event4(event3);
	SharedEventInterface event5;
	event5 = event4;

	DebugListener listener1("log.txt");

	printf(" Event%d: %s (%u)\n", 1, event1.type().name(), event1.type().uid());
	printf(" Event%d: %s (%u)\n", 2, event2.type().name(), event2.type().uid());
	printf(" Event%d: %s (%u)\n", 3, event3->type().name(), event3->type().uid());
	printf(" Event%d: %s (%u)\n", 4, event4->type().name(), event4->type().uid());
	printf(" Event%d: %s (%u)\n", 5, event5->type().name(), event5->type().uid());

	bool same = (event1.type() == event2.type());

	listener1.handle(event1);
	listener1.handle(event2);
	listener1.handle(*event3);
	listener1.handle(*event4);
	listener1.handle(*event5);

	std::cout << "Are events the same? " << same << std::endl;
	return 0;
}

