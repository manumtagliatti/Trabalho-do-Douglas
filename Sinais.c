#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // Adicionada para a função sleep

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int ready = 0;

void *waiting_thread(void *arg) {
    pthread_mutex_lock(&lock);
    while (!ready) {
        printf("Thread: esperando por sinal.\n");
        pthread_cond_wait(&cond, &lock);
    }
    printf("Thread: sinal recebido e continuando\n");
    pthread_mutex_unlock(&lock);
    return NULL;
}

void *signaling_thread(void *arg) {
    sleep(1);
    pthread_mutex_lock(&lock);
    ready = 1;
    printf("Thread: enviando sinal.\n");
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t wait_thread, signal_thread;

    pthread_create(&wait_thread, NULL, waiting_thread, NULL);
    pthread_create(&signal_thread, NULL, signaling_thread, NULL);

    pthread_join(wait_thread, NULL);
    pthread_join(signal_thread, NULL);

    return 0;
}