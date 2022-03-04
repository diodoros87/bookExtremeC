// Nazwa pliku: ExtremeC_examples_chapter16_1_barrier.c
// Opis: Ten przykład pokazuje użycie barier
//              do synchronizacji dwóch wątków.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <pthread.h>

#include "../print.h"

// Obiekt bariery.
//pthread_barrier_t barrier;

typedef struct {
   pthread_barrier_t * barriers[2];
   const char * const string;
} state_t;

void* thread_body(void* arg) {
   state_t * state = (state_t *) arg;
   if (NULL != state->barriers[0])
      pthread_barrier_wait(state->barriers[0]);
   LOG("%s", state->string);
   if (NULL != state->barriers[1])
      pthread_barrier_wait(state->barriers[1]);
  return NULL;
}
/*
void* thread_body_2(void* arg) {
  // Oczekiwanie na dołączenie drugiego wątku.
  pthread_barrier_wait(&barrier);
  printf("B");
  return NULL;
}
*/
void barrier_init(pthread_barrier_t * barrier, const pthread_barrierattr_t * attr, unsigned int count) {
   if (pthread_barrier_init(barrier, attr, count)) {
      LOG("sem_open error: %s\n", strerror(errno));
      exit(1);
   }
}

int main(int argc, char** argv) {
  pthread_barrier_t barrier[2];
   for(size_t i = 0; i < 2; i++)
      barrier_init(&barrier[i], NULL, 2);
   
   state_t states [3] = { { { &barrier[0], &barrier[1] }, "B" },
                        { { &barrier[1], NULL }, "C" }, 
                           { { NULL, &barrier[0] }, "A" }
   } ;
   
   pthread_t thread [3];
   printf("----------------------- %s \t\t\t%p\n", __FUNCTION__, barrier);
   for(int i = 0; i < 3; i++)
      if (pthread_create(thread + i, NULL, thread_body, states + i)) {
         printf("Nie udało się utworzyć wątków.\n");
         exit(2);
      }
      
   for(int i = 0; i < 3; i++)
      if (pthread_join(thread[i], NULL)) {
         printf("Nie udało się dołączyć wątków.\n");
         exit(3);
      }

  // Usunięcie obiektu bariery.
  for(int i = 0; i < 2; i++)
      if (pthread_barrier_destroy(barrier + i)) {
         printf("Nie udało się zwolnic barier.\n");
         exit(4);
      }
  
  printf("----------------------- %s \t\t\t%p\n", __FUNCTION__, barrier);

  return 0;
}
