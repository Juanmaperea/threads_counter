#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int max, num_threads;

typedef struct {
    int id;
    int local_counter;
} thread_data;

void *mythread(void *arg) {
    thread_data *data = (thread_data *)arg;
    int i;
    printf("Thread %d: begin [addr of i: %p]\n", data->id, &i);
    for (i = 0; i < max; i++) {
        data->local_counter++;
    }
    printf("Thread %d: done\n", data->id);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <loopcount> <num_threads>\n", argv[0]);
        exit(1);
    }
    max = atoi(argv[1]);
    num_threads = atoi(argv[2]);

    pthread_t threads[num_threads];
    thread_data thread_args[num_threads];

    printf("main: begin\n");
    for (int i = 0; i < num_threads; i++) {
        thread_args[i].id = i;
        thread_args[i].local_counter = 0;
        pthread_create(&threads[i], NULL, mythread, &thread_args[i]);
    }

    int total_counter = 0;
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        total_counter += thread_args[i].local_counter;
    }

    printf("main: done\n [total counter: %d]\n [should: %d]\n", total_counter, max * num_threads);
    return 0;
}
