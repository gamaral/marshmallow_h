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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef MARSHMALLOW_CORE_STRHASH_H
#define MARSHMALLOW_CORE_STRHASH_H 1

#include <core/hash.h>

#include <string>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Core { /******************************************** Core Namespace */

	/*!
	 * An extended Core:Hash that uses a string as the buffer to hash.
	 */
	class MARSHMALLOW_CORE_EXPORT
	StrHash : public Hash
	{
		struct Private;
		Private *m_p;

	public:

		StrHash(void);

		/*!
		 * @param str String used for hash
		 */
		StrHash(const char *str);

		/*!
		 * @param str String used for hash
		 */
		StrHash(const std::string &str);
		StrHash(const StrHash &copy);
		virtual ~StrHash(void);

		/*!
		 * @brief Unique ID
		 */
		MMUID uid(void) const
		    { return(result()); }

		/*!
		 * @brief Hashed String
		 */
		const std::string & str(void) const;

	public: /* operator */

		operator const std::string & (void) const;

		operator const char * (void) const;

		StrHash & operator=(const StrHash &rhs);

	public:
		static const StrHash Null;
	};

} /*********************************************************** Core Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
