#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define ISBINARY(a) a =='^' || a=='v' || a=='=' || a=='>'

struct node{
	char val;
	struct node *left;
	struct node *right;
};

typedef struct node *Tree;


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

Tree mkTree(char root, Tree left, Tree right)
{
	Tree t = malloc(sizeof(struct node));
	t->val = root;
	t->left = left;
	t->right = right;
	return t;
}

void printTree(Tree t)
{
	if(t!=NULL){
		printf("%c",t->val);
		printTree(t->left);
		printTree(t->right);
	}
}

Tree P(char **);

Tree E(char **input)
{
	Tree tsub = NULL;
	Tree tmain = NULL;

	char nxt;

	tsub = P(input);
	while( ISBINARY(next(input))  ){
		nxt = next(input);
		consume(input);
		tmain = mkTree(nxt, tsub, E(input));
	}
	if (tmain)
		return tmain;
	else
		return tsub;
}

Tree P(char **input)
{
	char nxt = next(input);
	Tree temp;

	if (nxt >= 'A' && nxt <='Z'){
		consume(input);
		return mkTree(nxt,NULL, NULL);
	}
	else if (nxt == '('){
		consume(input);
		temp = E(input);
		expect(input, ')');
		return temp;
	}
	else if (nxt == '~'){
		consume(input);
		return mkTree(nxt,P(input), NULL);
	}
	else
		error();
	
}	

struct column{
	char varname;
	int *bitpatt;
};

typedef struct column Column;

Column* return_mask(char *input, int *varc)
{
	int count = 0;
	int no; 
	int **list;
	Column *c;
	int level, i,j, temp, alphabet;
	int array[26] = {};

	/*Creating an array of letters and counting which are present*/
	while(input[i])
	{
		if (input[i] >='A' && input[i] <='Z')
			array[input[i]-'A']++;
		i++;
	}

	for(i=0;i<26;i++)
		if (array[i])
			count++;

	no = pow(2,count);
	list = malloc(count);
	c = malloc(sizeof(Column)*count);

	for(i=0,j=0;i<26;i++){
		if(array[i]){
			c[j].varname = 'A'+ i;
			c[j].bitpatt = (int *)malloc(sizeof(int)*pow(2,count));
			j++;
		}
	}


	for (level = 0; level < count; level++){
		for (i = (1<<count)-1,j=0; i>=0; i--){   // we'll always output 2**n bits
			temp = i;
			printf("%d",(temp >> level) & 1);
			temp = i;
			c[level].bitpatt[j++] = (temp >> level) & 1;
		}
		printf("\n");

	}	

	*varc =  count;	
	return c;
}


void Erecognizer(char **input){
	Tree myTree = NULL;
	Column *myCol;
	int varc = 0;
	int i,j;

	myCol = return_mask(*input,&varc);
	printf("No of columns: %d\n",varc);
	for(i=0;i<varc; i++){
		printf("%c:\t",myCol[i].varname);
		for(j=0;j<pow(2,varc);j++)
			printf("%d",myCol[i].bitpatt[j]);
		putchar('\n');
	}
	myTree = E(input);
	expect(input,'*');
	printTree(myTree);
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

