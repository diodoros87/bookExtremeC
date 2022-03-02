// Nazwa pliku: ExtremeC_examples_chapter15_3_mutex.c
// Opis: Ten przykład używa muteksu do wyeliminowania stanu
//              wyścigu do danych zaobserwowanego w przykładzie 15.3.

#include <stdio.h>
#include <stdlib.h>

// Nagłówek standardu POSIX niezbędny podczas pracy z biblioteką pthread.
#include <pthread.h>

// Obiekt muteksu używany w celu synchronizacji dostępu
// do stanu współdzielonego.
pthread_spinlock_t mtx;

int shared_var = 0;

/*
void* thread_body_1(void* arg) {
  int* shared_var = (int*)arg;
   pthread_spin_lock(&mtx);
  (shared_var)++;
  printf("%d", shared_var);
  pthread_spin_unlock(&mtx);

  return NULL;
}

void* thread_body_2(void* arg) {

  int* shared_var = (int*)arg;
   pthread_spin_lock(&mtx);
  
  shared_var += 2;
  printf("%d", shared_var);
  pthread_spin_unlock(&mtx);

  return NULL;
}
*/
void* thread_body(void* arg) {

  int* ingredient = (int*)arg;
   pthread_spin_lock(&mtx);
  // Sekcja o znaczeniu krytycznym.
  
  shared_var += *ingredient;
  printf("%d", shared_var);
  pthread_spin_unlock(&mtx);

  return NULL;
}

int main(int argc, char** argv) {

  // Zmienna współdzielona.
  int shared_var = 0;

  // Uchwyty wątków.
  pthread_t thread1;
  pthread_t thread2;
  pthread_t thread3;
  printf("----------------------- %s \t\t\t%d\n", __FUNCTION__, shared_var);
  // Inicjalizacja muteksu i związanych z nim zasobów.
  pthread_spin_init(&mtx, NULL);
  
  int one = 1;
  int two = 2;
  int three = 3;

  // Utworzenie nowych wątków.
  int result1 = pthread_create(&thread1, NULL,
          thread_body, &one);
  int result2 = pthread_create(&thread2, NULL,
          thread_body, &two);
  int result3 = pthread_create(&thread3, NULL,
          thread_body, &three);

  if (result1 || result2 || result3) {
    printf("Nie udało się utworzyć wątków.\n");
    exit(1);
  }

  // Oczekiwanie na zakończenie działania wątków.
  result1 = pthread_join(thread1, NULL);
  result2 = pthread_join(thread2, NULL);
  result3 = pthread_join(thread3, NULL);
  
  if (result1 || result2 || result3) {
    printf("Nie udało się dołączyć wątków.\n");
    exit(2);
  }

  pthread_spin_destroy(&mtx);
  
  printf("\n----------------------- %s \t\t\t%d\n", __FUNCTION__, shared_var);

  return 0;
}
