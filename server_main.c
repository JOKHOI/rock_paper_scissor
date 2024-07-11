#include "include/server.h"


int main(void){

	int socketFd;
	int *pointerSocket = &socketFd;
	struct sockaddr_in address;
	struct sockaddr_in *ptrAddress=&address ;
	createSocket(pointerSocket);
	attachSocket(pointerSocket);

	configureServerAddr(ptrAddress);
	bindSocket(pointerSocket,ptrAddress);
	startListen(pointerSocket);
	ioMultiplexing(pointerSocket); 
	return 0;
}







