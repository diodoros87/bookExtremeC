#include "utility.h"
#include "print.h"
#include "result_codes.h"
#include "system.h"
#include "c_string.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
/*
Result_codes change_makefile(void) {
   struct File_modify_t * modifier = NULL;
   Result_codes result = File_modify_init(&modifier, "Makefile");
   if (result == OK)
      result = File_modify_set(modifier, "Makefile");  
   if (result == OK) {
      char * filename = NULL;
      result = File_modify_get_filename(modifier, &filename);
      LOG("File name = %s\n", filename);
      free(filename);
   }
   if (result == OK)
      result = edit_makefile(modifier);
   File_modify_destroy(&modifier);
   free(modifier);
   assert_many(modifier == NULL, "assert failed: ", "s p", "pointer to File_modify_t == ", modifier);
   return OK;
}
*/
Result_codes make_clean_make(void) {
   char *exec_args[] = { "make", "clean", NULL, NULL};
   int result = execute(exec_args);
   //if (result != SYSTEM_ERROR) 
   //  result = change_makefile();
   if (result == OK) { 
      exec_args[0] = "make 2> compilation_output.txt";
      result = call_system(exec_args[0]);
      /*
      exec_args[1] = NULL;
      if (execute(exec_args) != SYSTEM_ERROR)
         result = OK;*/
   }
   LOG("Parent process: pid = %d\nGoodbye!\n", getpid());
   return result;
}

int launch_execs_in_directory(const bool_t valgrind, const char * const ld_path, const char * const directory, const char * const executables[],
   const size_t EXEC_SIZE, const size_t repetitions) {
   FUNCTION_INFO(__FUNCTION__);
   if (NULL == directory || NULL == executables || NULL == ld_path || EXEC_SIZE == 0) {
      LOG("\ndirectory = %d, executables = %p, ld_path = %p, EXEC_SIZE = %zu\n", directory, executables, ld_path, EXEC_SIZE);
      return INVALID_ARG;
   }
   
   const char * command = NULL;
   int result = OK;
   const char * const valgrind_str = valgrind ? "valgrind --leak-check=full --show-leak-kinds=all --exit-on-first-error=yes --error-exitcode=1 --tool=memcheck --track-origins=yes" : "";
   //if (valgrind)
   //   valgrind_str = "valgrind --leak-check=full --show-leak-kinds=all --exit-on-first-error=yes --error-exitcode=1 --tool=memcheck --track-origins=yes";
   //size_t EXEC_SIZE = sizeof(executables[i]) / sizeof(executables[i][0]);
   for (int i = 0; i < EXEC_SIZE; i++) {
      command = concatenate_many(ld_path, " ", valgrind_str, " ./", directory, executables[i], NULL);
      for (int counter = 0; counter < repetitions && result == OK; counter++)
         result = call_system(command);
      free(command);
   }
   return result;
}

#define LAUNCH_EXECS(dir_idx, executables, repetitions) \
   launch_execs_in_directory(valgrind, ld_path, directories[dir_idx], executables, sizeof(executables) / sizeof(executables[0]), repetitions)

int test_threading(const bool_t valgrind) {
   FUNCTION_INFO(__FUNCTION__);
   static const char * const ld_path = "LD_LIBRARY_PATH=.";
   static const char * const directories[] = { "ch15/", "ch16/" };
   static const char * const ch15[] = { /*"join_thread",  "detach_thread", "race",*/
      "race_corrupt_memory"/*, "race_to_data" */}; 
   static const char * const ch16[] = { /*"race_to_data_mutex", "race_cond_var", "race_barrier", */
      "race_to_data_sem", "h2o"
   };
   int result = LAUNCH_EXECS(0, ch15, 0);
   if (OK == result)
      result = LAUNCH_EXECS(1, ch16, 3);
   return result;
}

int main(const int argc, const char * argv[]) {
   const char * program_name = strrchr(argv[0], '/');
   LOG(" Program name = %s\n", program_name ? ++program_name : argv[0]);
   FUNCTION_INFO(__FUNCTION__);
   
   const bool_t valgrind = (argc == 2 && strcmp(argv[1], "valgrind") == 0) ? 1 : 0;
   int result = test_threading (valgrind);
   assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   //if (result == OK)
   //   result = make_clean_make();
   //assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   //if (result == OK)
   //   result = test_threading(valgrind);
   //assert_many(result == OK, "assert failed: ", "s d", "result == ", result);
   LOG(" Program name = %s", program_name);
   FUNCTION_INFO(__FUNCTION__);
   LOG(" Final result = %d\n", result);
   return result;
}
