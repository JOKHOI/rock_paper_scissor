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

void evaluateHand(client player[], int clientCount )
{
    if (clientCount < 2){
        return;
    }
    
    if (player[0].ready == 1 && player[1].ready == 1){
        char *result1;
        char *result2;
        if(strcmp(player[0].hand, player[1].hand) == 0)
        {
            result1 = "\n\t===Draw===\n";
            result2 = "\n\t===Draw===\n";
        }
        else if (strcmp(player[0].hand , "rock")==0 && strcmp(player[1].hand , "scissor")==0 || strcmp(player[0].hand , "paper")==0 && strcmp(player[1].hand , "rock")==0 || strcmp(player[0].hand , "scissor")==0 && strcmp(player[1].hand , "paper")==0)
        {
            player[0].score += 1;
            result1 = "\t===You Win===\n";
            result2 = "\t===You Lose===\n";
        }
        else 
        {
            player[1].score += 1;
            result1 = "\t===You Lose===\n";
            result2 = "\t===You Win===\n";
        }
    
        write(player[0].socket, result1, strlen(result1));
        write(player[1].socket, result2, strlen(result2));

        player[0].ready = 0;
        player[1].ready = 0;
    }
}


// Function to handle I/O multiplexing using select()
// It monitors multiple client connections to handle incoming data and new connections
void ioMultiplexing(int *socketFd) {
    fd_set readfds; // Set of file descriptors to monitor for read operations
    int max_fd, readyfds, active_connections = 0;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    client player[MAX_PLAYER]={{0}};
    // Server loop to continuously monitor and handle I/O operations
    while (1) {
        // Clear the socket set
        FD_ZERO(&readfds);

        // Add server socket to set
        FD_SET(*socketFd, &readfds);
        max_fd = *socketFd;

        // Add client sockets to set
        for (int i = 0; i < MAX_PLAYER; i++) {
            if (player[i].socket != 0)
                FD_SET(player[i].socket, &readfds); // Add each active client socket to the set

            if (player[i].socket > max_fd)
                max_fd = player[i].socket; // Update the maximum file descriptor number
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
                if (player[i].socket == 0) {
                    player[i].socket = new_socket; // Store the new client socket
                    active_connections++;
                    break;
                }
            }
        }

        // Handle I/O operations on client sockets
        for (int i = 0; i < MAX_PLAYER; i++) { // Iterate over all possible clients
            int sd = player[i].socket; // Get the client socket descriptor
            if (sd != 0 && FD_ISSET(sd, &readfds)) {
                char buffer[1024] = {0};
                // Read data from the client socket
                int valread = read(sd, buffer, 1024);
                if (valread <= 0) {
                    if (valread == 0) {
                        // Client disconnected
                        printf("Host %s disconnected\n", inet_ntoa(address.sin_addr));
                    } else {
                        // Error occurred
                        fprintf(stderr, "Read error: %s (code: %d)\n", strerror(errno), errno);
                    }
                    close(sd); // Close the client socket
                    player[i].socket = 0; // Mark the client socket as inactive
                } else {
                         buffer[valread] = '\0';
                         strncpy(player[i].hand, buffer, sizeof(buffer));
                         player[i].ready = 1;
                         printf("Player %d: %s\n", i, player[i].hand);
                         for(int j=0; j<MAX_PLAYER; j++){
                            if(player[j].socket != 0){
                                evaluateHand(player, MAX_PLAYER);
                         }
                         
                    }
                }
            }
        }
    }

}