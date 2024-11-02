#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int max;

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
    if (argc != 2) {
        fprintf(stderr, "usage: %s <loopcount>\n", argv[0]);
        exit(1);
    }
    max = atoi(argv[1]);

    pthread_t p1, p2;
    thread_data data1 = {1, 0}, data2 = {2, 0};

    printf("main: begin\n");
    pthread_create(&p1, NULL, mythread, &data1);
    pthread_create(&p2, NULL, mythread, &data2);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    int total_counter = data1.local_counter + data2.local_counter;
    printf("main: done\n [total counter: %d]\n [should: %d]\n", total_counter, max * 2);
    return 0;
}
