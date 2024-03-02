#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define M 20
#define K 10
#define N 10

int A[M][K];
int B[K][N];
int C[M][N];
int D[M][N];

typedef struct {
    int i;
    int j;
} Arguments;

//function used to randomly assign values to the matrix
void fillMatrix(){
    for(int i = 0; i < M; i++){
        for(int j = 0; j < K; j++){
            A[i][j] = rand() % 100; // random number between 0 and 99
        }
    }
    for(int i = 0; i < K; i++){
        for(int j = 0; j < N; j++){
            B[i][j] = rand() % 100; // random number between 0 and 99
        }
    }
}

//function used to print the matrix to the console
void printMatrix() {
    printf("Matrix A:\n");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            printf("%d ", A[i][j]);// printing the matrix A
        }
        printf("\n");
    }
    printf("Matrix B:\n");
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", B[i][j]);//printing the matrix B
        }
        printf("\n");
    }
}

void* multiply(void *args) {
    Arguments *arg = (Arguments*)args;
    int sum = 0;
    for (int k = 0; k < K; k++) {
        sum += A[arg->i][k] * B[k][arg->j];
    }
    C[arg->i][arg->j] = sum;
    pthread_exit(0);
}

typedef struct {
    int this_thread_index;
    int total_threads;
} _Arguments;

void* _multiply(void *args) {
    _Arguments *arg = (_Arguments*)args;
    int sum = 0;
    for (int i = arg->this_thread_index; i < M; i += arg->total_threads) {
        for (int j = 0; j < N; j++) {
            sum = 0;
            for (int k = 0; k < K; k++) {
                sum += A[i][k] * B[k][j];
            }
            D[i][j] = sum;
        }
    }
    pthread_exit(0);
}

int main() {
    fillMatrix();

    printMatrix();
    
    // Using multiple threads
    int MAX_THREADS = M*N;
    int num_threads = 1;
    double time_taken[MAX_THREADS];
    struct timeval before_time_tv;
    gettimeofday(&before_time_tv, NULL); // to calculate the time taken
    while(num_threads <= MAX_THREADS)
    {
        struct timeval start_time_tv, end_time_tv, present_time_tv;
        gettimeofday(&start_time_tv, NULL);
        pthread_t _threads[num_threads];
        _Arguments _args[num_threads];
        for (int i = 0; i < num_threads; i++) {
            _args[i].this_thread_index = i;
            _args[i].total_threads = num_threads;
            pthread_create(&_threads[i], NULL, _multiply, &_args[i]);
        }
        for (int i = 0; i < num_threads; i++) {
            pthread_join(_threads[i], NULL);
        }
        gettimeofday(&end_time_tv, NULL);
        time_taken[num_threads - 1] = (double)(end_time_tv.tv_usec - start_time_tv.tv_usec) / 1e6 + (end_time_tv.tv_sec - start_time_tv.tv_sec);
        num_threads++;
        // gettimeofday(&present_time_tv, NULL);
        // double present_time = (double)(present_time_tv.tv_usec - before_time_tv.tv_usec) / 1e6 + (present_time_tv.tv_sec - before_time_tv.tv_sec);
        // printf("\r[%s] %d%%; %fs", progress_bar, progress, present_time);
        fflush(stdout);
    }
    // Find the best number of threads (finding the minimum time taken for the multiplication of the matrices using different number of threads)
    int best_threads = 1;
    double best_time = time_taken[0];
    for (int i = 1; i < MAX_THREADS; i++) {
        if (time_taken[i] < time_taken[best_threads - 1]) {
            best_threads = i + 1;
            best_time = time_taken[i];
        }
    }
    // code column for printing out the matrices
    // printf("Product of the matrices:\n");
    // for (int i = 0; i < M; i++) {
    //     for (int j = 0; j < N; j++) {
    //         printf("%d ", D[i][j]);
    //     }
    //     printf("\n");
    // }

    printf("\nThe number of threads which give the best time effeciency: %d\n", best_threads);
    printf("Time taken for %d threads: %fs\n", best_threads, best_time);
    return 0;
}