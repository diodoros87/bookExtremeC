// Nazwa pliku: ExtremeC_examples_chapter15_3_sem.c
// Opis: Ten przykład używa semafora binarnego w celu
//       rozwiązania problemu wyścigu danych w przykładzie 15.3.

#include <stdio.h>
#include <stdlib.h>

// Nagłówek standardu POSIX niezbędny podczas pracy z biblioteką pthread.
#include <pthread.h>

#include <fcntl.h>
#include <sys/stat.h> 

// Semafory nie zostały udostępnione przez plik nagłówkowy pthread.h.
#include <semaphore.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

// Wskaźnik główny prowadzi do obiektu semafora używanego
// w celu synchronizacji dostępu do stanu współdzielonego.
sem_t *semaphore;

int shared_var = 0;

void* thread_body(void* arg) {
  int* igredient = (int*)arg;
  sem_wait(semaphore);
  shared_var += *igredient;
  printf("%d", shared_var);
  sem_post(semaphore);
  
  return NULL;
}
/*
void* thread_body_1(void* arg) {
  // Pobranie wskaźnika prowadzącego do zmiennej współdzielonej.
  int* shared_var = (int*)arg;
  // Oczekiwanie na semafor.
  sem_wait(semaphore);
  // Inkrementacja o 1 wartości zmiennej współdzielonej
  // przez bezpośredni zapis wartości pod adresem w pamięci.
  (*shared_var)++;
  
  printf("%d", *shared_var);
  sem_post(semaphore);
  // Zwolnienie semafora.
  
  return NULL;
}

void* thread_body_2(void* arg) {
  // Pobranie wskaźnika prowadzącego do zmiennej współdzielonej.
  int* shared_var = (int*)arg;
  // Oczekiwanie na semafor.
  sem_wait(semaphore);
  // Inkrementacja o 1 wartości zmiennej współdzielonej
  // przez bezpośredni zapis wartości pod adresem w pamięci.
  (*shared_var) += 2;
  printf("%d", *shared_var);
  // Zwolnienie semafora.
  sem_post(semaphore);
  return NULL;
}
*/
int main(int argc, char** argv) {

  // Zmienna współdzielona.
  

  // Uchwyty wątków.
  pthread_t thread1;
  pthread_t thread2;
  pthread_t thread3;

#ifdef __linux__ // __linux__
  //printf("semaphore = sem_open(sem0, O_CREAT | O_EXCL, 0644, 1) %s \t\t\t%d\n", __FUNCTION__, shared_var);
  // Nienazwane semafory są nieobsługiwane na platformie macOS. Dlatego też
  // trzeba zainicjalizować semafor, podobnie jak w przypadku używania nazwanej
  // funkcji sem_open().
  semaphore = sem_open("sem14", O_CREAT | O_EXCL, 0644, 1);
  if (semaphore != NULL)
     printf("O_CREAT | O_EXCL %s \t\t\t%d\n", __FUNCTION__, shared_var);
  else {
     fprintf(stderr, "sem_open error: %s\n", strerror(errno));
     semaphore = sem_open("sem14", O_EXCL, 0644, 1);
     if (semaphore != NULL)
        printf("O_EXCL %s \t\t\t%d\n", __FUNCTION__, shared_var);
     else
        fprintf(stderr, "sem_open error: %s\n", strerror(errno));
  }
  //semaphore = sem_open("Qwdf56gh77jggt", O_EXCL, 0644, 1);
#else
  printf("sem_t local_semaphore %s \t\t\t%d\n", __FUNCTION__, shared_var);
  sem_t local_semaphore;
  semaphore = &local_semaphore;
  // Inicjalizacja semafora jako muteksu (semafor binarny).
  sem_init(semaphore, 0, 1);
#endif
  if (semaphore == SEM_FAILED)
     fprintf(stderr, "sem_open error: %s\n", strerror(errno));
   assert(semaphore != NULL);
  // Utworzenie nowych wątków.
  int one = 1;
  int result1 = pthread_create(&thread1, NULL,
          thread_body, &one);
  int two = 2;
  int result2 = pthread_create(&thread2, NULL,
          thread_body, &two);
  int three = 3;
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

#ifdef __linux__
  if (sem_close(semaphore))
#else
  if (sem_destroy(semaphore))
#endif
   fprintf(stderr, " sem_close failed: %s", strerror(errno));
  
  printf("\n----------------------- %s \t\t\t%d\n", __FUNCTION__, shared_var);

  return 0;
}
