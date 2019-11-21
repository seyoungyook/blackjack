#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_CARDSET			1
#define N_CARD				52
#define N_DOLLAR			50


#define N_MAX_CARDNUM		13
#define N_MAX_USER			5
#define N_MAX_CARDHOLD		10
#define N_MAX_GO			17
#define N_MAX_BET			5

#define N_MIN_ENDCARD		30

//card tray object
int CardTray[N_CARDSET*N_CARD];
int cardIndex = 0;							


//player info
int dollar[N_MAX_USER];						//dollars that each player has
int n_user;									//number of users


//play yard information
int cardhold[N_MAX_USER+1][N_MAX_CARDHOLD];	//cards that currently the players hold
int cardSum[N_MAX_USER];					//sum of the cards
int bet[N_MAX_USER];						//current betting 
int gameEnd = 0; 							//game end flag

//some utility functions
int cnt; //the (order -1) of the card in tray (ex, when cnt = 1, shape[1] means the shape of second card in the tray )
char Shape[4][10] = { "Hart", "Dia", "Spade", "Club" };  //for print shape name
char KAJQ[13][5] = { "K", "A", "x", "x", "x", "x", "x", "x", "x", "x", "x", "J", "Q" }; //for print letter K,A,J,Q
int CardNum[4][13] = {
{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10},
{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10},
{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10},
{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10}}; //save the actual card number in the blackjack game
int shape[N_CARDSET*N_CARD]; //save card's shape order in CardTray
int num[N_CARDSET*N_CARD]; //save card's number order in CardTray

//card processing functions ---------------

//calculate the actual card number in the blackjack game
//int cardnum mean cardhold[n][m]-order of the card in cardtray
int getCardNum( int cardnum ) {
	
	return CardNum[shape[cardnum]][num[cardnum]];
	
}

//print the card information (e.g. DiaA)
//int cardnum mean cardhold[n][m]
void printCard( int cardnum ) {

	if(num[cardnum]>=2 && num[cardnum] <= 10) //when the card number is number
		printf(" %s%d ", Shape[shape[cardnum]],num[cardnum] );
	else if(num[cardnum]==0||num[cardnum]==1||num[cardnum]==11||num[cardnum]==12)//when the card number is leter
		printf(" %s%s ", Shape[shape[cardnum]], KAJQ[num[cardnum]]);


	return;
	
	
}

