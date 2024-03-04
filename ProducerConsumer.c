#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
// Producer Consumer Problem is a classic example of a multi-process synchronization problem. The problem describes two processes, the producer and the consumer, who share a common, fixed-size buffer used as a queue. The producer’s job is to generate data, put it into the buffer, and start again. At the same time, the consumer is consuming the data (i.e. removing it from the buffer), one piece at a time. The problem is to make sure that the producer won’t try to add data into the buffer if it’s full and that the consumer won’t try to remove data from an empty buffer.
#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t empty, full, mutex;

void *producer(void *arg) {
    int item;
    while (1) {
        item = rand() % 100; // Generate a random item
        sem_wait(&empty); // Wait for an empty slot in the buffer
        sem_wait(&mutex); // Acquire the mutex to protect the buffer
        buffer[in] = item; // Add the item to the buffer
        printf("Producer produced item: %d\n", item);
        in = (in + 1) % BUFFER_SIZE; // Update the in pointer
        sem_post(&mutex); // Release the mutex
        sem_post(&full); // Signal that the buffer is full
    }
}

void *consumer(void *arg) {
    int item;
    while (1) {
        sem_wait(&full); // Wait for a full slot in the buffer
        sem_wait(&mutex); // Acquire the mutex to protect the buffer
        item = buffer[out]; // Remove an item from the buffer
        printf("Consumer consumed item: %d\n", item);
        out = (out + 1) % BUFFER_SIZE; // Update the out pointer
        sem_post(&mutex); // Release the mutex
        sem_post(&empty); // Signal that the buffer is empty
    }
}

int main() {
    pthread_t producerThread, consumerThread;

    // Initialize the semaphores
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    // Create the producer and consumer threads
    pthread_create(&producerThread, NULL, producer, NULL);
    pthread_create(&consumerThread, NULL, consumer, NULL);

    // Wait for the threads to finish
    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);

    // Destroy the semaphores
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}