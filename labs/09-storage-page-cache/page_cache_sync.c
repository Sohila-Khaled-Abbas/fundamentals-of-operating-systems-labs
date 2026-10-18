/**
 * Lab 09: Storage Management & The OS Page Cache
 * Fundamentals of Operating Systems - Hussein Nasser
 * 
 * Demonstrates:
 * 1. Buffered I/O (write() writes to the OS Page Cache in RAM - fast)
 * 2. Synchronous flushing (fsync() forces dirty pages from RAM to disk - slow)
 * 3. The trade-off between write throughput and crash durability
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
    #include <io.h>
    #include <fcntl.h>
    #define fsync _commit
#else
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/stat.h>
#endif

#define RECORD_COUNT 1000
#define RECORD_SIZE 128

// Calculate elapsed time in milliseconds
static double get_time_ms(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) * 1000.0 + 
           (end.tv_nsec - start.tv_nsec) / 1000000.0;
}

int main() {
    struct timespec start, end;
    char buffer[RECORD_SIZE];
    memset(buffer, 'A', RECORD_SIZE - 1);
    buffer[RECORD_SIZE - 1] = '\n';

    printf("========================================================\n");
    printf("   Lab 09: Storage Management & OS Page Cache Demo      \n");
    printf("========================================================\n");
    printf("Writing %d records of %d bytes each...\n\n", RECORD_COUNT, RECORD_SIZE);

    // Experiment 1: Buffered Writes (Page Cache only, no explicit fsync)
    const char *file1 = "buffered_output.tmp";
    FILE *fp1 = fopen(file1, "wb");
    if (!fp1) {
        perror("Failed to open file1");
        return 1;
    }

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < RECORD_COUNT; i++) {
        fwrite(buffer, 1, RECORD_SIZE, fp1);
    }
    fclose(fp1); // Flushes userspace buffer to page cache
    clock_gettime(CLOCK_MONOTONIC, &end);
    double buffered_time = get_time_ms(start, end);

    printf("[1] Buffered Writes (Kernel Page Cache):\n");
    printf("    Elapsed Time: %.2f ms\n", buffered_time);
    printf("    Latency/Write: %.4f ms\n", buffered_time / RECORD_COUNT);
    printf("    Status: Data resides in RAM (dirty pages), OS flushes lazily.\n\n");

    // Experiment 2: Synchronous Writes (calling fsync after every write)
    const char *file2 = "fsync_output.tmp";
    FILE *fp2 = fopen(file2, "wb");
    if (!fp2) {
        perror("Failed to open file2");
        return 1;
    }
    int fd2 = fileno(fp2);

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < RECORD_COUNT; i++) {
        fwrite(buffer, 1, RECORD_SIZE, fp2);
        fflush(fp2);
        fsync(fd2); // Force flush from OS Page Cache to physical disk
    }
    fclose(fp2);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double fsync_time = get_time_ms(start, end);

    printf("[2] Synchronous Writes (fsync after each write):\n");
    printf("    Elapsed Time: %.2f ms\n", fsync_time);
    printf("    Latency/Write: %.4f ms\n", fsync_time / RECORD_COUNT);
    printf("    Status: Every write is guaranteed durable on physical storage.\n\n");

    // Clean up temporary files
    remove(file1);
    remove(file2);

    // Analysis
    double ratio = (buffered_time > 0) ? (fsync_time / buffered_time) : 0;
    printf("========================================================\n");
    printf("Analysis & Insights:\n");
    printf("    fsync is ~%.1fx slower due to disk head seek / flash write cycles.\n", ratio);
    printf("========================================================\n");
    printf("Key OS Concepts:\n");
    printf("- The OS Page Cache buffers disk writes in memory for high throughput.\n");
    printf("- 'Dirty pages' are flushed periodically by kernel threads (e.g. pdflush/flush).\n");
    printf("- Relational databases (Postgres, MySQL WAL) must use fsync() for ACID durability,\n");
    printf("  which is why group commit and WAL optimization are critical.\n");

    return 0;
}
