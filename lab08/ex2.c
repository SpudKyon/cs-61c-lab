#include "ex2.h"

double dotp_naive(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    for (int i = 0; i < arr_size; i++)
        global_sum += x[i] * y[i];
    return global_sum;
}

// Critical Keyword
double dotp_critical(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    // TODO: Implement this function
    #pragma omp parallel
    {
        double local_sum = 0.0;

        for (int i = 0; i < arr_size; i++)
            local_sum += x[i] * y[i];

        #pragma omp critical
        global_sum = local_sum;
    }
    // Use the critical keyword here!
    return global_sum;
}

// Reduction Keyword
double dotp_reduction(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    // TODO: Implement this function
    #pragma omp parallel for reduction(+:global_sum)
    for (int i = 0; i < arr_size; i++)
        global_sum += x[i] * y[i];
    // Use the reduction keyword here!
    return global_sum;
}

// Manual Reduction
double dotp_manual_reduction(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    // TODO: Implement this function
    double local_sums[omp_get_max_threads()];
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        local_sums[thread_id] = 0.0;

        #pragma omp for
        for (int i = 0; i < arr_size; i++)
            local_sums[thread_id] += x[i] * y[i];
    }

    // Combine local sums into the global sum
    for (int i = 0; i < omp_get_max_threads(); i++)
        global_sum += local_sums[i];
    // Do NOT use the `reduction` directive here!
    return global_sum;
}
