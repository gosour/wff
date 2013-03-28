#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

void Erecognizer(char **input){
	Tree myTree = NULL;

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

