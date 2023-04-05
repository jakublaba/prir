#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "matrix.h"

matrix_t* A;
matrix_t* B;
matrix_t* RESULT;
double SQ_SUM;
double FROB;

int main(int argc, char** argv) {
    if (argc < 4) {
        fprintf(stderr, "Run with 2 files containing matrix data and number of threads\n");
        return EXIT_FAILURE;
    }

    A = load_file(argv[1]);
    B = load_file(argv[2]);
    int n_threads = atoi(argv[3]);

    print_matrix(A);
    printf("\n");
    print_matrix(B);
    printf("\n");

    multiply_parallel(n_threads);

    print_matrix(RESULT);

    double frob = frobenius_norm(n_threads);
    printf("Frobenius norm of result %lf\n", frob);

    free_matrix(A);
    free_matrix(B);
    free_matrix(RESULT);

    return EXIT_SUCCESS;
}

double** alloc_2d_arr(int rows, int cols) {
    double** arr = calloc(rows, sizeof(double*));

    for (int i = 0; i < rows; ++i) {
        arr[i] = calloc(cols, sizeof(double));
    }

    return arr;
}

void free_2d_arr(double** arr, int rows) {
    for (int i = 0; i < rows; ++i) free(arr[i]);
    free(arr);
}

matrix_t* load_file(char* fname) {
    FILE* file = fopen(fname, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open %s\n", fname);
        exit(EXIT_FAILURE);
    }

    int rows, cols;
    fscanf(file, "%d", &rows);
    fscanf(file, "%d", &cols);

    double** data = alloc_2d_arr(rows, cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            fscanf(file, "%lf", &data[i][j]);
        }
    }

    matrix_t* matrix = malloc(sizeof(matrix_t));
    matrix->data = data;
    matrix->rows = rows;
    matrix->cols = cols;

    fclose(file);

    return matrix;
}

void free_matrix(matrix_t* matrix) {
    free_2d_arr(matrix->data, matrix->rows);
    free(matrix);
}

void print_matrix(matrix_t* matrix) {
    for (int i = 0; i < matrix->rows; ++i) {
        printf("[");
        for (int j = 0; j < matrix->cols; ++j) {
            printf("%6.4lf ", matrix->data[i][j]);
        }
        printf("\b]\n");
    }
}

void multiply_parallel(int n_threads) {
    RESULT = malloc(sizeof(matrix_t));
    RESULT->data = alloc_2d_arr(A->rows, B->cols);
    RESULT->rows = A->rows;
    RESULT->cols = B->cols;
    printf("%d x %d\n", RESULT->rows, RESULT->cols);
    pthread_t* thread_pool = calloc(n_threads, sizeof(pthread_t));

    int chunk_size = RESULT->rows / n_threads;
    printf("Chunk size: %d\n", chunk_size);

    for (int i = 0; i < RESULT->rows; ++i) {
        int idx = n_threads % 2 == 0
            ? i % n_threads
            : (i * chunk_size) % n_threads;
        pthread_join(thread_pool[idx], NULL);
        //printf("Executing task %d on thread %d\n", i, idx);
        pthread_create(&thread_pool[idx], NULL, mult_task, (void*) i);
    }

    for (int i = 0; i < n_threads; ++i) {
        pthread_join(thread_pool[i], NULL);
    }

    free(thread_pool);
}

void* mult_task(void* args) {
    int row = (int) args;
    //free(args); segfault

    for (int j = 0; j < B->cols; ++j) {
        for (int k = 0; k < B->rows; ++k) {
            RESULT->data[row][j] += A->data[row][k] * B->data[k][j];
        }
    }

    pthread_exit(NULL);
}

double frobenius_norm(int n_threads) {
    pthread_t* thread_pool = calloc(n_threads, sizeof(pthread_t));
    int chunk_size = A->rows / n_threads;

    for (int i = 0; i < A->rows; ++i) {
        int idx = n_threads % 2 == 0
            ? i % n_threads
            : (i * chunk_size) % n_threads;
        pthread_join(thread_pool[idx], NULL);
        pthread_create(&thread_pool[idx], NULL, frobenius_task, (void*) i);
    }

    for (int i = 0; i < n_threads; ++i) {
        pthread_join(thread_pool[i], NULL);
    }

    free(thread_pool);

    return sqrt(SQ_SUM);
}

void* frobenius_task(void* args) {
    int row = (int) args;
    double partial = .0;

    for (int j = 0; j < A->cols; ++j) {
        partial += RESULT->data[row][j] * RESULT->data[row][j];
    }

    SQ_SUM += partial;

    pthread_exit(NULL);
}
