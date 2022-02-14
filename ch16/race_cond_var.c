// Nazwa pliku: ExtremeC_examples_chapter16_1_cv.c
// Opis: Ten przykład pokazuje użycie zmiennej warunkowej
//              w oczekiwaniu na wystąpienie zdarzenia.

#include <stdio.h>
#include <stdlib.h>

// Nagłówek standardu POSIX niezbędny podczas pracy z biblioteką pthread.
#include <pthread.h>

#include "../print.h"

#define TRUE  1
#define FALSE 0

typedef unsigned int bool_t;

// Struktura pozwalająca na przechowywanie wszystkich zmiennych
// związanych ze stanem współdzielonym.
typedef struct {
  // Opcja wskazująca, czy znak 'A' został wyświetlony.
  bool_t          done;
  // Obiekt muteksu chroniący sekcje o znaczeniu krytycznym.
  pthread_mutex_t mtx;
  // Zmienna warunkowa używana do synchronizacji dwóch wątków.
  pthread_cond_t  cv;
} shared_state_t;

// Inicjalizacja elementów składowych obiektu shared_state_t.
void shared_state_init(shared_state_t *shared_state) {
  shared_state->done = FALSE;
  pthread_mutex_init(&shared_state->mtx, NULL);
  pthread_cond_init(&shared_state->cv, NULL);
}

// Usunięcie elementów składowych obiektu shared_state_t.
void shared_state_destroy(shared_state_t *shared_state) {
  pthread_mutex_destroy(&shared_state->mtx);
  pthread_cond_destroy(&shared_state->cv);
}

void* thread_body_1(void* arg) {
  shared_state_t* ss = (shared_state_t*)arg;
  pthread_mutex_lock(&ss->mtx);
  LOG("%s", "A");
  ss->done = TRUE;
  // Sygnalizowanie wątków oczekujących na zmienną warunkową.
  pthread_cond_signal(&ss->cv);
  pthread_mutex_unlock(&ss->mtx);
  
  return NULL;
}

void* thread_body_2(void* arg) {
  shared_state_t* ss = (shared_state_t*)arg;
  pthread_mutex_lock(&ss->mtx);
  // Oczekiwanie, aż opcja przyjmie wartość TRUE.
  if (!ss->done) {
    // Oczekiwanie na zmienną warunkową.
   pthread_cond_wait(&ss->cv, &ss->mtx);
   //pthread_cond_signal(&ss->cv);
    //pthread_cond_broadcast(&ss->cv);
  }
  
  pthread_mutex_unlock(&ss->mtx);
  LOG("%s", "B");
  return NULL;
}

int main(int argc, char** argv) {

  // Stan współdzielony.
  shared_state_t shared_state;

  // Inicjalizacja stanu współdzielonego.
  shared_state_init(&shared_state);
  
  LOG("\n----------------------- %s \t\t\t%d\n", __FUNCTION__, shared_state.done);
//printf("\n----------------------- %s \t\t\t%d\n", __FUNCTION__, shared_state.done);
  // Uchwyty wątków.
  pthread_t thread1;
  pthread_t thread2;

  // Utworzenie nowych wątków.
  int result1 =
    pthread_create(&thread1, NULL, thread_body_1, &shared_state);
  int result2 =
    pthread_create(&thread2, NULL, thread_body_2, &shared_state);

  if (result1 || result2) {
    printf("Nie udało się utworzyć wątków.\n");
    exit(1);
  }
  //pthread_cond_broadcast(&shared_state.cv);
//printf("\n----------------------- %s \t\t\t%d\n", __FUNCTION__, shared_state.done);
  // Oczekiwanie na zakończenie działania wątków.
  result1 = pthread_join(thread1, NULL);
  result2 = pthread_join(thread2, NULL);

  if (result1 || result2) {
    printf("Nie udało się dołączyć wątków.\n");
    exit(2);
  }

  // Usunięcie stanu współdzielonego oraz zwolnienie obiektu muteksu
  // i zmiennej warunkowej.
  shared_state_destroy(&shared_state);
  
  LOG("\n----------------------- %s \t\t\t%d\n", __FUNCTION__, shared_state.done);

  return 0;
}
