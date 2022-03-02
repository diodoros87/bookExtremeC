// Nazwa pliku: ExtremeC_examples_chapter15_2.c
// Opis: Ten przykład powoduje utworzenie trzech wątków, które wyświetlają
//              różne ciągi tekstowe. Poszczególne przeploty są narzucane
//              przez zarządcę procesów, co prowadzi do wygenerowania
//              odmiennych danych wyjściowych przez ten przykład.

#include <stdio.h>
#include <stdlib.h>

// Nagłówek standardu POSIX niezbędny podczas pracy z biblioteką pthread.
#include <pthread.h>

void* thread_body(void* arg) {
  char* str = (char*)arg;
  printf("%s", str);
  return NULL;
}

int main(int argc, char** argv) {
   printf("\n");
  // Uchwyty wątków.
  pthread_t thread1;
  pthread_t thread2;
  pthread_t thread3;

  // Utworzenie nowych wątków.
  int result1 = pthread_create(&thread1, NULL,
          thread_body, "jabłko");
  int result2 = pthread_create(&thread2, NULL,
          thread_body, "pomarańcza");
  int result3 = pthread_create(&thread3, NULL,
          thread_body, "cytryna");

  if (result1 || result2 || result3) {
    printf("Nie udało się utworzyć wątków.\n");
    exit(1); 
  }
   
  // Oczekiwanie na zakończenie działania wątków.
  
  result1 = pthread_join(thread1, NULL);
  result2 = pthread_join(thread2, NULL);
  result3 = pthread_join(thread3, NULL);

  /*
  result1 = pthread_detach(thread1);
  result2 = pthread_detach(thread2);
  result3 = pthread_detach(thread3);
  */
  if (result1 || result2 || result3) {
    printf("Nie udało się dołączyć wątków.\n");
    exit(2);
  }
  pthread_exit(NULL);
  printf("\n");
  //pthread_exit(NULL);
  return 0;
}
