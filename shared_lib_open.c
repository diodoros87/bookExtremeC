#include <dlfcn.h>

#include <stdlib.h>

#include "print.h"

void * get_handle (char * filepath, int flag) {
   void * result = dlopen(filepath, flag);
   if (NULL == result) {
      LOG_EXIT(__FUNCTION__, dlerror(), EXIT_FAILURE);  /* brackets - multiline macro */
   }
   return result;
}

void * get_symbol (void * handle, char * symbol) {
   void * result = dlsym(handle, symbol);
   if (NULL == result) {
      LOG_EXIT(__FUNCTION__, dlerror(), EXIT_FAILURE);   /* brackets - multiline macro */
   }
   return result;
}

int close_handle(void ** handle) {
   if (! handle) {
      LOG_EXIT(__FUNCTION__, "handle is NULL ", EXIT_FAILURE);   /* brackets - multiline macro */
   }
   if (! *handle) {
      LOG_EXIT(__FUNCTION__, "address pointed by handle is NULL ", EXIT_FAILURE);   /* brackets - multiline macro */
   }   
   const int result = dlclose(*handle);
   if (0 != result) {
      LOG_EXIT(__FUNCTION__, "Closing handle was failed", result);   /* brackets - multiline macro */
   }
   return result;
}
