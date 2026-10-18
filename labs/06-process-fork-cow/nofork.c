#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main() {
    // Allocate 10 MB of memory and initialize it to 0 before forking
    size_t tenMB = 10 * 1024 * 1024; // 10 MB

    char *buffer = (char *)malloc(tenMB);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
   // memset(buffer, 0, tenMB);

        // Parent process
        while (1) { // Infinite loop
            printf("Parent process: PID = %d, buffer[0] = %d\n", getpid(), buffer[0]);
  //          buffer[0]++; // Modify first byte
            sleep(1); // Sleep for 1 second to avoid flooding the output
        }
        free(buffer); // Ideally, this should be reached if the loop ends

    return 0; // This line will never be reached in the current setup
}

