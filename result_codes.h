#ifndef RESULT_CODES_H 
#define RESULT_CODES_H

typedef enum {
   INCORRECT_VALUE    = -5,
   OPEN_FILE_ERROR    = -4,
   RENAME_FILE_ERROR  = -3,
   FILE_CLOSE_ERROR   = -2,
   SYSTEM_ERROR       = -1,
   OK                 = 0,
   BAD_ALLOC          = 1,
   BAD_FUNTION_CALL   = 2,
   INVALID_ARG        = 3,
   BAD_CAST           = 4,
   REGEX_ERROR        = 5,
   OUT_OF_RANGE_ERROR = 6,
   RUNTIME_ERROR      = 7,
   STD_ERROR          = 8,
   UNRECOGNIZED_ERROR = 9,
   CHILD_PROCESS_FAIL = 127
} Result_codes;

#endif
