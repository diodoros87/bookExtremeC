#ifndef SINGLETON_H
#define SINGLETON_H

#include <stdlib.h>

#define ALLOC(buffer, type, n) \
(buffer) = (type *) malloc ((n) * sizeof(type)); \
if ((buffer) == NULL) { \
   LOG("%s", "out of memory: malloc() returns NULL ");  \
}
/*
#define SINGLETON_STACK(type, function, ...) type* Singleton_Stack_##type() { \
                  static type instance = function(&instance, __VA_ARGS__);               \
                  return &instance;                       \
               }
               */
#define SINGLETON_STACK(type, value) type* Singleton_Stack_##type() { \
                  static type instance = value;               \
                  return &instance;                       \
               }
               /*
#define SINGLETON_HEAP(type, function, ...) type** Singleton_##type() { \
                  static type* instance = NULL;                           \
                  if (NULL == instance) {                   \
                     ALLOC(instance, type, 1);           \
                     if (instance)                          \
                        *instance = function(__VA_ARGS__);      \
                  }                                         \
                  return &instance;                       \
               }
               */
               /*
#define SINGLETON_HEAP(type, function, ...) type* Singleton_##type() { \
                  static type* instance = NULL;                           \
                  if (NULL == instance) {                   \
                     ALLOC(instance, type, 1);           \
                     if (instance)                          \
                        *instance = function(__VA_ARGS__);      \
                  }                                         \
                  return instance;                       \
               }
*/
               
#define SINGLETON_HEAP(type, function, ...) type* Singleton_Heap_##type() { \
                  static type* instance = NULL;             \
                  if (NULL == instance) {                   \
                     ALLOC(instance, type, 1);           \
                     if (instance)                          \
                        function(instance, __VA_ARGS__);      \
                  }                                         \
                  return instance;                       \
               }
               
#define INIT_SINGLETON(pointer, singleton_function)   \
if (NULL == pointer)    \
      pointer = singleton_function();

#endif
