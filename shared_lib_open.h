#ifndef SHARED_LIB_OPEN_H
#define SHARED_LIB_OPEN_H

#ifdef __cplusplus
 #define EXTERNC extern "C"
 #else
 #define EXTERNC
 #endif

EXTERNC void * get_handle (char * filepath, int flag);
EXTERNC void * get_symbol (void * handle, char * symbol);
EXTERNC int close_handle(void ** handle);

#undef EXTERNC

#endif
