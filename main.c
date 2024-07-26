#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include "include/game.h"
#include "include/client.h"



int main(void)
{
	//single-player mode
	player P1 = {0,0};	
	player P2={0,0};
	player *AI   = &P1;
	player *User = &P2; 
	int socketFd;
	int *pointerSocket = &socketFd;
	struct sockaddr_in address;
	struct sockaddr_in *ptrAddress=&address ;
	srand(time(NULL));
	char input[10];
	int state;
	int i=0;	
	printf("\nPress 1 then hit enter to start single player\n");
	printf("\nPress 2 then hit enter to start Multiplayer\n");
	scanf("%d",&state);
	printf("\n==================================\n");
	switch(state)
	{	
		
	



		case 1:
			while(i<5){
					printf("\n\n===================================\n");
					printf("		ROUND %d		",i);
					printf("\n===================================\n");
					AI->hand = rand()%3;
					printf("\n\nWrite <rock>,<paper> or <scissor> no cap \n\n");
					scanf("%s",input);
					User->hand=checkUserInput(input);
					printAIHand(AI);
					checkPlayerResult(User,AI);
					i++;	

			}
			break;
		case 2:
			while(i<5){
					
				
					printf("\n\n===================================\n");
					printf("\n		ROUND %d		\n",i);
					printf("\n\n===================================\n");
					printf("Enter : rock,paper,scissor  \n");
					createSocket(pointerSocket);
					serverSetup(ptrAddress);
					startConnect(pointerSocket,ptrAddress);
					startListen(pointerSocket);
					return 0 ;
					i++;	

		}
	}			
	return 0 ;



}
