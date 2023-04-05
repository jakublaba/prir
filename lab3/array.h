#ifndef ARRAY
#define ARRAY

typedef struct array {
    int* data;
    int size;
} array_t;

array_t* create_arr(int*, int);
void free_arr(array_t*);
void print(array_t*);
void save_to_file(array_t*, char*);

#endif
