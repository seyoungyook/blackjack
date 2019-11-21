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

//playing game functions -----------------------------

//player settiing
int configUser(void) {
	
	printf("Input the number of players (MAX:5) : ");
	scanf("%d",&n_user);
	
	while(n_user > N_MAX_USER)
	{
		printf("Too many players!\n");
		printf("Input the number of players (MAX:5) : ");
		scanf("%d",&n_user);
		
	}
	
	return 0;
	
}


//betting
int betDollar(void) {
	
	int i;
	int yourbetting;
	
	srand(time(NULL));
	
	printf("-------BETTING STEP--------\n");
	printf(" -> your betting ( total :$%d) : ", dollar[0] );
	scanf("%d",&yourbetting);
	
	while(yourbetting>N_DOLLAR)
	{
		printf(" -> you only have $%d! bet again\n", dollar[0]);
		printf(" -> your betting ( total :$%d ) : ", dollar[0] );
		scanf("%d",&yourbetting);
	}
	
	bet[0] = yourbetting;
	
	
	for(i=1;i<n_user;i++)
		{
			bet[i] = rand()%5+1; // player can bet $1~$5
			printf(" -> player %d bets $%d ( out of $%d )\n",i,bet[i],dollar[i]);
		}

		
	return 0;
}


//offering initial 2 cards
void offerCards(void) {
	int i;
	cnt = 0;
	
	//1. give two card for each players
	for (i=0;i<n_user;i++)
	{
		cardhold[i][0] = cnt; //cnt mean the order of card and actual value of cnt is (the order of card -1) and in cardhold, we save the order of card
		cnt++;
		cardhold[i][1] = cnt;
		cnt++;
		cardSum[i] = getCardNum( cardhold[i][0] ) + getCardNum( cardhold[i][1] );
	}
	//2. give two card for the operator
	cardhold[n_user][0] = cnt;
	cnt++;
	cardhold[n_user][1] = cnt;
	cnt++;
	cardSum[n_user] = getCardNum( cardhold[n_user][0] ) + getCardNum( cardhold[n_user][1] );
	
	return;
}

//print initial card status
void printCardInitialStatus(void) {
	
	int i;
	
	printf("----------- CARD OFFERING -----------\n");
	
	//print server's card status
	
	printf("---server : X ");
	printCard( cardhold[n_user][1] ); //from the cardhold-order of card in tray- print card shape ans number
	printf("\n");
		
	//print your card status
		
	printf("---you : ");
	printCard( cardhold[0][0] );
	printCard( cardhold[0][1] );
	printf("\n");
	
	//print players's card status
	
	for(i=1;i<n_user;i++)
		{ 
			printf("---player %d : ",i);
			printCard( cardhold[i][0] );
			printCard( cardhold[i][1] );
			printf("\n");
	
		}
	
	return;
		
	
}

//for me
int getAction(void) {
	
	int G; //mean go ( G + 0 -> look like G0 )
	int i=2;
	
	if(cardSum[0]==21) //in blackjack
		{
			dollar[0] = dollar[0] + bet[0];
			printf("BLACK JACK! --> +$%d ($%d)\n", bet[0], dollar[0]);
		}
	
	while(cardSum[0]<21) //in under 21
	{
		printf("Action? ( 0 : go, others : stay ) : ");
		scanf("%d",&G);
	
		if(G==0)
			{
			cardhold[0][i]=cnt;
			cardSum[0] = cardSum[0]+ getCardNum(cardhold[0][i]);
			printUserCardStatus(0,(i+1));
			cnt++;
			i++;
			
			if(cardSum[0]>=21)
				break;
			else
				{
				printf("\n");
				}
			}
		else
			break;
	}
	
	if(cardSum[0]>21) //when overflow
		{
			dollar[0] = dollar[0] - bet[0];
			printf("DEAD (sum : %d) --> -$%d ($%d)\n", cardSum[0], bet[0], dollar[0]);
		}
	
}
//for player and server
int getActioninautomatic(int playernum){
	
	int i=2;
	
	if(cardSum[playernum]==21&&playernum!=n_user) //when blackjack
		{
			dollar[0] = dollar[0] + bet[0];
			printf("BLACK JACK! --> +$%d ($%d)\n", bet[0], dollar[0]);
		}
	
	while(cardSum[playernum]<17)
		{
			printf("GO!\n");
			cardhold[playernum][i]=cnt;
			cardSum[playernum]=cardSum[playernum] + getCardNum(cardhold[playernum][i]);
			printUserCardStatus(playernum,(i+1));
			cnt++;
			i++;
		}
		
	if(cardSum[playernum]>=17&&cardSum[playernum]<=21)
		printf("STAY!\n");
	else if(cardSum[playernum]>21&&playernum!=n_user) // when player overflow
		{
		dollar[playernum] = dollar[playernum] - bet[playernum];
		printf("DEAD (sum : %d) --> -$%d ($%d)\n", cardSum[playernum], bet[playernum], dollar[playernum]);
		}
	else if(cardSum[n_user]>21)	//when server overflow
		{
			printf("Server DEAD ( sum : %d )\n", cardSum[n_user]);
		}
}


void printUserCardStatus(int user, int cardcnt) {
	int i;
	
	printf("   -> card : ");
	for (i=0;i<cardcnt;i++)
		printCard(cardhold[user][i]);
	printf("\t ::: ");
}

