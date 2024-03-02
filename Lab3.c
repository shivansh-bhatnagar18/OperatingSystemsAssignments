#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 80
// #define BUFFER_SIZE 50
#define HISTORY_SIZE 10

// char buffer [BUFFER_SIZE];
char history[HISTORY_SIZE][MAX_LINE];
int history_count = 0;

void add_to_history(const char *command) {
    if (history_count < HISTORY_SIZE) {
        strcpy(history[history_count], command);
        history_count++;
    } else {
        for (int i = 0; i < HISTORY_SIZE - 1; i++) {
            strcpy(history[i], history[i + 1]);
        }
        strcpy(history[HISTORY_SIZE - 1], command);
    }
}

void change_directory(char *args[]) {
    if (args[1] == NULL) {
        fprintf(stderr, "cd: missing argument\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("cd failed");
        }
    }
}

void print_history() {
    printf("Command History:\n");
    for (int i = 0; i < history_count; i++) {
        printf("%d. %s\n", i + 1, history[i]);
    }
}

void setup (char inputBuffer[], char *args[], int *background) {
    int length,
        i,
        start,
        ct;

    ct = 0;

    length = read(STDIN_FILENO, inputBuffer, MAX_LINE);

    start = -1;

    add_to_history(inputBuffer);

    if (length == 0) {
        exit(0);
    }

    if (length < 0) {
        perror("Error reading the command");
        exit(-1);
    }

    for (i = 0; i < length; i++) {
        switch (inputBuffer[i]) {
            case ' ':
            case '\t':
                if (start != -1) {
                    args[ct] = &inputBuffer[start];
                    ct++;
                }
                inputBuffer[i] = '\0';
                start = -1;
                break;
            case '\n':
                if (start != -1) {
                    args[ct] = &inputBuffer[start];
                    ct++;
                }
                inputBuffer[i] = '\0';
                args[ct] = NULL;
                break;
            default:
                if (start == -1) {
                    start = i;
                }
                if (inputBuffer[i] == '&') {
                    *background = 1;
                    inputBuffer[i - 1] = '\0';
                }
        }
    }

    args[ct] = NULL;
}

int main (void) {
    char inputBuffer[MAX_LINE];
    int background;
    char *args[MAX_LINE / 2 + 1];
    pid_t pid;

    while (1) {
        background = 0;

        printf("COMMAND-> ");
        fflush(stdout);

        setup(inputBuffer, args, &background);

        if (strcmp(args[0], "history") == 0) {
            print_history();
            continue;
        } else if (strcmp(args[0], "cd") == 0) {
            change_directory(args);
            continue;
        } else if (strcmp(args[0], "exit") == 0) {
            exit(0); 
        } else {

        pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) {
            execvp(args[0], args);
        } else {
            if (background == 0) {
                wait(NULL);
            }
        }
        }
    }

    return 0;
}