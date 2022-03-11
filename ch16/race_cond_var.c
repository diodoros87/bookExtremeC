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

//typedef unsigned int bool_t;

// Struktura pozwalająca na przechowywanie wszystkich zmiennych
// związanych ze stanem współdzielonym.
typedef struct {
  // Opcja wskazująca, czy znak 'A' został wyświetlony.
  char actual_print;
  // Obiekt muteksu chroniący sekcje o znaczeniu krytycznym.
  pthread_mutex_t mtx;
  // Zmienna warunkowa używana do synchronizacji dwóch wątków.
  pthread_cond_t  cv;
} shared_state_t;

typedef struct {
  // Opcja wskazująca, czy znak 'A' został wyświetlony.
  const shared_state_t * const shared;
  char print;
  char next_print;
} state_t;

// Inicjalizacja elementów składowych obiektu shared_state_t.
void shared_state_init(shared_state_t *shared_state, const char actual_print) {
  shared_state->actual_print = actual_print;
  pthread_mutex_init(&shared_state->mtx, NULL);
  pthread_cond_init(&shared_state->cv, NULL);
}

// Usunięcie elementów składowych obiektu shared_state_t.
void shared_state_destroy(shared_state_t *shared_state) {
  pthread_mutex_destroy(&shared_state->mtx);
  pthread_cond_destroy(&shared_state->cv);
}

void* thread_body(void* arg) {
  state_t * state = (state_t*)arg;
  shared_state_t* ss = (shared_state_t*)state->shared;
  pthread_mutex_lock(&ss->mtx);
  while (ss->actual_print != state->print) {
   pthread_cond_wait(&ss->cv, &ss->mtx);
   //pthread_cond_signal(&ss->cv);
    //pthread_cond_broadcast(&ss->cv);
  }
  LOG("%c", ss->actual_print);
  ss->actual_print = state->next_print;
  // Sygnalizowanie wątków oczekujących na zmienną warunkową.
  pthread_cond_broadcast(&ss->cv);
  pthread_mutex_unlock(&ss->mtx);
  
  return NULL;
}
/*
void* thread_body_1(void* arg) {
  shared_state_t* ss = (shared_state_t*)arg;
  pthread_mutex_lock(&ss->mtx);
  LOG("%s", "A");
  ss->done = TRUE;
  pthread_cond_signal(&ss->cv);
  pthread_mutex_unlock(&ss->mtx);
  
  return NULL;
}

void* thread_body_2(void* arg) {
  shared_state_t* ss = (shared_state_t*)arg;
  pthread_mutex_lock(&ss->mtx);
  if (!ss->done) {
   pthread_cond_wait(&ss->cv, &ss->mtx);
   //pthread_cond_signal(&ss->cv);
    //pthread_cond_broadcast(&ss->cv);
  }
  pthread_mutex_unlock(&ss->mtx);
  LOG("%s", "B");
  return NULL;
}
*/
int main(int argc, char** argv) {

  // Stan współdzielony.
   shared_state_t ss;
   shared_state_init(&ss, 'A');
  state_t state [3] = { { &ss, 'C', 'C' }, { &ss, 'B', 'C' }, { &ss, 'A', 'B' } };
  /*
  shared_state_init(&shared_state[0], 'A', 'A', 'B');
  shared_state_init(&shared_state[1], 'A', 'B', 'C');
  shared_state_init(&shared_state[2], 'A', 'C', 'C');*/
  
  LOG("\n----------------------- %s \t\t\t%d\n", __FUNCTION__, state->shared->actual_print);
//printf("\n----------------------- %s \t\t\t%d\n", __FUNCTION__, shared_state.done);
  // Uchwyty wątków.
  pthread_t thread [3];
  
  for(int i = 0; i < 3; i++)
      if (pthread_create(thread + i, NULL, thread_body, state + i)) {
         printf("Nie udało się utworzyć wątków.\n");
         exit(1);
      }

  //pthread_cond_broadcast(&shared_state.cv);
//printf("\n----------------------- %s \t\t\t%d\n", __FUNCTION__, shared_state.done);
  // Oczekiwanie na zakończenie działania wątków.
  
  for(int i = 0; i < 3; i++)
      if (pthread_join(thread[i], NULL)) {
         printf("Nie udało się dołączyć wątków.\n");
         exit(2);
      }

  // Usunięcie stanu współdzielonego oraz zwolnienie obiektu muteksu
  // i zmiennej warunkowej.
  //for(int i = 0; i < 3; i++)
   shared_state_destroy(&ss);
  
  LOG("\n----------------------- %s \t\t\t%d\n", __FUNCTION__, state->shared->actual_print);

  return 0;
}
