// Nazwa pliku: ExtremeC_examples_chapter15_1.c
// Opis: Utworzenie nowego wątku wykonującego proste zadanie!

#include <stdio.h>
#include <stdlib.h>

// Nagłówek standardu POSIX niezbędny podczas pracy z biblioteką pthread.
#include <pthread.h>

// Ta funkcja zawiera logikę, która powinna zostać wykonana
// w oddzielnym wątku.
void* thread_body(void* arg) {
  printf("Witaj z pierwszego wątku!\n");
  return NULL;
} 

int main(int argc, char** argv) {

  // Uchwyt wątku.
  pthread_t thread;

  // Utworzenie nowego wątku.
  int result = pthread_create(&thread, NULL, thread_body, NULL);
  // Jeżeli utworzenie nowego wątku zakończyło się niepowodzeniem.
  if (result) {
    printf("Nie udało się utworzyć nowego wątku. Kod błędu: %d\n",
            result);
    exit(1);
  }
//pthread_exit(NULL);

  result = pthread_join(thread, NULL);
  if (result) {
    printf("Dołączenie nowego wątku zakończyło się niepowodzeniem. Kod błędu: %d\n",
            result);
    exit(2);
  } 
  //pthread_exit(NULL);
  return 0;
}
