#pragma once

/**
 * A macro representing a null pointer.
 *
 * NULL is defined as a void pointer to address 0, commonly used to indicate
 * that a pointer does not point to any valid memory location.
 */
#define NULL ((void*) 0)

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

/**
 * Boolean type.
 *
 * The bool type is defined as an 8-bit unsigned integer, where 0 represents false
 * and any non-zero value represents true.
 */
typedef uint8_t             bool;
/** Macro representing the boolean value true. */
#define true	            1
/** Macro representing the boolean value false. */
#define false	            0

#ifdef __x86_64__
/** Defines the size_t type as uint64_t on 64-bit systems. */
typedef uint64_t            size_t;
#else
/** Defines the size_t type as uint32_t on 32-bit systems. */
typedef uint32_t            size_t;
#endif

/**
 * Type for status.
 */
typedef enum {
    /** Operation successful */
    STATUS_SUCCESS = 0,
    /**< Operation failed */
    STATUS_FAILURE = 1
} Status;
