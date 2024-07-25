#include "include/client.h"

int main(void)
{
	int socketFd;
	int *pointerSocket = &socketFd;
	struct sockaddr_in address;
	struct sockaddr_in *ptrAddress=&address ;
	printf("rock=0,paper=1,scissor=2\n");
	createSocket(pointerSocket);
	serverSetup(ptrAddress);
	startConnect(pointerSocket,ptrAddress);
	startListen(pointerSocket);
	return 0 ;
}
