// Nazwa pliku: ExtremeC_examples_chapter16_5.c
// Opis: Ten przykład pokazuje pięć wątków przeprowadzających
//       operacje na stercie zaalokowanej w tablicy współdzielonej.
//        W tym kodzie zostały użyte bariery w celu zapewnienia
//        możliwości synchronizacji wątków.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>

#include "../print.h"

#define CHECK_RESULT(result) \
if (result) { \
  printf("Wystąpił błąd biblioteki pthread.\n"); \
  exit(1); \
}

#define CHECK_NULL(pointer) \
if (! (pointer)) { \
   LOG_EXIT(__FUNCTION__, " pointer is null ", EXIT_FAILURE); \ 
}

int TRUE = 1;
int FALSE = 0;

// Wskaźnik prowadzący do tablicy współdzielonej.
char* shared_array;
// Wielkość tablicy współdzielonej.
size_t shared_array_len;

#ifdef __linux__ //__APPLE__
   typedef struct  {
      pthread_mutex_t barrier_mutex;
      pthread_cond_t  barrier_cv;
      unsigned int    barrier_thread_count;
      unsigned int    barrier_round;
      unsigned int    barrier_thread_limit;
   } barrier;
   
   void barrier_init(barrier * b, unsigned limit) {
      CHECK_NULL(b);
      pthread_mutex_init(&(b->barrier_mutex), NULL);
      pthread_cond_init(&(b->barrier_cv), NULL);
      b->barrier_thread_count = 0;
      b->barrier_thread_limit = limit;
      b->barrier_round = 0;
   }
   
   void barrier_destroy(barrier * b) {
      CHECK_NULL(b);
      pthread_mutex_destroy(&b->barrier_mutex);
      pthread_cond_destroy(&b->barrier_cv);
   }

   void barrier_wait(barrier * b) {
      CHECK_NULL(b);
      pthread_mutex_lock(&(b->barrier_mutex));
      b->barrier_thread_count++;
      if (b->barrier_thread_count >= b->barrier_thread_limit) {
         b->barrier_thread_count = 0;
         b->barrier_round++;
         pthread_cond_broadcast(&(b->barrier_cv));
      } else {
         unsigned int my_round = b->barrier_round;
         do {
            pthread_cond_wait(&(b->barrier_cv), &(b->barrier_mutex));
         } while (my_round == b->barrier_round);
      }
      pthread_mutex_unlock(&(b->barrier_mutex));
   }
   
   barrier alloc_barrier;
   barrier fill_barrier;
   barrier done_barrier;
#else
   pthread_barrier_t alloc_barrier;
   pthread_barrier_t fill_barrier;
   pthread_barrier_t done_barrier;
#endif

void* alloc_thread_body(void* arg) {
  shared_array_len = 20;
  shared_array = (char*)malloc(shared_array_len * sizeof(char*));
#ifdef __linux__ //__APPLE__
  barrier_wait(&alloc_barrier);
#else
  pthread_barrier_wait(&alloc_barrier);
#endif
  return NULL;
}

void* filler_thread_body(void* arg) {
#ifdef __linux__ //__APPLE__
  barrier_wait(&alloc_barrier);
#else
  pthread_barrier_wait(&alloc_barrier);
#endif
  int even = *((int*)arg);
  char c = 'a';
  size_t start_index = 1;
  if (even) {
    c = 'Z';
    start_index = 0;
  }
  for (size_t i = start_index; i < shared_array_len; i += 2) {
    shared_array[i] = even ? c-- : c++;
  }
  shared_array[shared_array_len - 1] = '\0';
#ifdef __linux__ //__APPLE__
  barrier_wait(&fill_barrier);
#else
  pthread_barrier_wait(&fill_barrier);
#endif
  return NULL;
}

void* printer_thread_body(void* arg) {
#ifdef __linux__ //__APPLE__
  barrier_wait(&fill_barrier);
#else
  pthread_barrier_wait(&fill_barrier);
#endif
  printf(">> %s\n", shared_array);
#ifdef __linux__ //__APPLE__
  barrier_wait(&done_barrier);
#else
  pthread_barrier_wait(&done_barrier);
#endif
  return NULL;
}

void* dealloc_thread_body(void* arg) {
#ifdef __linux__ //__APPLE__
  barrier_wait(&done_barrier);
#else
  pthread_barrier_wait(&done_barrier);
#endif
  
  free(shared_array);
  
#ifdef __linux__ //__APPLE__
  barrier_destroy(&alloc_barrier);
  barrier_destroy(&fill_barrier);
  barrier_destroy(&done_barrier);
#else
  pthread_barrier_destroy(&alloc_barrier);
  pthread_barrier_destroy(&fill_barrier);
  pthread_barrier_destroy(&done_barrier);
#endif
  return NULL;
}

int main(int argc, char** argv) {

  shared_array = NULL;
#ifdef __linux__ //__APPLE__
  barrier_init(&alloc_barrier, 3);
  barrier_init(&fill_barrier, 4);
  barrier_init(&done_barrier, 3);
#else
  pthread_barrier_init(&alloc_barrier, NULL, 3);
  pthread_barrier_init(&fill_barrier, NULL, 4);
  pthread_barrier_init(&done_barrier, NULL, 3);
#endif

  pthread_t alloc_thread;
  pthread_t even_filler_thread;
  pthread_t odd_filler_thread;
  pthread_t printer_thread_1;
  pthread_t printer_thread_2;
  pthread_t dealloc_thread;

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  int res = pthread_attr_setdetachstate(&attr,
          PTHREAD_CREATE_DETACHED);
  CHECK_RESULT(res);

  res = pthread_create(&alloc_thread, &attr,
          alloc_thread_body, NULL);
  CHECK_RESULT(res);

  res = pthread_create(&even_filler_thread,
          &attr, filler_thread_body, &TRUE);
  CHECK_RESULT(res);

  res = pthread_create(&odd_filler_thread,
          &attr, filler_thread_body, &FALSE);
  CHECK_RESULT(res);

  res = pthread_create(&printer_thread_1, &attr,
          printer_thread_body, NULL);
  CHECK_RESULT(res);
  
  res = pthread_create(&printer_thread_2, &attr,
          printer_thread_body, NULL);
  CHECK_RESULT(res);

  res = pthread_create(&dealloc_thread, &attr,
          dealloc_thread_body, NULL);
  CHECK_RESULT(res);

  pthread_exit(NULL);

  return 0;
}
