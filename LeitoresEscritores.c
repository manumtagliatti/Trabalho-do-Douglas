#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READERS 5
#define WRITERS 2

pthread_mutex_t resource_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t read_count_mutex = PTHREAD_MUTEX_INITIALIZER;
int read_count = 0;
int shared_data = 0;

void *reader(void *arg) {
    int id = *((int *)arg);
    while (1) {
        pthread_mutex_lock(&read_count_mutex);
        read_count++;
        if (read_count == 1) {
            pthread_mutex_lock(&resource_mutex);
        }
        pthread_mutex_unlock(&read_count_mutex);

        printf("Sensor %d lendo = %d\n", id, shared_data);
        usleep(500000); 

        pthread_mutex_lock(&read_count_mutex);
        read_count--;
        if (read_count == 0) {
            pthread_mutex_unlock(&resource_mutex);
        }
        pthread_mutex_unlock(&read_count_mutex);
        sleep(rand() % 3);
    }
    return NULL;
}

void *writer(void *arg) {
    int id = *((int *)arg);
    while (1) {
        pthread_mutex_lock(&resource_mutex);
        
        shared_data = rand() % 100;
        printf("Escritor %d atualizando dado para %d\n", id, shared_data);

        pthread_mutex_unlock(&resource_mutex);
        sleep(rand() % 5);
    }
    return NULL;
}

int main() {
    pthread_t readers[READERS], writers[WRITERS];
    int ids[READERS + WRITERS];

    for (int i = 0; i < READERS; i++) {
        ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &ids[i]);
    }

    for (int i = 0; i < WRITERS; i++) {
        ids[READERS + i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &ids[READERS + i]);
    }

    for (int i = 0; i < READERS; i++) {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    return 0;
}
