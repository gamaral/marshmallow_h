/*
 * Copyright (c) 2011-2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
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

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <csignal>
#include <cstdio>
#include <cstring>

#include "core/logger.h"

#include "event/eventmanager.h"
#include "event/quitevent.h"

extern int MMain(int argc, char *argv[]);

MARSHMALLOW_NAMESPACE_USE

static void
SignalHandler(int signal, siginfo_t *siginfo, void *context)
{
	MMUNUSED(signal);
	MMUNUSED(siginfo);
	MMUNUSED(context);

	if (!Event::EventManager::Instance()) {
		MMERROR("\n*** Unix system signal received. But can't queueing quit event message yet... Ignoring. ***\n");
		return;
	}

	MMDEBUG("\n*** Unix system signal received. Queueing quit event message. ***\n");
	Event::EventManager::Instance()->queue(new Event::QuitEvent);
}

int
main(int argc, char *argv[])
{
	/* prep signal action*/

	struct sigaction action;
	memset(&action, 0, sizeof(action));
	action.sa_sigaction = &SignalHandler;
	action.sa_flags = SA_SIGINFO;

	/* handle signals */
	sigaction(SIGINT,  &action, NULL);
	sigaction(SIGQUIT, &action, NULL);
	sigaction(SIGTERM, &action, NULL);

	return(MMain(argc, argv));
}

