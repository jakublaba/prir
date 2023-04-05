#ifndef MATRIX_MUL_PARALLEL
#define MATRIX_MUL_PARALLEL

typedef struct matrix {
    int rows;
    int cols;
    double** data;
} matrix_t;

double** alloc_2d_arr(int, int);
void free_2d_arr(double**, int);
matrix_t* load_file(char*);
void print_matrix(matrix_t*);
void multiply_parallel(int);
void* mult_task(void*);
void free_matrix(matrix_t*);
double frobenius_norm(int);
void* frobenius_task(void*);

#endif
