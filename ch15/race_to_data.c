#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <stdatomic.h>
/*
void* display(const int* const arg) {
  printf("%d", *arg);
  return NULL;
}
*/
void* thread_body_1(void* arg) {
  _Atomic volatile int* const  shared_var_ptr = (_Atomic volatile int* const )arg;
  (*shared_var_ptr)++;
  printf("%d", *shared_var_ptr);
  return NULL;
}

void* thread_body_2(void* arg) {
  _Atomic volatile int* const  shared_var_ptr = (_Atomic volatile int* const )arg;
  *shared_var_ptr += 2;
  printf("%d", *shared_var_ptr);
  return NULL;
}

int main(void) {
  _Atomic volatile int shared_var = 0;

  pthread_t thread1;
  pthread_t thread2;
   printf("----------------------- %s \t\t\t%d\n", __FUNCTION__, shared_var);

  int result1 = pthread_create(&thread1, NULL,
          thread_body_1, &shared_var);
  int result2 = pthread_create(&thread2, NULL,
          thread_body_2, &shared_var);

  if (result1 || result2) {
    fprintf(stderr, " pthread_create: result1 = %d || result2 = %d\n", result1, result2);
    exit(1);
  }

  result1 = pthread_join(thread1, NULL);
  result2 = pthread_join(thread2, NULL);
  //result1 = pthread_detach(thread1);
  //result2 = pthread_detach(thread2);

  if (result1 || result2) {
    fprintf(stderr, "pthread_join: result1 = %d || result2 = %d\n", result1, result2);
    exit(2);
  }
  printf("\n----------------------- %s \t\t\t%d\n", __FUNCTION__, shared_var);
  pthread_exit(NULL);
  return 0;
}
