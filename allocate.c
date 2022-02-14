#include "allocate.h"
#include "c_string.h"
#include "print.h"

#include <string.h>

void * copy_bytes(const void * source, const size_t bytes) {
   if (! source || bytes == 0) { 
      LOG_EXIT(__FUNCTION__, "source string is null or number of bytes is 0", EXIT_FAILURE);
   }
   void * result = NULL;
   ALLOCATE_STRING(result, bytes);
   if (NULL != result)
      memcpy(result, source, bytes);
   return result;
}
