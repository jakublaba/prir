#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

array_t* create_arr(int* data, int size) {
    array_t* arr = malloc(sizeof(array_t));
    arr->data = calloc(size, sizeof(int));
    if (arr->data == NULL) {
        fprintf(stderr, "Failed to allocate memory for array\n");
        exit(EXIT_FAILURE);
    }
    arr->size = size;

    memcpy(arr->data, data, size * sizeof(int));

    return arr;
}

void free_arr(array_t* arr) {
    free(arr->data);
    free(arr);
}

void print(array_t* arr) {
    for (int i = 0; i < arr->size; ++i) {
        printf("%d ", arr->data[i]);
    }
    printf("\b\n");
}

void save_to_file(array_t* arr, char* fname) {
    FILE* file = fopen(fname, "w");

    if (file == NULL) {
        fprintf(stderr, "Cannot open %s for writing\n", fname);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < arr->size; ++i) {
        fprintf(file, "%d\n", arr->data[i]);
    }
}
