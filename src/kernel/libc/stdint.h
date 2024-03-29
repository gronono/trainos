#pragma once

/**
 * https://en.cppreference.com/w/cpp/header/cstdint
 */


/** Unsigned 8-bit integer type. */
typedef unsigned char       uint8_t;
/** Signed 8-bit integer type. */
typedef signed char         int8_t;
/** Unsigned 16-bit integer type. */
typedef unsigned short      uint16_t;
/** Signed 16-bit integer type. */
typedef short               int16_t;
/** Unsigned 32-bit integer type. */
typedef unsigned int        uint32_t;
/** Signed 32-bit integer type. */
typedef int                 int32_t;
/** Unsigned 64-bit integer type. */
typedef unsigned long long  uint64_t;
/** Signed 64-bit integer type. */
typedef long long           int64_t;

#if __x86_64__
typedef uint64_t            size_t;
typedef uint64_t            uintptr_t;
typedef uint64_t            uintmax_t;
typedef int64_t             intmax_t;
typedef int64_t             ptrdiff_t;
#else
typedef uint32_t            size_t;
typedef uint32_t            uintptr_t;
typedef uint32_t            uintmax_t;
typedef int32_t             intmax_t;
typedef int32_t             ptrdiff_t;
#endif
