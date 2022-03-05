// Nazwa pliku: ExtremeC_examples_chapter15_1_2.c
// Opis: Przykład 15.1 używający wątku odłączonego.

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
  int i = 0;
  int j = 0;
  int a = 0;
  i++;
  printf("%d\n", i);
  printf("%d\n", ++j);
  printf("%d\n", ++a);

  // Utworzenie nowego wątku.
  int result = pthread_create(&thread, NULL, thread_body, NULL);
  // Jeżeli utworzenie nowego wątku zakończyło się niepowodzeniem.
  if (result) {
    printf("Nie udało się utworzyć nowego wątku. Kod błędu: %d\n",
            result);
    exit(1);
  }

  // Odłączenie wątku.
  result = pthread_detach(thread);
  // Jeżeli wątek jest odłączony, jego działanie nie kończy się sukcesem.
  if (result) {
    printf("Wątek nie mógł zostać odłączony. Kod błędu: %d\n",
            result);
    exit(2);
  }

  // Zakończenie działania wątku głównego.
  pthread_exit(NULL);

  return 0;
}
