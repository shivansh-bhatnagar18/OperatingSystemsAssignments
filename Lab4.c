#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
  
void *fibonacciElement(void *arg) 
{ 
    int n = *((int *) arg);
    int *result = malloc(sizeof(int));
    if (n == 0) {
        *result = 0;
        return result;
    } else if (n == 1) {
        *result = 1;
        return result;
    } else {
        int a = 0;
        int b = 1;
        int c;
        for (int i = 2; i <= n; i++) {
            c = a + b;
            a = b;
            b = c;
        }
        *result = c;
    }
    return result;
}
  
int main() 
{ 
    int n;
    printf("Enter the number of elements: ");
    scanf("%d", &n);
    pthread_t threads[n];
    int results[n];
    for (int i = 0; i < n; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&threads[i], NULL, fibonacciElement, arg);
    }
    for (int i = 0; i < n; i++) {
        int *result;
        // pthread_join(threads[i], (void **) &result);
        results[i] = *result;
        // free(result);
        printf("Fibonacci element %d: %d\n", i, results[i]);
    }

    return 0; 
} 