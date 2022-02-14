#include "utility.h"
#include "print.h"

jmp_buf JMP_BUF;

_Noreturn void longjump_test(jmp_buf env, const int value) {
   FUNCTION_INFO(__FUNCTION__);
   longjmp(env, value);
}
