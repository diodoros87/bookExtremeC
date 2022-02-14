#ifndef ALLOCATE_H
#define ALLOCATE_H

#include <stdlib.h>

#define ALLOCATE_TYPE(pointer, Type, n) \
(pointer) = (Type *) malloc (sizeof(Type) * n); \
if ((pointer) == NULL) { \
   LOG("%s", "out of memory: malloc() returns NULL ");  \
}

#define ALLOCATE_SINGLE_TYPE(pointer, Type) \
ALLOCATE_TYPE(pointer, Type, 1)

void * copy_bytes(const void * source, const size_t bytes);

#endif
