#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int max;
// Se agrega una variable para almacenar el número de hilos:
int num_threads;

void *mythread(void *arg) {
  int counter = 0;
  int i; // stack (private per thread)
  printf("%ld: begin [addr of i: %p]\n", (long)arg, &i);
  for (i = 0; i < max; i++) {
	counter = counter + 1;
  }
  printf("%ld: done\n", (long)arg);
  return (void *)(size_t)counter;
}

int main(int argc, char *argv[]) {
  // Se cambia la restricción del número de argumentos, pues ahora se esperan 3 (El nombre de la función, el parámetro del máximo de conteos y el número de hilos a ejecutar):
  if (argc != 3) {
	fprintf(stderr, "usage: main-first <loopcount>\n");
	exit(1);
  }
  max = atoi(argv[1]);
  // Se asigna el valor del número de hilos:
  num_threads = atoi(argv[2]);
  
  // Se crea un arreglo que almacene los hilos que se van a ejecutar (Antes solo se tenían dos: P1 y P2):
  pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
  
  // Se crea una variable para acumular los resultados de cada hilo:
  int total_counter = 0;
  int thread_counter = 0;

  // Se crean los hilos, de acuerdo a la cantidad ingresada:
  for (long i = 0; i < num_threads; i++) {
    printf("main: begin [thread_counter = %d][%p]\n", thread_counter, &thread_counter);
    pthread_create(&threads[i], NULL, mythread, (void *)i);
  }
  
  // Se espera a la finalización de cada ciclo y se acumulan los resultados:
  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], (void **)&thread_counter);
    total_counter = total_counter + (int)(size_t)thread_counter;
  }

  // Se imprimen los resultados:
  printf("main: done\n [total_counter: %d]\n [should: %d]\n",
	  total_counter, max*num_threads);
  free(threads);
  return 0;
}

