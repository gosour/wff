#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int ** return_mask(int n)
{
	int no = pow(2,n);
	int **list = malloc(n);
	unsigned int level, i, temp;

	for(i=0;i<n;i++){
		list[i]	= malloc(no);
	}

    for (level = 0; level < n; level++){
		printf("YOYO:\t");
        for (i = (1<<n)-1; i>=0; i--)   // we'll always output 2**n bits
			temp = i;
			printf("%d",(temp >> level) & 1);
            list[level][i] = (temp >> level) & 1;
		printf("\n");
    }
	
	return list;
}

void error()
{
	printf("SHUT DOWN EVERYTHING\n");
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
		printf("Expect\n");
		error();
	}
}

void P(char **);

void E(char **input)
{
	P(input);
	while(next(input) == '^'){
		consume(input);
		P(input);
	}
}

void P(char **input)
{
	char nxt = next(input);
	printf("NoW: %c\n",nxt);
	if (nxt == 'p'){
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
	else{
		printf("P\n");
		error();
	}
}	

void Erecognizer(char **input){
	E(input);
	expect(input,'*');
}



int main()
{
	int n = 4;
	int i,j;
	int **list;
	list = 	return_mask(n);
	for(i=0;i<n;i++){
   		for(j=0;j<pow(2,n);j++)
			printf("%d",list[i][j]);
		printf("\n");
	}
}
