#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> // Add the missing header file

int main() {
    pid_t pid_A, pid_B, pid_C; // Declare the variables before using them

    pid_A = getpid();
    printf("Process A (PID: %d) created\n", pid_A);

    pid_B = fork();

    if (pid_B < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid_B == 0) {
        // Child process B
        pid_B = getpid();
        printf("Process B (PID: %d) created with parent PID: %d\n", pid_B, getppid());

        pid_C = fork();

        if (pid_C < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid_C == 0) {
            // Child process C
            pid_C = getpid();
            printf("Process C (PID: %d) created with parent PID: %d\n", pid_C, getppid());

            // Simulate some work for process C
            sleep(5);
            printf("Process C (PID: %d) completed\n", pid_C);
        } else {
            // Process B continues and exits immediately after forking C
            exit(EXIT_SUCCESS);
        }
    } else {
        // Process A continues and waits for process B
        wait(NULL);
        printf("Process B (PID: %d) completed\n", pid_B);
    }

    return 0;
}
