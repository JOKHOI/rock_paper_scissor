#include "../include/server.h"
#include "../include/game.h"
#define MAX_PLAYER 2

// Function to create a socket and return its file descriptor
int createSocket(int *socketFd)
{
    if((*socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("\nError while creating socket...\n");
        exit(1);
    }
    return *socketFd;
}

// Function to set socket options, allowing reuse of address and port
void attachSocket(int *socketFd)
{
    int opt = 1;
    if(setsockopt(*socketFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
}

// Function to configure server address struct
void configureServerAddr(struct sockaddr_in *address)
{
    address->sin_family = AF_INET;
    address->sin_port = htons(8081);
    address->sin_addr.s_addr = INADDR_ANY;
}

// Function to bind the socket to the specified address and port
void bindSocket(int *socketFd, struct sockaddr_in *address)
{
    socklen_t addrlen = sizeof(struct sockaddr_in);
    if(bind(*socketFd, (struct sockaddr *) address, addrlen) < 0)
    {
        perror("bind failed...");
        exit(1);    
    }
}

// Function to start listening for incoming connections
void startListen(int *socketFd)
{
    if(listen(*socketFd, MAX_PLAYER) < 0){
        perror("Listen failed...");
        exit(1);
    }
}

// Function to handle I/O multiplexing using select()
// It monitors multiple client connections to handle incoming data and new connections
void ioMultiplexing(int *socketFd) {
    fd_set readfds; // Set of file descriptors to monitor for read operations
    int max_fd, readyfds, active_connections = 0;
    int client_socket[MAX_PLAYER] = {0}; // Array to hold client socket file descriptors
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char inBuffer[MAX_PLAYER][1024]; // Buffers for incoming data
    char outBuffer[MAX_PLAYER][1024]; // Buffers for outgoing data

    // Server loop to continuously monitor and handle I/O operations
    while (1) {
        // Clear the socket set
        FD_ZERO(&readfds);

        // Add server socket to set
        FD_SET(*socketFd, &readfds);
        max_fd = *socketFd;

        // Add client sockets to set
        for (int i = 0; i < MAX_PLAYER; i++) {
            if (client_socket[i] != 0)
                FD_SET(client_socket[i], &readfds); // Add each active client socket to the set

            if (client_socket[i] > max_fd)
                max_fd = client_socket[i]; // Update the maximum file descriptor number
        }

        // Wait for an activity on one of the sockets
        readyfds = select(max_fd + 1, &readfds, NULL, NULL, NULL);

        if ((readyfds < 0) && (errno != EINTR)) {
            printf("select error"); // Print an error message if select fails
        }

        // Check if there's an incoming connection on the server socket
        if (FD_ISSET(*socketFd, &readfds)) {
            int new_socket;
            if ((new_socket = accept(*socketFd, (struct sockaddr *)&address, &addrlen)) < 0) {
                perror("accept"); // Print an error message if accept fails
                exit(EXIT_FAILURE); // Exit the program if accept fails
            }

            // Inform about the new connection
            printf("New connection from %s\n", inet_ntoa(address.sin_addr));

            // Add new socket to client socket array
            for (int i = 0; i < MAX_PLAYER; i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket; // Store the new client socket
                    active_connections++;
                    break;
                }
            }
        }

        // Handle I/O operations on client sockets
        for (int i = 0; i < MAX_PLAYER; i++) { // Iterate over all possible clients
            int sd = client_socket[i]; // Get the client socket descriptor
            if (sd != 0 && FD_ISSET(sd, &readfds)) {
                memset(inBuffer[i], 0, 1024); // Clear the input buffer for the client
                memset(outBuffer[i], 0, 1024); // Clear the output buffer for the client

                // Read data from the client socket
                int valread = read(sd, inBuffer[i], 1024);
                if (valread <= 0) {
                    if (valread == 0) {
                        // Client disconnected
                        printf("Host %s disconnected\n", inet_ntoa(address.sin_addr));
                    } else {
                        // Error occurred
                        fprintf(stderr, "Read error: %s (code: %d)\n", strerror(errno), errno);
                    }
                    close(sd); // Close the client socket
                    client_socket[i] = 0; // Mark the client socket as inactive
                } else {
                    // Echo back the message to other clients
                    inBuffer[i][valread] = '\0'; // Null-terminate the received data
                    printf("client%d: %s", i, inBuffer[i]); // Print the received message
                    char tempBuffer[1024];
                    snprintf(tempBuffer, sizeof(tempBuffer), "client%d", i);
                    strcat(outBuffer[i], tempBuffer);
                    strcat(outBuffer[i], " : ");
                    strcat(outBuffer[i], inBuffer[i]);
                    //write to the other clients
                    for (int j = 0; j < MAX_PLAYER; j++) { // Iterate over all possible clients
                        if (client_socket[j] != 0 ) {
                            

                            write(client_socket[j],"hi\n", 4); // Send the message to other clients
                        }
                    }
                }
            }
        }
    }
}
