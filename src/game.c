#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include "../include/game.h"
int checkUserInput(char *userInput)
{
	if (strcmp(userInput,"rock")==0)
	{
		return ROCK;
	}
	else if(strcmp(userInput,"paper")==0)
	{	
		return PAPER;
	}

	else if (strcmp(userInput,"scissor")==0)
	{
		return SCISSOR;
	}
	else
	{
		printf("INVALID INPUT");
		exit(1);
	
	}


}


void printAIHand( player *AI)
{
	int hand = AI->hand;
	switch (hand)
	{
		case 0:
			printf("\nAI hand was ROCK\n");
			break;
		case 1:
			printf("\nAI hand was PAPER\n");
			break;
	
		case 2: 
			printf("\nAI hand was scissor\n");
			break;
		default:
			printf("Something is wrong in print AI hand\n ");
	}
}


void checkPlayerResult( player *User, player *AI)
{		
		int result;
		result = User->hand-AI->hand;
		//CHECK FOR NON EXISTANT MOVE
		if(User->hand<ROCK || User->hand>SCISSOR){
			exit(1);
		}	
		switch(result)
		{
			case  0:
				printf("\033[1;33m");
				printf("\n===============No Winners================\n");
				printf("\033[0m");
				printf("\nUser score is:%d\n",User->score);
				printf("\nAI score is:%d\n",AI->score);
				break;
			case  1:
				printf("\033[1;32m");
				printf("\n===============User Wins===================\n");
				printf("\033[0m");
				User->score++;
				printf("\nUser score is:%d\n",User->score);
				printf("\nAI score is:%d\n",AI->score);
				break;
			case -1:
				printf("\033[1;31m");
				printf("\n===============AI wins====================\n");
				printf("\033[0m");
				AI->score++;
				printf("\nUser score is:%d\n",User->score);
				printf("\nAI score is:%d\n",AI->score);
				break;
			case  2:
				printf("\033[1;31m");
				printf("\n===============AI win=====================\n");
				printf("\033[0m");
				AI->score++;
				printf("\nUser score is:%d\n",User->score);
				printf("\nAI score is:%d\n",AI->score);
				break;
		
			case  -2:
				printf("\033[1;32m");
				printf("\n==============User wins====================\n");
				printf("\033[0m");
				User->score++;
				printf("\nUser score is:%d\n",User->score);
				printf("\nAI score is:%d\n",AI->score);
				break;
			default :
				printf("something is wrong in checkPlayerResult");
				break;
		
		}
		






}












