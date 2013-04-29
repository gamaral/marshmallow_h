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

#ifndef MARSHMALLOW_CORE_HASH_H
#define MARSHMALLOW_CORE_HASH_H 1

#include <core/environment.h>
#include <core/global.h>
#include <core/namespace.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Core { /******************************************** Core Namespace */

	/*!
	 * As it's name suggests Core::Hash is used hash a random assortment of
	 * data into a Unique Identifier (UID) that can later be used to do
	 * quick comparisons.
	 */
	class MARSHMALLOW_CORE_EXPORT
	Hash
	{
		PRIVATE_IMPLEMENTATION
	public:

		Hash(void);

		/*!
		 * Hash Contructor
		 *
		 * @param d Data to hash
		 * @param length Data length
		 * @param mask UID mask
		 */
		Hash(const char *d, size_t length, MMUID mask);

		/*!
		 * Hash Copy Contructor
		 *
		 * @param copy Hash
		 */
		Hash(const Hash &copy);
		virtual ~Hash(void);

		/*!
		 * @brief Datum
		 */
		MMUID result(void) const;

	public: /* operator */

		operator MMUID() const;

		Hash & operator=(const Hash &rhs);

		bool operator==(const Hash &rhs) const;
		bool operator!=(const Hash &rhs) const;
		bool operator<(const Hash &rhs) const;

	public: /* static */

		/*!
		 * @brief Hash algorithm
		 *
		 * One-at-a-time hash is used.
		 */
		static MMUID Algorithm(const char *data, size_t length, MMUID mask);

	protected:

		void rehash(const char *d, size_t length, MMUID mask);
	};

} /*********************************************************** Core Namespace */
MARSHMALLOW_NAMESPACE_END

#endif
