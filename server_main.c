#include "include/server.h"
#include <stdio.h>
#include <stdlib.h>  // For exit(), EXIT_FAILURE

int main(void) {
    int socketFd;
    struct sockaddr_in address;

    // Create socket and check for errors
    if (createSocket(&socketFd) < 0) {
        fprintf(stderr, "Error: Failed to create socket.\n");
        exit(EXIT_FAILURE);  // Exit on error
    }

    // Attach socket options (like SO_REUSEADDR) and check for errors
    if (attachSocket(&socketFd) < 0) {
        fprintf(stderr, "Error: Failed to attach socket options.\n");
        exit(EXIT_FAILURE);  // Exit on error
    }

    // Configure server address (port, IP)
    configureServerAddr(&address);

    // Bind the socket to the address and check for errors
    if (bindSocket(&socketFd, &address) < 0) {
        fprintf(stderr, "Error: Failed to bind socket.\n");
        exit(EXIT_FAILURE);  // Exit on error
    }

    // Start listening for incoming connections and check for errors
    if (startListen(&socketFd) < 0) {
        fprintf(stderr, "Error: Failed to start listening on socket.\n");
        exit(EXIT_FAILURE);  // Exit on error
    }

    // Begin I/O multiplexing for handling client connections
    ioMultiplexing(&socketFd);  // Assuming ioMultiplexing handles its own errors

    return 0;  // Exit normally
}
