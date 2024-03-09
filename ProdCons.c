#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t empty, full, mutex;

#include <stdbool.h>

#define NUM_ITERATIONS 10

void printBuffer() {
    printf("Buffer: ");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("%d ", buffer[i]);
    }
    printf("\n");
}

void *producer(void *arg) {
    int item;
    int counter = 0;
    while (counter < NUM_ITERATIONS) {
        item = rand() % 100;
        sem_wait(&empty);                                                                                                                                                                                              
        sem_wait(&mutex);
        buffer[in] = item;
        printBuffer();
        in = (in + 1) % BUFFER_SIZE;
        sem_post(&mutex);
        sem_post(&full);
        counter++;
    }
}

void *consumer(void *arg) {
    int item;
    int counter = 0;
    while (counter < NUM_ITERATIONS) {
        sem_wait(&full);
        sem_wait(&mutex);
        buffer[out] = 0;
        printBuffer();
        out = (out + 1) % BUFFER_SIZE;
        sem_post(&mutex);
        sem_post(&empty);
        counter++;
    }
}

int main() {
    pthread_t producerThread, consumerThread;

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_create(&producerThread, NULL, producer, NULL);
    pthread_create(&consumerThread, NULL, consumer, NULL);

    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}