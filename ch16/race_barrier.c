// Nazwa pliku: ExtremeC_examples_chapter16_1_barrier.c
// Opis: Ten przykład pokazuje użycie barier
//              do synchronizacji dwóch wątków.

#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

// Obiekt bariery.
pthread_barrier_t barrier;

void* thread_body_1(void* arg) {
  printf("A");
  // Oczekiwanie na dołączenie pierwszego wątku.
  pthread_barrier_wait(&barrier);
  return NULL;
}

void* thread_body_2(void* arg) {
  // Oczekiwanie na dołączenie drugiego wątku.
  pthread_barrier_wait(&barrier);
  printf("B");
  return NULL;
}

int main(int argc, char** argv) {

  // Inicjalizacja obiektu bariery.
  pthread_barrier_init(&barrier, NULL, 2);

  // Uchwyty wątków.
  pthread_t thread1;
  pthread_t thread2;
   printf("----------------------- %s \t\t\t%d\n", __FUNCTION__, barrier);
  // Utworzenie nowych wątków.
  int result1 = pthread_create(&thread1, NULL,
          thread_body_1, NULL);
  int result2 = pthread_create(&thread2, NULL,
          thread_body_2, NULL);

  if (result1 || result2) {
    printf("Nie udało się utworzyć wątków.\n");
    exit(1);
  }

  // Oczekiwanie na zakończenie działania wątków.
  result1 = pthread_join(thread1, NULL);
  result2 = pthread_join(thread2, NULL);

  if (result1 || result2) {
    printf("Nie udało się dołączyć wątków.\n");
    exit(2);
  }
  
  printf("----------------------- %s \t\t\t%d\n", __FUNCTION__, barrier);

  // Usunięcie obiektu bariery.
  pthread_barrier_destroy(&barrier);
  
  printf("----------------------- %s \t\t\t%d\n", __FUNCTION__, barrier);

  return 0;
}
