/*
#define MANUAL_DLL_LOAD
*/
#include "print.h"
#include "connector.h"
#include "demo_derived_functions.h"
#include "utility.h"
#include "human_functions.h"
#include "human_derived_functions.h"
#include "demo_functions.h"
#include "human.h"
#include "base_cpp_test.h"
#include "derived_cpp_test.h"
#include "variadic_template_test.h"
#include "base_test.h"
#include "derived_test.h"

#include <stdlib.h>
#include <limits.h>
#include <string.h>

void handle_terminate(void) { 
   LOG("%s", "handler called - must've been some exception ?!\n");
}

static void at_exit (void) {
   puts (" At exit\n");
   FUNCTION_INFO(__FUNCTION__);
   destroy_regex();  /* test_human_linking and test_human_derived use NAME_REGEX allocated in heap  */
}

static Result_codes main_test_linking(void) {
   static const Result_codes (*TESTS [])(void) = { test_demo_linking, test_demo_derived,
      test_human_linking, test_human_derived, test_money,
      test_base_cpp, test_derived_cpp,
      test_base, test_derived
   };
   atexit (at_exit);
   const size_t SIZE = sizeof(TESTS) / sizeof(TESTS[0]);
   Result_codes result = OK;
   for (size_t i = 0; i < SIZE && result == OK; i++) {
      result = TESTS[i]();
      assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   }

   return result;
}

const int test_print_many_longjmp = 22;

static _Noreturn void test_print_many(void) {
   float f = 6.0f;
   LOG("\nAddress pointed by money is: %p\n", &f);            /* mandatory casting when using va_list function  */
   print_many("1 test of print_many", "p G Lg o  rr7 d G s d s", (float*)(&f), f, (long double)(-5.0), 399, 6.0, "QQQQQQ", 7, "rrrr");
   int i = 66;
   LOG("\nAddress pointed by money is: %p\n", &i);
   print_many("2 test of print_many", " p  d       Lg  d G s d s", (int*)(&i), i, -5.0L, 399, 6.0, "QQQQQQ", 7, "rrrr");
   void * nul = &i;
   print_many(" 3 test of print_many", " p  c hd hu F G F G  u  lu ld llu lld ", nul, 'i', (short)7, (unsigned short)USHRT_MAX, 6.0f, 7.0f,
                           77.8, 66.6, (unsigned)99, (unsigned long)ULONG_MAX, (long)LONG_MAX, (unsigned long long)ULLONG_MAX, (long long)LLONG_MAX);
   longjmp(JMP_BUF, test_print_many_longjmp);
}

static Result_codes tests(void) {
   volatile int jmp_value = 0;
   const int jmp_value_2 = -35;
   if ((jmp_value = setjmp(JMP_BUF)) != 0) {
      FUNCTION_INFO(__FUNCTION__);
      LOG("\nAfter calling test_print_many longjmp set value to %d\n", jmp_value);
      assert_many(test_print_many_longjmp == jmp_value, "assert failed: ", "s d", "jmp_value should be ", test_print_many_longjmp);
      if ((jmp_value = setjmp(JMP_BUF)) != 0) {
         FUNCTION_INFO(__FUNCTION__);
         LOG("\nlongjmp set value to %d\n Return with %d ?\n", jmp_value, EXIT_FAILURE);
         assert_many(jmp_value_2 == jmp_value, "assert failed: ", "s d", "jmp_value should be ", jmp_value_2);/*
         return EXIT_FAILURE;*/
      }
      else 
         longjump_test(JMP_BUF, jmp_value_2);
      
      return main_test_linking(); 
   }
   else 
      test_print_many();
}

int main(const int argc, const char ** argv) {
   const char * program_name = strrchr(argv[0], '/');
   LOG("%s\n", program_name ? ++program_name : argv[0]);
   FUNCTION_INFO(__FUNCTION__);
   set_handler(handle_terminate);
   Result_codes result = tests();
   LOG(" Program name = %s", program_name);
   FUNCTION_INFO(__FUNCTION__);
   LOG(" Final result = %d\n", result);
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   return result;
}
