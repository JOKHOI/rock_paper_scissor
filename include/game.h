#ifndef GAME_H 
#define GAME_H
#define ROCK 0
#define PAPER 1
#define SCISSOR 2

typedef struct {
	int hand;
	char buffer[1024];
	int score;
}player;

int checkUserInput(char *userInput);
void printAIHand(player *AI);
void checkPlayerResult( player *User, player *AI);



#endif
