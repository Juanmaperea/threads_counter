#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int max;

void *mythread(void *arg) {
  char *letter = arg;
  int counter = 0;
  int i; // stack (private per thread)
  printf("%s: begin [addr of i: %p]\n", letter, &i);
  for (i = 0; i < max; i++) {
	counter = counter + 1; // shared: only one
  }
  printf("%s: done\n", letter);
  return (void *)(size_t)counter;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
	fprintf(stderr, "usage: main-first <loopcount>\n");
	exit(1);
  }
  max = atoi(argv[1]);

  pthread_t p1, p2;
  int counter1 = 0;
  int counter2 = 0;
  printf("main: begin [counter1 = %d][%p] [counter2 = %d][%p]\n", counter1, &counter1, counter2, &counter2);
  pthread_create(&p1, NULL, mythread, "A");
  pthread_create(&p2, NULL, mythread, "B");
  // join waits for the threads to finish
  pthread_join(p1, (void **)&counter1);
  pthread_join(p2, (void **)&counter2);
  int total_counter = counter1 + counter2;
  printf("main: done\n [total_counter: %d]\n [should: %d]\n",
	  total_counter, max*2);
  return 0;
}

