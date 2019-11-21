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

// calculate the card sum and see if : 1. under 21, 2. over 21, 3. blackjack
//cardSum<=21
int calcStepResult(int cardSum, int playernum, int serversum) {
	
	int BJ; //mean blackjack, actual value is sum of first and second cards
	int k;
	
	BJ = getCardNum(cardhold[playernum][0])+getCardNum(cardhold[playernum][1]);
	
	if(getCardNum(cardhold[n_user][0])+getCardNum(cardhold[n_user][1])!=21)
		
		{
		if(BJ == 21 )
			printf("Win due to BlackJack! ($%d)\n", dollar[playernum]);
		else if(cardSum>=serversum&&cardSum<=21&&BJ!=21)
			{
			dollar[playernum] = dollar[playernum] + bet[playernum];
			printf("Win (sum=%d) --> ($%d)\n", cardSum, dollar[playernum] );
			}
		else if(cardSum<serversum&&cardSum<21&&serversum<=21)
			{
			dollar[playernum] = dollar[playernum] - bet[playernum];
			printf("Lose (sum=%d) --> ($%d)\n", cardSum, dollar[playernum] );
			}
		else if(cardSum>21)
			printf("Lose due to overflow! ($%d)\n", dollar[playernum]);
			
		else if(serversum>21&&cardSum<=21)
			{
			dollar[playernum] = dollar[playernum] + bet[playernum];
			printf("Win (sum=%d) --> ($%d)\n", cardSum, dollar[playernum] );
			}
			
	}
	else if(getCardNum(cardhold[n_user][0])+getCardNum(cardhold[n_user][1])==21)
		{
		if(BJ == 21 )
			printf("Win due to BlackJack! ($%d)\n", dollar[playernum]);
		else
			{
			dollar[playernum] = dollar[playernum] - bet[playernum];
			printf("Lose (sum=%d) --> ($%d)\n", cardSum, dollar[playernum] );
			}
		}
}

int checkResult() {
	
	int i;
	
	printf(" your result : ");
	calcStepResult(cardSum[0],0,cardSum[n_user]);
	printf("\n");
	
	for(i=1;i<n_user;i++)
		{
			printf(" %d'th player's result : ", i);
			calcStepResult(cardSum[i],i,cardSum[n_user]);
			printf("\n");
		}
	
}

int checkWinner() {
	
	int i,j,k,l;
	int win=0;
	int wwin=0;
	
	printf("--------------------------------------------\n");
	printf("--------------------------------------------\n");
	printf("--------------------------------------------\n");
	printf("game end!! your money : $%d, player's money : ' ", dollar[0]);
	
	for(i=1;i<n_user;i++)
		{
			printf(" %d,", dollar[i]);
		}
	printf("\n");
	
	for(j=1;j<n_user;j++)
		{
		if((dollar[0]-dollar[j])<0)
			break;
		else
			win++;
			}

	if(win==(n_user-1))
		printf("your win\n");
			
	for(k=1;k<n_user;k++)
		{
			for(l=0;l<n_user;l++)
			 {
			if((dollar[k]-dollar[l])<0)
				break;
			else 
				wwin++;
			 }
		 if(wwin == n_user)
		 	printf("player %d's win\n",k);
		}
}
