#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> // Include the pthread header file


void* threadC(void* arg) {
    printf("Thread C created\n");

    // Simulate some work for thread C
    sleep(5);

    printf("Thread C completed\n");
    pthread_exit(NULL);
}

void* threadB(void* arg) {
    printf("Thread B created\n");

    pthread_t tid_C;
    pthread_create(&tid_C, NULL, threadC, NULL);
    pthread_join(tid_C, NULL);

    printf("Thread B completed\n");
    pthread_exit(NULL);
}

int main() {
    pthread_t tid_B;

    printf("Thread A created\n");

    pthread_create(&tid_B, NULL, threadB, NULL);
    pthread_join(tid_B, NULL);

    printf("Thread A completed\n");

    return 0;
}
