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

void printUserCardStatus(int user, int cardcnt);

//get an integer input from standard input (keyboard)
//return : input integer value
//         (-1 is returned if keyboard input was not integer)
int getIntegerInput(void) {
    int input, num;
    
    num = scanf("%d", &input);
    fflush(stdin);
    if (num != 1) //if it fails to get integer
        input = -1;
    
    return input;
}


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


//card array controllers -------------------------------

//mix the card sets and put in the array
int mixCardTray(void) {
	
	srand(time(NULL));
	
	int i,j;
	
	
	for(i=0;i<N_CARDSET*N_CARD;i++)
	{
		shape[i] = rand()%4; //0=hart 1=dia 2=spade 3=club, save the order of card shape in cardtray
		num[i] = rand()%13; //save the order of card number in cardtray
		CardTray[i]=CardNum[shape[i]][num[i]];
		
		for(j=0;j<i;j++)
		{
			if( shape[i]==shape[j]&&num[i]==num[j] )
				{
					i=(i-1);
					break; //prevent overlap
				}
	
		}
		
	}	
		
	return 0;

}


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



int main(int argc, char *argv[]) {
	int roundIndex = 0;
	int round = 1; //for print round
	int max_user;
	int i, j, k; //used in for
	int BJ; //mean blackjack, actual value is sum of first and second cards
	
	srand((unsigned)time(NULL));
	
	//set the number of players
	configUser();


	//Game initialization --------
	//1. players' dollar
	for(i=0;i<n_user;i++)
		dollar[i] = 50;
	
	//2. card tray
	mixCardTray();



	//Game start --------
	do {
		
		betDollar();
		offerCards(); //1. give cards to all the players
		
		printCardInitialStatus();
		printf("\n------------------ GAME start --------------------------\n");
		//my turn
		printf(">>> My turn! ------\n ");
		printUserCardStatus(0, 2);
		getAction();
		
		//each player's turn
		for (j=1;j<n_user;j++) //each player
		{	
			printf(">>> Player %d turn! ------\n ", j);
			printUserCardStatus(j,2);
			getActioninautomatic(j);
		
		}
		
		//sever's turn 
		printf(">>> Server turn! ------\n ");
		printUserCardStatus(n_user,2);
		getActioninautomatic(n_user);
		BJ = getCardNum(cardhold[n_user][0])+getCardNum(cardhold[n_user][1]);
		
		if(cardSum[n_user]>21)
			printf("[[[[[[[ Server result is ... ... overflow!! ]]]]]]]\n");
	 	else if(BJ == 21)
			printf("[[[[[[[ Server result is ... ... BlackJack!! ]]]]]]]\n");
	 	else
			printf("[[[[[[[ Server result is ... ... %d!! ]]]]]]]\n",cardSum[n_user]);
	
		printf("------------ROUND %d result ....\n",round);
		//result
		checkResult();
		
		round++;
		cardIndex = cnt;
		
		printf("--------------------------------------------\n");
		printf("------------ROUND %d ( cardIndex : %d )\n",round,cardIndex);
		printf("--------------------------------------------\n");
		
		if( ((N_CARDSET*N_CARD) - cardIndex ) < (n_user*2) )
			{	printf("card ran out of the tray!! finishing the game\n");
				gameEnd = 1;
			}
		
		for(k=0;k<n_user;k++)
			{
				if ( dollar[k] <= 0 )
					{
					printf("someone go bankrupt!! finishing the game\n");
					gameEnd = 1;
					}
			}
		
	} while (gameEnd == 0);
	
	checkWinner();
	
	
	return 0;
}
