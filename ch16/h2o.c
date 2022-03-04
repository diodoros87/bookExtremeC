// Nazwa pliku: ExtremeC_examples_chapter16_2.c
// Opis: Klasyczny przykład pokazujący, jak semafor ogólny
//              może zostać użyty do utworzenia cząsteczek
//              wody z użyciem wątków przedstawiających atomy
//              tlenu i wodoru. Każdy wątek tlenu powinien
//              czekać na dwa wątki wodoru, aby w ten sposób
//              powstała cząsteczka wody.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h> // Plik nagłówkowy wymagany do działania funkcji errno() i strerror().

#include <fcntl.h>

#include "../print.h"

// Nagłówek standardu POSIX niezbędny podczas pracy z biblioteką pthread.
#include <pthread.h>
// Semafory nie zostały udostępnione przez plik nagłówkowy pthread.h.
#include <semaphore.h>

#ifdef __linux__ //__APPLE__
// Na platformie Apple konieczne jest symulowanie funkcjonalności barier.
pthread_mutex_t barrier_mutex;
pthread_cond_t  barrier_cv;
unsigned int    barrier_thread_count;
unsigned int    barrier_round;
unsigned int    barrier_thread_limit;

void barrier_wait() {
  pthread_mutex_lock(&barrier_mutex);
  barrier_thread_count++;
  if (barrier_thread_count >= barrier_thread_limit) {
    barrier_thread_count = 0;
    barrier_round++;
    pthread_cond_broadcast(&barrier_cv);
  } else {
    unsigned int my_round = barrier_round;
    do {
      pthread_cond_wait(&barrier_cv, &barrier_mutex);
    } while (my_round == barrier_round);
  }
  pthread_mutex_unlock(&barrier_mutex);
}

#else
// Bariera pozwalająca na synchronizację wątków przedstawiających wodór i tlen.
pthread_barrier_t water_barrier;
#endif

// Muteks pozwalający na synchronizację wątków przedstawiających tlen.
pthread_mutex_t   oxygen_mutex;

// Ogólny semafor pozwalający na synchronizację wątków przedstawiających wodór.
sem_t*            hydrogen_sem;

// Zmienna współdzielona w postaci liczby całkowitej przeznaczonej do zliczania cząsteczek wody.
unsigned int      num_of_water_molecules;

void* hydrogen_thread_body(void* arg) {
  // Dwa wątki wodoru mogą wejść do sekcji o znaczeniu krytycznym.
  sem_wait(hydrogen_sem);
  // Oczekiwanie na dołączenie pozostałych wątków wodoru.
#ifdef __linux__ //__APPLE__
  barrier_wait();
#else
  pthread_barrier_wait(&water_barrier);
#endif
  LOG("\n%s\n", __FUNCTION__);
  sem_post(hydrogen_sem);
  return NULL;
}

void* oxygen_thread_body(void* arg) {
  pthread_mutex_lock(&oxygen_mutex);
  // Oczekiwanie na dołączenie wątków wodoru.
#ifdef __linux__ //__APPLE__
  barrier_wait();
#else
  pthread_barrier_wait(&water_barrier);
#endif
  num_of_water_molecules++;
  LOG("\n%s %u\n", __FUNCTION__, num_of_water_molecules);
  pthread_mutex_unlock(&oxygen_mutex);
  return NULL;
}

void semaphore_init(sem_t** semaphore, const char * sem_name_0, const char * sem_name_1, unsigned int value) {
#ifdef __linux__ //__APPLE__
   *semaphore = sem_open(sem_name_0, O_CREAT | O_EXCL, 0644, value);
   if (*semaphore != NULL)
      printf("O_CREAT | O_EXCL %s \t\t\t%p\n", __FUNCTION__, *semaphore);
   else {
      fprintf(stderr, "sem_open error: %s\n", strerror(errno));
      if (*semaphore = sem_open(sem_name_1, O_EXCL, 0644, value))
         printf("O_EXCL %s \t\t\t%p\n", __FUNCTION__, *semaphore);
      else
         fprintf(stderr, "sem_open error: %s\n", strerror(errno));
   }
   //semaphore = sem_open("Qwdf56gh77jggt", O_EXCL, 0644, 1);
#else
   printf("sem_t local_semaphore %s \t\t\t%p\n", __FUNCTION__, *semaphore);
   static sem_t local_semaphore;
   *semaphore = &local_semaphore;
   // Inicjalizacja semafora jako muteksu (semafor binarny).
   sem_init(*semaphore, 0, value);
#endif
}

int main(int argc, char** argv) {
  num_of_water_molecules = 0;

  pthread_mutex_init(&oxygen_mutex, NULL);

   semaphore_init(&hydrogen_sem, "hydrogen_1", "hydrogen_0", 2);

#ifdef __linux__ //__APPLE__
  pthread_mutex_init(&barrier_mutex, NULL);
  pthread_cond_init(&barrier_cv, NULL);
  barrier_thread_count = 0;
  barrier_thread_limit = 3;
  barrier_round = 0;
#else
  pthread_barrier_init(&water_barrier, NULL, 3);
#endif

  // Do utworzenia 50 cząsteczek wody potrzeba 50 atomów tlenu
  // i 100 atomów wodoru.
  pthread_t thread[150];

  // Utworzenie wątków przedstawiających tlen.
  for (int i = 0; i < 50; i++) {
    if (pthread_create(thread + i, NULL,
                oxygen_thread_body, NULL)) {
      printf("Nie można utworzyć wątku tlenu.\n");
      exit(1);
    }
  }

  // Utworzenie wątków przedstawiających wodór.
  for (int i = 50; i < 150; i++) {
    if (pthread_create(thread + i, NULL,
                hydrogen_thread_body, NULL)) {
      printf("Nie można utworzyć wątku wodoru.\n");
      exit(2);
    }
  }

  printf("Oczekiwanie na reakcję atomów wodoru i tlenu ...\n");
  // Oczekiwanie na zakończenie działania wszystkich wątków.
  for (int i = 0; i < 150; i++) {
    if (pthread_join(thread[i], NULL)) {
      printf("Nie udało się dołączyć wątku.\n");
      exit(3);
    }
  }

  printf("Liczba utworzonych cząsteczek wody: %d\n",
          num_of_water_molecules);

#ifdef __linux__ //__APPLE__
  sem_close(hydrogen_sem);
#else
  sem_destroy(hydrogen_sem);
#endif

  return 0;
}
