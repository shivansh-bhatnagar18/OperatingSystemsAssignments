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

void fillMatrix(){
    for(int i = 0; i < M; i++){
        for(int j = 0; j < K; j++){
            A[i][j] = rand() % 100;
        }
    }
    for(int i = 0; i < K; i++){
        for(int j = 0; j < N; j++){
            B[i][j] = rand() % 100;
        }
    }
}

void printMatrix() {
    printf("Matrix A:\n");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
    printf("Matrix B:\n");
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }
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
    int MAX_THREADS = M*N;
    int num_threads = 1;
    double time_taken[MAX_THREADS];
    struct timeval before_time_tv;
    gettimeofday(&before_time_tv, NULL);
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
        fflush(stdout);
    }
    int best_threads = 1;
    double best_time = time_taken[0];
    for (int i = 1; i < MAX_THREADS; i++) {
        if (time_taken[i] < time_taken[best_threads - 1]) {
            best_threads = i + 1;
            best_time = time_taken[i];
        }
    }
    printf("Time taken matrix:\n");
    for (int i = 0; i < MAX_THREADS; i++) {
        printf("Thread %d: %fs\n", i + 1, time_taken[i]);
    }
    printf("\nThe number of threads which give the best time efficiency: %d\n", best_threads);
    printf("Time taken for %d threads: %fs\n", best_threads, best_time);
    return 0;
}




