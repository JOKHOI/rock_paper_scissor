#ifndef	SERVER_H
#define SERVER_H

#include<arpa/inet.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/time.h>
#include <sys/select.h>
#include <sys/time.h>

typedef struct {
	int socket;
    char hand[1024];	
    int ready;
	int score;
}client;


int createSocket(int *socketFd);
void attachSocket(int *socketFd);
void configureServerAddr(struct sockaddr_in *address);
void bindSocket(int *socketFd,struct sockaddr_in *address);
void startListen(int * socketFd);
void ioMultiplexing(int * socketFd); 
#endif
