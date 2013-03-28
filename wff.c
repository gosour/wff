//Check if an expression is a wff or not

#include<stdio.h>
#include<stdlib.h>

int check_wff(char *);

int main(int argc, char **argv)
{
	char expr[100];

	if (argc != 2){
		printf("USAGE: one expression as an argument without space\n");
		return EXIT_FAILURE;
	}
	else{
		return check_wff(expr);
	}
}
