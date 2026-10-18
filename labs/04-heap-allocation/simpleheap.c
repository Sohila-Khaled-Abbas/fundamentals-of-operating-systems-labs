#include <stdlib.h>

int main() {
    // Allocate memory for a 32-bit integer
    int *ptr = malloc(sizeof(int));
    // Store the value 10 in the allocated memory
    *ptr = 10;
    // Add 1 to the stored value
    *ptr += 1;
    // Free the allocated memory
    free(ptr);
    return 0;
}
