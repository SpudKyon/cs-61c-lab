#include "ex1.h"

void v_add_naive(double* x, double* y, double* z) {
    #pragma omp parallel
    {
        for(int i=0; i<ARRAY_SIZE; i++)
            z[i] = x[i] + y[i];
    }
}

// Adjacent Method
void v_add_optimized_adjacent(double* x, double* y, double* z) {
    #pragma omp parallel
    {
        int num_threads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();

        for (int i = thread_id; i < ARRAY_SIZE; i += num_threads) {
            z[i] = x[i] + y[i];
        }
    }
}

// Chunks Method
void v_add_optimized_chunks(double* x, double* y, double* z) {
    #pragma omp parallel
    {
        int num_threads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();
        int chunk_size = (ARRAY_SIZE + num_threads - 1) / num_threads; // Ceiling division

        int start = thread_id * chunk_size;
        int end = (thread_id == num_threads - 1) ? ARRAY_SIZE : start + chunk_size;

        for (int i = start; i < end; i++) {
            z[i] = x[i] + y[i];
        }
    }
    // Do NOT use the `for` directive here!
}
