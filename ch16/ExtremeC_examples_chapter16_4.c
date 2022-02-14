// Nazwa pliku: ExtremeC_examples_chapter16_4.c
// Opis: Ten przykład pokazuje, jak zapomniany wskaźnik
//              może powstać przez współdzielenie adresu
//              wewnątrz stosu wątku.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>

int* shared_int;

void* t1_body(void* arg) {
  int local_var = 100;
  shared_int = &local_var;
  // Oczekiwanie, aż drugi wątek wyświetli współdzieloną liczbę całkowitą.
  usleep(10);
  return NULL;
}

void* t2_body(void* arg) {
  printf("%d\n", *shared_int);
  return NULL;
}

int main(int argc, char** argv) {

  shared_int = NULL;

  pthread_t t1;
  pthread_t t2;

  pthread_create(&t1, NULL, t1_body, NULL);
  pthread_create(&t2, NULL, t2_body, NULL);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  return 0;
}
