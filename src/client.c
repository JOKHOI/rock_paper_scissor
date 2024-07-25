#include "../include/client.h"  // Include the client header file for function declarations and macros
#define PORT 8081 // Define the port number to connect to the server
#define ROCK 0
#define PAPER 1
#define SCISSOR 2
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
    if (inet_pton(AF_INET, "192.168.2.240", &address->sin_addr) <= 0) {
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
    fd_set waitfds;  // Set of file descriptors to monitor (e.g., keyboard, network socket)
    int readyfds;  // Number of file descriptors ready for I/O
    char recvline[1024];  // Buffer for data received from the network
    char sendline[1024];  // Buffer for data to send to the network
    while (1) {  // Infinite loop to continuously check for I/O
        FD_ZERO(&waitfds);  // Clear the set of monitored file descriptors
        FD_SET(*socketFd, &waitfds);  // Add network socket to the set of monitored file descriptors
        FD_SET(STDIN_FILENO, &waitfds);  // Add standard input (keyboard) to the set of monitored file descriptors
        memset(recvline, 0, sizeof(recvline));  // Clear the buffer for received data
        memset(sendline, 0, sizeof(sendline));  // Clear the buffer for data to send

        // Wait for activity on either the socket or stdin (keyboard)
        readyfds = select(*socketFd + 1, &waitfds, NULL, NULL, NULL);
        if ((readyfds < 0) && (errno != EINTR)) {  // Check for errors in select() call
            printf("select error");  // Print error message if select fails
        }

        // Check if there is input from the keyboard
        if (FD_ISSET(STDIN_FILENO, &waitfds)) {
            // Read a line of input from the keyboard into sendline buffer
            if (fgets(sendline, sizeof(sendline), stdin) != NULL) {
	       write(*socketFd, sendline, strlen(sendline));
        	    }
        	}

        // Check if there is data coming from the network socket
        if (FD_ISSET(*socketFd, &waitfds)) {
            // Read the incoming data into recvline buffer
            int n = read(*socketFd, recvline, sizeof(recvline) - 1);
            if (n > 0) {  // If data is received
                recvline[n] = '\0';  // Null-terminate the received data
                // Print the received message to the screen
                fprintf(stdout, "%s", recvline);
            } else if (n < 0) {  // If there is an error reading the data
                perror("read error");  // Print the error message
            }
        }
    }
}
