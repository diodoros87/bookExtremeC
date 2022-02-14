// Nazwa pliku: ExtremeC_examples_chapter15_2_1.c
// Opis: Ten kod jest taki sam, jak w przykładzie 
//              15.2, choć w tym przypadku mamy pewne
//              pozostawione wskaźniki, które są przekazywane
//              do wątków zamiast wskaźników prowadzących
//              do literów ciągów tekstowych.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  char str1[7], str2[7], str3[6];
  strcpy(str1, "apple");
  strcpy(str2, "orange");
  strcpy(str3, "lemon");

  // Utworzenie nowych wątków.
  int result1 = pthread_create(&thread1, NULL, thread_body, str1);
  int result2 = pthread_create(&thread2, NULL, thread_body, str2);
  int result3 = pthread_create(&thread3, NULL, thread_body, str3);

  if (result1 || result2 || result3) {
    printf("Nie udało się utworzyć wątków.\n");
    exit(1);
  }
   /*
   result1 = pthread_join(thread1, NULL);
  result2 = pthread_join(thread2, NULL);
  result3 = pthread_join(thread3, NULL);*/
  // Odłączenie wątków.
  
  result1 = pthread_detach(thread1);
  result2 = pthread_detach(thread2);
  result3 = pthread_detach(thread3);

  if (result1 || result2 || result3) {
    printf("Wątki nie mogły zostać odłączone.\n");
    exit(2);
  }
printf("\n");
  // W tym momencie następuje zwolnienie pamięci zarezerwowanej dla ciągów tekstowych.
  pthread_exit(NULL);
//printf("\n");
  return 0;
}
