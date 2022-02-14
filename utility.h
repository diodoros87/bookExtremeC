#ifndef UTILITY_H
#define UTILITY_H

#include <setjmp.h>
#include <stdnoreturn.h>

extern jmp_buf JMP_BUF;

_Noreturn void longjump_test(jmp_buf env, const int value);

#define REQUIRE_NON_NULL(pointer, message) \
if (! (pointer) ) { \
   LOG_EXIT(__FUNCTION__, (message), EXIT_FAILURE); \
}

typedef int bool_t;

#endif
