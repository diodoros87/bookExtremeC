// Nazwa pliku: ExtremeC_examples_chapter16_4.c
// Opis: Ten przykład pokazuje, jak zapomniany wskaźnik
//              może powstać przez współdzielenie adresu
//              wewnątrz stosu wątku.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <errno.h> // Plik nagłówkowy wymagany do działania funkcji errno() i strerror().

#include <pthread.h>

int* shared_int;
pthread_barrier_t barrier1;
pthread_barrier_t barrier2;

typedef struct {
   pthread_barrier_t * barrier1;
   pthread_barrier_t * barrier2;
} shared_state_t;

void barrier_init(pthread_barrier_t * barrier, const pthread_barrierattr_t * attr, unsigned int count) {
   if (pthread_barrier_init(barrier, attr, count)) {
      fprintf(stderr, "pthread_barrier_init error: %s\n", strerror(errno));
      exit(1);
   }
}

void barrier_wait(pthread_barrier_t * barrier) {
   pthread_barrier_wait(barrier);
//    if (NULL != barrier && pthread_barrier_wait(barrier)) {
//       fprintf(stderr, "pthread_barrier_wait error: %s\n", strerror(errno));
//       exit(1);
//    }
}

void* t1_body(void* arg) {
   shared_state_t * ss = (shared_state_t *) arg;
   int local_var = 100;
   shared_int = &local_var;
   // Oczekiwanie, aż drugi wątek wyświetli współdzieloną liczbę całkowitą.
   barrier_wait(ss->barrier1);
   //if (usleep(10))
   //   fprintf(stderr, " usleep error: %s\n", strerror(errno));
   barrier_wait(ss->barrier2);
   return NULL;
}

void* t2_body(void* arg) {
   shared_state_t * ss = (shared_state_t *) arg;
   barrier_wait(ss->barrier1);
  printf("%d\n", *shared_int);
  barrier_wait(ss->barrier2);
  return NULL;
}

int main(int argc, char** argv) {
  shared_int = NULL;
  pthread_barrier_t barrier1;
  pthread_barrier_t barrier2;
  barrier_init(&barrier1, NULL, 2);
  barrier_init(&barrier2, NULL, 2);
   shared_state_t shared_state = { &barrier1, &barrier2 };
  pthread_t t1;
  pthread_t t2;

  if(pthread_create(&t1, NULL, t1_body, &shared_state))
     fprintf(stderr, " pthread_create error: %s\n", strerror(errno));
  if(pthread_create(&t2, NULL, t2_body, &shared_state))
     fprintf(stderr, " pthread_create error: %s\n", strerror(errno));

  if(pthread_join(t1, NULL))
     fprintf(stderr, " pthread_join error: %s\n", strerror(errno));
  if(pthread_join(t2, NULL))
   fprintf(stderr, " pthread_join error: %s\n", strerror(errno));

  return 0;
}
