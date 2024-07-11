#include "include/client.h"

int main(void)
{

	int socketFd;
	int *pointerSocket = &socketFd;
	struct sockaddr_in address;
	struct sockaddr_in *ptrAddress=&address ;

	createSocket(pointerSocket);
	serverSetup(ptrAddress);
	startConnect(pointerSocket,ptrAddress);
	startListen(pointerSocket);
	
}
