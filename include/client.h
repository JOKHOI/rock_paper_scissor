#ifndef CLIENT_H
#define CLIENT_H
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h> // Added for exit()
#include <errno.h> // Added for errno
#include <sys/select.h> // Added for select()
int createSocket(int *socketFd);
void startListen(int *socketFd);
void startConnect(int *socketFd, struct sockaddr_in *address);
void serverSetup(struct sockaddr_in *address);
int readLine(char * buffer,int maxchars,char eoc);
#endif
