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

#pragma once

#ifndef MARSHMALLOW_CORE_GLOBAL_H
#define MARSHMALLOW_CORE_GLOBAL_H 1

#define MARSHMALLOW_VERSION ((MARSHMALLOW_VERSION_MAJOR << 24) \
                           + (MARSHMALLOW_VERSION_MINOR << 16) \
                           + (MARSHMALLOW_VERSION_BUILD <<  8) \
                           +  MARSHMALLOW_VERSION_REVISION)

#define MARSHMALLOW_VERSION_MAJOR    0x00
#define MARSHMALLOW_VERSION_MINOR    0x01
#define MARSHMALLOW_VERSION_BUILD    0x00
#define MARSHMALLOW_VERSION_REVISION 0x0b

#define VIRTUAL
#define NO_ASSIGN_COPY(x) NO_ASSIGN(x); \
	        NO_COPY(x)
#define NO_COPY(x) x(const x&)
#define NO_ASSIGN(x) x& operator=(const x&)

#define PRIVATE_IMPLEMENTATION struct Private;\
		Private *m_p;

#define PIMPL m_p
#define PIMPL_CREATE PIMPL(new Private)
#define PIMPL_CREATE_X(...) PIMPL(new Private(__VA_ARGS__))
#define PIMPL_DESTROY delete PIMPL, PIMPL = 0

#define MMIGNORE (void)
#define MMUNUSED(x) static_cast<void>(x)

#define MMMIN(x,y) (x < y ? x : y)
#define MMMAX(x,y) (x > y ? x : y)
#define MMRANGE(x,y,z) (y < x ? x : ( y > z ? z : y))

#endif
