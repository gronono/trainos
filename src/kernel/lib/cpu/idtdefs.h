#pragma once

#define EXCEPTION_DIVIDE_BY_ZERO                    0
#define EXCEPTION_DEBUG                             1
#define EXCEPTION_NON_MASKABLE                      2
#define EXCEPTION_BREAKPOINT                        3
#define EXCEPTION_OVERFLOW                          4
#define EXCEPTION_BOUND_RANGE_EXCEEDED              5
#define EXCEPTION_INVALID_OPCODE                    6
#define EXCEPTION_DEVICE_NOT_AVAILABLE              7
#define EXCEPTION_DOUBLE_FAULT                      8
#define EXCEPTION_COPROCESSOR_SEGMENT_OVERRUN       9
#define EXCEPTION_INVALID_TSS                      10
#define EXCEPTION_SEGMENT_NOT_PRESENT              11
#define EXCEPTION_STACK_SEGMENT_FAULT              12
#define EXCEPTION_GENERAL_PROTECTION_FAULT         13
#define EXCEPTION_PAGE_FAULT                       14
// #define Reserved	                               15
#define EXCEPTION_X87_FLOATING_POINT               16
#define EXCEPTION_ALIGNMENT_CHECK                  17
#define EXCEPTION_MACHINE_CHECK                    18
#define EXCEPTION_SIMD_FLOATING_POINT              19
#define EXCEPTION_VIRTUALIZATION                   20
#define EXCEPTION_CONTROL_PROTECTION               21
// #define Reserved	                               22->27
#define EXCEPTION_HYPERVISOR_INJECTION             28
#define EXCEPTION_VMM_COMMUNICATION                29
#define EXCEPTION_SECURITY                         30
// #define Reserved	                               31

