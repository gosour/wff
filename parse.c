#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ISBINARY(a) a =='^' || a=='v' || a=='=' || a=='>'

void error()
{
	printf("Syntax Error\n");
	exit(0);
}
char next(char **input)
{
	if (**input)
		return **input;
	else{
		printf("parse error\n");
		exit(0);
	}
}

void consume(char **input){
	if (**input)
		*((*input)++);
	else{
		printf("Parse error\n");
		exit(0);
	}
}

int expect(char **input, char a)
{
	if ( next(input) == a){
		consume(input);
		return 1;
	}
	else{
		printf("Expected %c but not found\n",a);
		error();
	}
}

void P(char **);

void E(char **input)
{
	P(input);
	while( ISBINARY(next(input))  ){
		consume(input);
		P(input);
	}
}

void P(char **input)
{
	char nxt = next(input);
	if (nxt >= 'A' && nxt <='Z'){
		consume(input);
	}
	else if (nxt == '('){
		consume(input);
		E(input);
		expect(input, ')');
	}
	else if (nxt == '~'){
		consume(input);
		P(input);
	}
	else
		error();
	
}	

void Erecognizer(char **input){
	E(input);
	expect(input,'*');
}


int main(int argc, char **argv)
{	
	int size = 0, i;
	char *input;
	if(argc<2){
		printf("Usage: parse [expression]\n");
		return EXIT_FAILURE;
	}
	for(i=0;argv[1][i];i++)
		size++;
	
	input = malloc(sizeof(char) * (size+1));
	input = strcpy(input,argv[1]);
	input = strcat(input,"*");	
	Erecognizer(&input); 
	printf("This is a wff\n");
	return EXIT_SUCCESS;
}

