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

#include "core/logger.h"
#include "core/global.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

MARSHMALLOW_NAMESPACE_USE;

/********************************************************* memory allocation */

void *
operator new(size_t size)
{
	/* TODO: Replace with custom allocator */
	void *ptr = malloc(size);
	MMINFO("Requested allocation of %lu bytes - returning %p.", size, ptr);
	return(ptr);
}

void *
operator new[](size_t size)
{
	/* TODO: Replace with custom allocator */
	void *ptr = malloc(size);
	MMINFO("Requested array allocation of %lu bytes - returning %p.", size, ptr);
	return(ptr);
}

void
operator delete(void *ptr)
{
	/* TODO: Replace with memory manager */
	MMINFO("Requested deallocation of %p.", ptr);
	free(ptr);
}

void
operator delete[](void *ptr)
{
	/* TODO: Replace with custom allocator */
	MMINFO("Requested array deallocation of %p.", ptr);
	free(ptr);
}

/********************************************************************** eastl */

void *
operator new[](size_t size, const char *pName, int flags, unsigned debugFlags,
    const char *file, int line)
{
	/* TODO: Replace with custom allocator */
	MMUNUSED(pName);
	MMUNUSED(flags);
	MMUNUSED(debugFlags);
	MMUNUSED(file);
	MMUNUSED(line);
	return(new char[size]);
}

void *
operator new[](size_t size, size_t alignment, size_t alignmentOffset, const
    char *pName, int flags, unsigned debugFlags, const char *file, int line)
{
	/* TODO: Replace with custom allocator */
	MMUNUSED(alignment);
	MMUNUSED(alignmentOffset);
	MMUNUSED(pName);
	MMUNUSED(flags);
	MMUNUSED(debugFlags);
	MMUNUSED(file);
	MMUNUSED(line);
	return(new char[size]);
}

int
Vsnprintf8(char8_t *pDestination, size_t n, const char8_t *pFormat, va_list arguments)
{
	return(vsnprintf(pDestination, n, pFormat, arguments));
}

