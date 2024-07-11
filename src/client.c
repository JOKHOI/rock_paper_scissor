#include "../include/client.h"  // Include the client header file for function declarations and macros
#define PORT 8081 // Define the port number to connect to the server

// Function to create a socket and return its file descriptor
int createSocket(int *socketFd)
{
    // Create a socket using AF_INET (IPv4), SOCK_STREAM (TCP), and default protocol
    if((*socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        // Print an error message and exit if socket creation fails
        perror("\nError while creating socket...\n");
        exit(1);
    }
    return *socketFd;  // Return the socket file descriptor
}

// Function to read a line from stdin until end of character or max characters is reached
int readLine(char *buffer, int maxchars, char eoc){
    int n = 0;  // Initialize a counter for the number of characters read
    while (n < maxchars) {  // Loop until the maximum characters are read
        buffer[n] = getc(stdin);  // Read a character from standard input
        if (buffer[n] == eoc)  // If the end of character is reached, break the loop
            break;
        n++;  // Increment the counter
    }
    return n;  // Return the number of characters read
}

// Function to set up server address struct
void serverSetup(struct sockaddr_in *address)
{
    address->sin_family = AF_INET;  // Set the address family to AF_INET (IPv4)
    address->sin_port = htons(PORT);  // Set the port number, converting it to network byte order

    // Convert the IP address from text to binary form and store it in address->sin_addr
    if (inet_pton(AF_INET, "127.0.0.1", &address->sin_addr) <= 0) {
        // Print an error message and exit if the address conversion fails
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }
}

// Function to initiate connection to the server
void startConnect(int *socketFd, struct sockaddr_in *address)
{
    // Connect to the server using the specified socket and address
    if (connect(*socketFd, (struct sockaddr *) address, sizeof(*address)) < 0) {
        // Print an error message and exit if the connection fails
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
}

// Function to handle I/O multiplexing using select()
// It monitors both stdin and the socket for incoming data and user input
void startListen(int *socketFd)
{
    fd_set waitfds;  // Declare a set of file descriptors to be monitored
    int readyfds;  // Declare a variable to store the number of ready file descriptors
    char recvline[1024];  // Buffer to store received data
    char sendline[1024];  // Buffer to store data to be sent

    while (1) {
        FD_ZERO(&waitfds);  // Clear the set of file descriptors
        FD_SET(*socketFd, &waitfds);  // Add the socket file descriptor to the set
        FD_SET(STDIN_FILENO, &waitfds);  // Add the standard input file descriptor to the set
        memset(recvline, 0, sizeof(recvline));  // Initialize the receive buffer with zeros
        memset(sendline, 0, sizeof(sendline));  // Initialize the send buffer with zeros

        // Wait for an activity on one of the file descriptors
        readyfds = select(*socketFd + 1, &waitfds, NULL, NULL, NULL);
        if ((readyfds < 0) && (errno != EINTR)) {
            // Print an error message if select fails
            printf("select error");
        }

        // Check if there's user input from stdin
        if (FD_ISSET(STDIN_FILENO, &waitfds)) {
            // Read a line from stdin and store it in sendline
            if (fgets(sendline, sizeof(sendline), stdin) != NULL) {
                // Send the data to the server
                write(*socketFd, sendline, strlen(sendline));
            }
        }

        // Check if there's data from the server
        if (FD_ISSET(*socketFd, &waitfds)) {
            // Read data from the socket and store it in recvline
            int n = read(*socketFd, recvline, sizeof(recvline) - 1);
            if (n > 0) {
                recvline[n] = '\0';  // Null-terminate the received string
                // Print the received message to stdout
                fprintf(stdout, "%s", recvline);
            } else if (n < 0) {
                // Print an error message if reading fails
                perror("read error");
            }
        }
    }
}
