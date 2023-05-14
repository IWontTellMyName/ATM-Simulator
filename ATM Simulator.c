# include <stdio.h>
# include <string.h>
#include <time.h>

int pos=0;
int prime(long int n)
{
	for(int j=2; j<n/2; j++)
		if(n%j==0)
			return 1;
	return 0;
}

int viewBalance()
{
	FILE *file=fopen("Database.csv","r");
	fseek(file, pos-7, SEEK_SET);
	int balance;
	fscanf(file, "%d", &balance);
	printf("Current balance is ₹%d", balance);
	fclose(file);
	return 0;
}

int depositWithdraw(int sign)
{
	int balance, change;
	FILE *file=fopen("Database.csv","r+");
	fseek(file, pos-7, SEEK_SET);
	fscanf(file, "%d", &balance);
	printf("Enter the amount : ");
	scanf("%d", &change);
	balance+=sign*change;
	fseek(file, pos-7, SEEK_SET);
	fprintf(file, "%06d", balance);
	printf("Updated Balance : ₹ %06d", balance);
	fclose(file);
	return 0;
}

int changePin(long int card)
{
	FILE *prime=fopen("Prime.csv", "r");
	FILE *database=fopen("Database.csv", "r");
	srandom(time(NULL));
	char line[18];
	int r=random()%1061;
	fseek(prime, r*5, SEEK_SET);
	fscanf(prime, "%d", &r);
	card*=r;
	
	fseek(database, pos-18, SEEK_SET);
	sprintf(line, "%010ld", card);
	fprintf(database, "%s", line);
	fclose(prime);
	fclose(database);
	printf("PIN updated to %d", r);
	return 0;
}

int main()
{
	//Card detail valuation
	long int pin;
	char line[20], product[15], *t;
	printf("Enter card number (without spaces) : ");
	gets(line);
	strncpy(product, line+0, 6);
	if(strcmp(product,"763425") !=0)
	{
		printf("Card not issued by this bank");
		return 1;
	}
	long int card=atoi(line+6);
	printf("Enter pin : ");
	scanf("%ld", &pin);
	if (prime(card)==1 || prime(pin)==1)
	{
		printf("Invalid PIN/card number");
		return 1;
	}
	pin*=card;
	sprintf(product, "%010ld", pin);
	
	//Searching for card in database
	FILE *file=fopen("Database.csv","r");
	while(fgets(line, 100, file))
	{
		t=strtok(line, ",");
		if(strcmp(product, t)==0)
		{
			pos = ftell(file);
			break;
		}
	}
	if (pos==0)
	{
		printf("Invalid PIN/Card number");
		return 1;
	}
	fclose(file);
	
	//Actions
	printf("Enter 1 to view balance\n");
	printf("Enter 2 to make a deposit\n");
	printf("Enter 3 to make a withdrawal\n");
	printf("Enter 4 to change your PIN\n");
	printf("Enter your choice : ");
	int ch;
	scanf("%d", &ch);
	
	if (ch==1)
		viewBalance();
	else if (ch==2)
		depositWithdraw(1);
	else if (ch==3)
		depositWithdraw(-1);
	else if (ch==4)
		changePin(card);
	else
		printf ("Invalid choice");
	
	printf("\nThanks for using ATM Simulator made by Snehal");
	return 0;
}