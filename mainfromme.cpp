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

#define hart				0
#define dia					1
#define spade				2
#define club				3
#define A					1
#define J					11
#define Q					12
#define K					0

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
int getCardNum(int cardnum) {
	
	int shape;
	int num;
	int cardnum;
	
	char *Shape[4] = { "Hart", "Dia", "Spade", "Club" };  //for print shape name
	char *KAJQ[13] = { "K", "A", "x", "x", "x", "x", "x", "x", "x", "x", "x", "J", "Q" }; //for print letter K,A,J,Q
	
	int CardNum[4][13] = {
	{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10},
	{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10},
	{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10},
	{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10}};
	
	printf("input the number of card shape ( hart=0, dia=1, spade=2, club=3 ) : ");
	scanf("%d",&shape);
	printf("input card number (K=0,A=1,J=11,Q=12) : ");
	scanf("%d",&num);
	
	if(num>=2 && num <= 10)
		printf("the actual card number of %s%d = %d", Shape[shape],num,num );
	else if(num==0||num==11||num==12)
		printf("the actual card number of %s%s = %d", Shape[shape], KAJQ[num], CardNum[shape][num] );
	else if(num==1)
		printf("the actual card number of %sA = 1 or 11", Shape[shape]);
		

		
	return 0;
}

//print the card information (e.g. DiaA)
void printCard(int shape, int num) {
	
	int shape;
	int num;
	int cardnum;
	
	char *Shape[4] = { "hart", "dia", "spade", "club" };
	char *KAJQ[13] = { "K", "A", "x", "x", "x", "x", "x", "x", "x", "x", "x", "J", "Q" };
	
	int CardNum[4][13] = {
	{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10},
	{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10},
	{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10},
	{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10}};
	
	printf("input the number of card shape ( hart=0, dia=1, spade=2, club=3 ) : ");
	scanf("%d",&shape);
	printf("input card number (K=0,A=1,J=11,Q=12) : ");
	scanf("%d",&num);
	
	if(num>=2 && num <= 10)
		printf("the card information is %s%d", Shape[shape],num );
	else if(num==0||num==11||num==12)
		printf("the card information is %s%s", Shape[shape], KAJQ[num]);
	else if(num==1)
		printf("the card information is %sA", Shape[shape]);
		

	return 0;
	
	
}


//card array controllers -------------------------------

//mix the card sets and put in the array
int mixCardTray(void) {
	
	srand(time(NULL));
	
	int i,j;
	
	int CardNum[4][13] = {
	{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10},
	{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10},
	{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10},
	{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10}};
	
	for(i=0;i<52;i++)
	{
		int shape[52];
		int num[52];
		shape[i] = rand()%4;
		num[i] = rand()%13;
		
		for(j=0;j<i;j++)
		{
			if( shape[i]==shape[j] & num[i]==num[j] )
				i--;
			else
				break;
		}
		
		printf("CardNum[%d][%d] \n",shape[i],num[i]);	
	}	
	
	return 0;

}

//get one card from the tray
int pullCard(void) {
	
	srand(time(NULL));

	int CardNum[4][13] = {
	{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10},
	{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10},
	{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10},
	{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10}};
	
	int shape;
	int num;
	
	shape = rand()%4;
	num = rand()%13;
	
	printf("CardNum[%d][%d]",shape,num);
	
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
	
	int yourbetting;
	
	printf("-------BETTING STEP--------\n");
	printf(" -> your betting ( total :$%d) : ", N_DOLLAR );
	scanf("%d",&yourbetting);
	
	while(yourbetting>N_DOLLAR)
	{
		printf(" -> you only have $%d! bet again\n", N_DOLLAR);
		printf(" -> your betting ( total :$%d ) : ", N_DOLLAR );
		scanf("%d",&yourbetting);
	}
	
	return 0;
}


//offering initial 2 cards
void offerCards(void) {
	int i;
	//1. give two card for each players
	for (i=0;i<n_user;i++)
	{
		cardhold[i][0] = pullCard();
		cardhold[i][1] = pullCard();
	}
	//2. give two card for the operator
	cardhold[n_user][0] = pullCard();
	cardhold[n_user][1] = pullCard();
	
	return;
}

//print initial card status
void printCardInitialStatus(void) {
	
}

int getAction(void) {
	
}


void printUserCardStatus(int user, int cardcnt) {
	int i;
	
	printf("   -> card : ");
	for (i=0;i<cardcnt;i++)
		printCard(cardhold[user][i]);
	printf("\t ::: ");
}




// calculate the card sum and see if : 1. under 21, 2. over 21, 3. blackjack
int calcStepResult() {
	
}

int checkResult() {
	
}

int checkWinner() {
	
}



int main(int argc, char *argv[]) {
	int roundIndex = 0;
	int max_user;
	int i;
	
	srand((unsigned)time(NULL));
	
	//set the number of players
	configUser();


	//Game initialization --------
	//1. players' dollar
	
	//2. card tray
	mixCardTray();



	//Game start --------
	do {
		
		betDollar();
		offerCards(); //1. give cards to all the players
		
		printCardInitialStatus();
		printf("\n------------------ GAME start --------------------------\n");
		
		//each player's turn
		for () //each player
		{
			while () //do until the player dies or player says stop
			{
				//print current card status printUserCardStatus();
				//check the card status ::: calcStepResult()
				//GO? STOP? ::: getAction()
				//check if the turn ends or not
			}
		}
		
		//result
		checkResult();
	} while (gameEnd == 0);
	
	checkWinner();
	
	
	return 0;
}
