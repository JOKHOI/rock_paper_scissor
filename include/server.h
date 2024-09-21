#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>  // For sockaddr_in

// Definition of the client structure
typedef struct {
    int socket;        // Socket file descriptor for the player
    char hand[10];     // Store player's hand ("rock", "paper", "scissor")
    int ready;         // Flag indicating if the player is ready
    int score;         // Player's score
} client;

// Function declarations
int createSocket(int *socketFd);
int attachSocket(int *socketFd);   // Change from void to int
int bindSocket(int *socketFd, struct sockaddr_in *address);  // Change from void to int
void configureServerAddr(struct sockaddr_in *address);  // Add missing return type
int startListen(int *socketFd);  // Change from void to int
void ioMultiplexing(int *socketFd);

#endif  // SERVER_H
