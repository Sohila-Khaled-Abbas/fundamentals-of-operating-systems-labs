#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char source_ip[16];
    char destination_ip[16];
    int source_port;
    int destination_port;
    int packet_length;
} Packet;

int main() {
    Packet *packets = (Packet *)malloc(1000000 * sizeof(Packet));
    Packet *p2 = (packets + 1);
    // You can initialize the packet fields here, if needed

    // Use the packets here

    // Free the allocated memory
    free(packets);

    return 0;
}

