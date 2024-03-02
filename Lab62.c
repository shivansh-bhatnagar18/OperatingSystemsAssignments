#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int* arr;
    int low;
    int high;
} ThreadArgs;

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void* quickSortThread(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int* arr = args->arr;
    int low = args->low;
    int high = args->high;

    if (low < high) {
        int pi = partition(arr, low, high);

        ThreadArgs args1 = { arr, low, pi - 1 };
        ThreadArgs args2 = { arr, pi + 1, high };

        pthread_t thread1, thread2;
        pthread_create(&thread1, NULL, quickSortThread, (void*)&args1);
        pthread_create(&thread2, NULL, quickSortThread, (void*)&args2);

        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
    }

    pthread_exit(NULL);
}

void quickSort(int arr[], int low, int high) {
    ThreadArgs args = { arr, low, high };
    quickSortThread((void*)&args);
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int n;
    printf("Enter the number of elements in the array: ");
    scanf("%d", &n);

    int* arr = (int*)malloc(n * sizeof(int));
    printf("Enter the elements of the array: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Original array: ");
    printArray(arr, n);

    quickSort(arr, 0, n - 1);

    printf("Sorted array: ");
    printArray(arr, n);

    free(arr);

    return 0;
}
