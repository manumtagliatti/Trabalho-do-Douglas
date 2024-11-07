#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 4
pthread_barrier_t barrier;

void *worker(void *arg) {
    long id = (long)arg;
    printf("Thread %ld atingiu a barreira.\n", id);
    pthread_barrier_wait(&barrier);
    printf("Thread %ld passou pela barreira.\n", id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, worker, (void *)i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);
    return 0;
}