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
    memset(buffer, 0, tenMB);

    pid_t pid = fork(); // Fork a new process

    if (pid == -1) {
        // Error handling: if fork() returns -1, an error occurred
        perror("Failed to fork");
        free(buffer); // Clean up memory allocation
        return 1;
    } else if (pid > 0) {
        // Parent process
        while (1) { // Infinite loop
            printf("Parent process: PID = %d, buffer[0] = %d\n", getpid(), buffer[0]);
  //          buffer[0]++; // Modify first byte
            sleep(1); // Sleep for 1 second to avoid flooding the output
        }
        free(buffer); // Ideally, this should be reached if the loop ends
    } else {
        // Child process
        int Y = 20; // Child process's new variable
        while (1) { // Infinite loop
            printf("Child process: PID = %d, Y = %d, buffer[0] = %d\n", getpid(), Y, buffer[0]);
//            buffer[0]++; // Modify first byte
            sleep(1); // Sleep for 1 second to avoid flooding the output
        }
        // Child process also needs to free memory if it ever exits the loop
    }

    return 0; // This line will never be reached in the current setup
}

