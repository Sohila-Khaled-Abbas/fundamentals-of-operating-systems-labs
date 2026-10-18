/**
 * Lab 08: CPU Cache Locality & Cache Lines
 * Fundamentals of Operating Systems - Hussein Nasser
 * 
 * Demonstrates CPU cache hierarchy (L1/L2/L3) and the performance impact
 * of spatial locality when traversing a large 2D matrix in row-major vs.
 * column-major order.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 4096
#define COLS 4096

// Dynamically allocated matrix on the heap (contiguous block)
static int matrix[ROWS][COLS];

// Measure CPU time in milliseconds
static double get_time_ms(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) * 1000.0 + 
           (end.tv_nsec - start.tv_nsec) / 1000000.0;
}

int main() {
    struct timespec start, end;
    long long sum = 0;

    printf("========================================================\n");
    printf("  Lab 08: CPU Cache Hierarchy & Spatial Locality Demo   \n");
    printf("========================================================\n");
    printf("Matrix size: %d x %d (%zu MB of contiguous memory)\n\n",
           ROWS, COLS, (ROWS * COLS * sizeof(int)) / (1024 * 1024));

    // 1. Initialize matrix
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            matrix[i][j] = 1;
        }
    }

    // 2. Row-Major Traversal (Cache-Friendly / High Spatial Locality)
    // CPU loads 64-byte cache lines; consecutive elements are already in L1 cache.
    clock_gettime(CLOCK_MONOTONIC, &start);
    sum = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            sum += matrix[i][j];
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double row_major_time = get_time_ms(start, end);
    printf("[1] Row-Major Traversal (Cache Friendly):\n");
    printf("    Time:       %.2f ms\n", row_major_time);
    printf("    Elements:   %lld\n\n", sum);

    // 3. Column-Major Traversal (Cache-Unfriendly / High Cache Miss Rate)
    // Access striding jumps by (COLS * 4) bytes, evicting cache lines before reuse.
    clock_gettime(CLOCK_MONOTONIC, &start);
    sum = 0;
    for (int j = 0; j < COLS; j++) {
        for (int i = 0; i < ROWS; i++) {
            sum += matrix[i][j];
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double col_major_time = get_time_ms(start, end);
    printf("[2] Column-Major Traversal (Cache Unfriendly):\n");
    printf("    Time:       %.2f ms\n", col_major_time);
    printf("    Elements:   %lld\n\n", sum);

    // 4. Comparative Analysis
    double speedup = col_major_time / row_major_time;
    printf("========================================================\n");
    printf("Performance Summary:\n");
    printf("    Speedup factor with Cache Locality: %.2fx faster!\n", speedup);
    printf("========================================================\n");
    printf("Takeaway: Modern CPUs are fast, but memory access is a\n");
    printf("bottleneck. Cache lines (64 bytes) fetch adjacent memory\n");
    printf("automatically into L1/L2 caches. Always write algorithms\n");
    printf("that leverage spatial and temporal cache locality.\n");

    return 0;
}
