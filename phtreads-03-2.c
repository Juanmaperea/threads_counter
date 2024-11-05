/*
 * Este programa ilustra el problema de concurrencia que se presenta al momento
 * que dos hilos intentan modificar una variable compartida.
 *
 * Ejemplo tomado del libro 'Operating Systems Three Easy Pieces'.
 *
 * Adaptado por: John Sanabria - john.sanabria@correounivalle.edu.co
 * Fecha: 2023-05-17
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int max;
int num_threads; // number of threads
int *counters; // array of counters for each thread
void *mythread(void *arg) {
  int index = *(int *)arg; // thread index parse to integer (Not letters)
  int i; // stack (private per thread)
  printf("%d: begin [addr of i: %p]\n", index, &i);

  for (i = 0; i < max; i++) {
    counters[index] += 1; // increment the specific counter for the thread
  }

  printf("%d: done\n", index);
  return NULL;
}

int main(int argc, char *argv[]) {

  if (argc != 3) {
    fprintf(stderr, "usage: %s <loopcount> <num_threads>\n", argv[0]);
    exit(1);
  }

  max = atoi(argv[1]);
  num_threads = atoi(argv[2]); 

  // initialize arrays of dinamic way using malloc in C 
  pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
  counters = malloc(num_threads * sizeof(int));

  for (int i = 0; i < num_threads; i++) {
    counters[i] = 0;
    printf("main: begin [counter%d = 0] [%p]\n", i, &counters[i]);
  }
  
  for (int i = 0; i < num_threads; i++) {
    int *arg = malloc(sizeof(*arg));
    *arg = i;
    pthread_create(&threads[i], NULL, mythread, arg);
  }

  // join waits for the threads to finish
  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  int total = 0;
  for (int i = 0; i < num_threads; i++) {
    total += counters[i];
  }

  printf("main: done\n [counter: %d]\n [should: %d]\n", total, max * num_threads);

  // free allocated memory (good practice)
  free(threads);
  free(counters);

  return 0;
}