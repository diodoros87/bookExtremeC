// Nazwa pliku: ExtremeC_examples_chapter16_3.c
// Opis: Ten przykład pokazuje zdefiniowanie wielkości sterty
//              i adresu stosu nowego wątku

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include <pthread.h>

void* thread_body_1(void* arg) {
  int local_var = 0;
  printf("Wątek1 > Adres stosu: %p\n", (void*)&local_var);
  return 0;
}

void* thread_body_2(void* arg) {
  int local_var = 0;
  printf("Wątek2 > Adres stosu: %p\n", (void*)&local_var);
  return 0;
}

int main(int argc, char** argv) {

  size_t buffer_len = PTHREAD_STACK_MIN + 100;
  // Bufor zaalokowany na stercie i użyty jako
  // obszar stosu wątku.
  char *buffer = (char*)malloc(buffer_len * sizeof(char));

  // Uchwyty wątków.
  pthread_t thread1;
  pthread_t thread2;

  // Utworzenie nowego wątku. with default attributes
  int result1 = pthread_create(&thread1, NULL,
          thread_body_1, NULL);

  // Utworzenie nowego wątku. with a custom stack region
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  // Określenie adresu stosu i jego wielkości.
  if (pthread_attr_setstack(&attr, buffer, buffer_len)) {
    printf("Nie udało się zdefiniować atrybutów stosu.\n");
    exit(1);
  }
  int result2 = pthread_create(&thread2, &attr,
          thread_body_2, NULL);

  if (result1 || result2) {
    printf("Nie udało się utworzyć wątków.\n");
    exit(2);
  }

  printf("Wątek główny > Adres sterty: %p\n", (void*)buffer);
  printf("Wątek główny > Adres stosu: %p\n", (void*)&buffer_len);

  // Oczekiwanie na zakończenie działania wątków.
  result1 = pthread_join(thread1, NULL);
  result2 = pthread_join(thread2, NULL);

  if (result1 || result2) {
    printf("Nie udało się dołączyć wątków.\n");
    exit(3);
  }

  free(buffer);

  return 0;
}
