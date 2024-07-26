#include "include/client.h"

int main(void)
{
	int socketFd;
	int *pointerSocket = &socketFd;
	struct sockaddr_in address;
	struct sockaddr_in *ptrAddress=&address ;
	printf("Enter : rock,paper,scissor  \n");
	createSocket(pointerSocket);
	serverSetup(ptrAddress);
	startConnect(pointerSocket,ptrAddress);
	startListen(pointerSocket);
	return 0 ;
}
