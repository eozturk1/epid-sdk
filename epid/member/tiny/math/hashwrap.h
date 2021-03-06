/*############################################################################
# Copyright 2017-2018 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
############################################################################*/
#ifndef EPID_MEMBER_TINY_MATH_HASHWRAP_H_
#define EPID_MEMBER_TINY_MATH_HASHWRAP_H_
/// Declaration of hash wrap function
/*! \file */
#include <stddef.h>
#include "epid/common/types.h"
#include "epid/member/tiny/math/mathtypes.h"

/// Enable sha512 support
#define SHA512_SUPPORT
/// Enable sha512_256 support
#define SHA512_256_SUPPORT
/// Enable sha384 support
#define SHA384_SUPPORT
/// Enable sha256 support
#define SHA256_SUPPORT

#ifdef SHA512_SUPPORT
#include "epid/member/tiny/math/sha512.h"
#endif

#ifdef SHA512_256_SUPPORT
#include "epid/member/tiny/math/sha512_256.h"
#endif

#ifdef SHA384_SUPPORT
#include "epid/member/tiny/math/sha384.h"
#endif

#ifdef SHA256_SUPPORT
#include "epid/member/tiny/math/sha256.h"
#endif

/*! \def SHORTEST_HASH_ALG
Hash algorithm with a shortest digest.
*/
#if defined(SHA256_SUPPORT)
#define SHORTEST_HASH_ALG kSha256
#elif defined(SHA512_256_SUPPORT)
#define SHORTEST_HASH_ALG kSha512_256
#elif defined(SHA384_SUPPORT)
#define SHORTEST_HASH_ALG kSha384
#elif defined(SHA512_SUPPORT)
#define SHORTEST_HASH_ALG kSha512
#else
#error \
    "All hash algoritms disabled. " \
    "Enable at least one defining <HASH NAME>_SUPPORT."
#endif

/// Sha Digest Element
typedef union sha_digest {
#ifdef SHA512_SUPPORT
  uint8_t sha512_digest[SHA512_DIGEST_SIZE];  ///< Support digest for sha512
#endif
#ifdef SHA512_256_SUPPORT
  uint8_t sha512_256_digest[SHA512_256_DIGEST_SIZE];  ///< Support digest for
                                                      /// sha512_256
#endif
#ifdef SHA384_SUPPORT
  uint8_t sha384_digest[SHA384_DIGEST_SIZE];  ///< Support digest for sha384
#endif
#ifdef SHA256_SUPPORT
  uint8_t sha256_digest[SHA256_DIGEST_SIZE];  ///< Support digest for sha256
#endif
  uint8_t digest[1];  ///< Pointer to digest
} sha_digest;

/// Tiny Sha wrapper Element
typedef struct tiny_sha {
  union {
#ifdef SHA512_SUPPORT
    sha512_state sha512s;  ///< The state of sha512 if supported
#endif
#ifdef SHA512_256_SUPPORT
    sha512_256_state sha512_256s;  ///< The state of sha512_256 if supported
#endif
#ifdef SHA384_SUPPORT
    sha384_state sha384s;  ///< The state of sha384 if supported
#endif
#ifdef SHA256_SUPPORT
    sha256_state sha256s;  ///< The state of sha256 if supported
#endif
  } sha_state_t;     ///< Store state of hash algorithm
  HashAlg hash_alg;  ///< The hash algorithm which selected
} tiny_sha;

/// Initializes the hash state
/*!

\param[in] sha_type
Type of hash algorithm

\param[in,out] s
The hash state to initialize.

\returns A value different from zero (i.e., true) if on success.
         Zero (i.e., false) otherwise.
*/
int tinysha_init(HashAlg sha_type, tiny_sha* s);

/// Hashes data into state using a chosen hash algorithm
/*!

\param[in,out] s
The hash state. Must be non-null or behavior is undefined.

\param[in] data
The data to hash into s.

\param[in] data_length
The size of data in bytes.
*/
void tinysha_update(tiny_sha* s, void const* data, size_t data_length);

/// Computes the hash algorithm in the digest buffer
/*!

\param[out] digest
The computed digest. Must be non-null or behavior is undefined.

\param[in] s
The hash state. Must be non-null or behavior is undefined.
*/
void tinysha_final(unsigned char* digest, tiny_sha* s);

/// Returns size of digest depending on hash algorithm
/*!

\param[in] s
The hash state. Must be non-null or behavior is undefined.

\returns
Size of digest
*/
size_t tinysha_digest_size(tiny_sha* s);

#endif  // EPID_MEMBER_TINY_MATH_HASHWRAP_H_
