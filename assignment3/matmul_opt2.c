#include "matmul_opt2.h"

void matmul_opt2(unsigned long long int* mat_a, 
unsigned long long int* mat_b, unsigned long long int* mat_c, unsigned long long int matrix_size){
    //**********************************Add your optimized code below**********************************//
    unsigned long long int tile_size = 16;

    for (unsigned long long int tile_b = 0; tile_b < matrix_size; tile_b += tile_size) {
        for (unsigned long long int tile_a = 0; tile_a < matrix_size; tile_a += tile_size) {
            for (unsigned long long int j = tile_b; j < tile_b + tile_size && j < matrix_size; j++) {
                for (unsigned long long int k = tile_a; k < tile_a + tile_size && k < matrix_size; k++) {
                    for (unsigned long long int i = 0; i < matrix_size; i++) {
                        mat_c[j * matrix_size + i] += mat_a[k * matrix_size + i] * mat_b[j * matrix_size + k];
                    }
                }
            }
        }
    }
}
