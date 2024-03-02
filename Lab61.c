#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 100

// Structure to hold the arguments for merge sort
struct MergeSortArgs {
    int* arr;
    int left;
    int right;
};

// Function to merge two sorted subarrays
void merge(int arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    int L[n1], R[n2];

    // Copy data to temporary arrays
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr[left..right]
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = left; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Function to perform merge sort on a given array
void* mergeSort(void* arg) {
    struct MergeSortArgs* args = (struct MergeSortArgs*)arg;
    int left = args->left;
    int right = args->right;
    int* arr = args->arr;

    if (left < right) {
        int mid = left + (right - left) / 2;

        // Create arguments for the first half
        struct MergeSortArgs args1;
        args1.arr = arr;
        args1.left = left;
        args1.right = mid;

        // Create arguments for the second half
        struct MergeSortArgs args2;
        args2.arr = arr;
        args2.left = mid + 1;
        args2.right = right;

        // Create threads for the two halves
        pthread_t thread1, thread2;
        pthread_create(&thread1, NULL, mergeSort, &args1);
        pthread_create(&thread2, NULL, mergeSort, &args2);

        // Wait for the threads to finish
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);

        // Merge the two sorted halves
        merge(arr, left, mid, right);
    }

    pthread_exit(NULL);
}

int main() {
    int n;
    int arr[MAX_SIZE];

    printf("Enter the number of elements: ");
    scanf("%d", &n);

    printf("Enter the elements:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    // Create arguments for the merge sort
    struct MergeSortArgs args;
    args.arr = arr;
    args.left = 0;
    args.right = n - 1;

    // Create a thread for the merge sort
    pthread_t thread;
    pthread_create(&thread, NULL, mergeSort, &args);

    // Wait for the thread to finish
    pthread_join(thread, NULL);

    printf("Sorted array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
