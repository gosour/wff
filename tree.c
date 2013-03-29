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
			c[level].bitpatt[j++] = (temp >> level) & 1;
		}

	}	

	*varc =  count;	
	return c;
}

int evalbin(char op, int op1, int op2){
	switch(op){
		case '^': return op1 && op2;
		case 'v': return op1 || op2;
		case '=': return (op1 && op2) || (!op1 && !op2);
		case '>': return (op1 && op2) || (!op1 && !op2) || (!op1 && op2);
		}
}

int evalun(char op, int op1){
	return !op1;
}

void printNodesUn(Tree);
void printNodesBin(Tree t)
{
	if(t!=NULL){
		/*putchar('(');*/
		if(t->left && t->left->val == '~')
			printNodesUn(t->left);
		else
			printNodesBin(t->left);
		printf("%c",(t->val));
		if(t->right && t->right->val== '~')
			printNodesUn(t->right);
		else
			printNodesBin(t->right);
		/*putchar(')');*/
	}
}


void printNodesUn(Tree t)
{
	if(t!=NULL){
		printf("%c",(t->val));
		if(t->left && t->left->val == '~')
			printNodesUn(t->left);
		else
			printNodesBin(t->left);
	}
}

void printTreeHead(Tree t)
{
	if(t!=NULL){
		/*printf("curr: %c\n",t->val);*/
		if(t->val >= 'A' && t->val <='Z')
			printf("%c",(t->val));
		
		printTreeHead(t->left);
		printTreeHead(t->right);
		if(ISBINARY(t->val))
			printNodesBin(t);
		else if(t->val == '~')
			printNodesUn(t);
		printf("%c",',');
	}
}

int getval(char varname, Column *c, int pos,int count)
{
	int i;
	for(i=0;i<count;i++){
		if(c[i].varname == varname)
			return c[i].bitpatt[pos];
	}
	error();
}

int evalTree(Tree t,Column *c, int pos, int count){
	if(ISBINARY(t->val))
		return evalbin(t->val, evalTree(t->left,c,pos,count), evalTree(t->right,c,pos,count));
	
	if(t->val == '~')
		return evalun(t->val, evalTree(t->left,c,pos,count));
	
	else
		return getval(t->val,c,pos,count); /*value corrosponding to current variable*/
}

void printTree(Tree t,Column *c, int pos, int count)
{
	if(t!=NULL){
		/*printf("curr: %c\n",t->val);*/
		if(t->val >= 'A' && t->val <='Z')
			printf("%d",getval((t->val),c,pos,count));
		
		printTree(t->left,c,pos,count);
		printTree(t->right,c,pos,count);
		if(ISBINARY(t->val))
			printf("%d",evalTree(t,c,pos,count));
		else if(t->val == '~')
			printf("%d",evalTree(t,c,pos,count));
		printf("%c",',');
	}
}

void totalEval(Tree t, Column *c, int count)
{
	int pos=0;
	printTreeHead(t);
	putchar('\n');
	while(pos<pow(2,count)){
		/*printf("%d",evalTree(t,c,pos, count));*/
		printTree(t,c, pos, count);
		putchar('\n');
		pos++;
	}
}

void Erecognizer(char **input){
	Tree myTree = NULL;
	Column *myCol;
	int varc = 0;
	int i,j;

	myCol = return_mask(*input,&varc);
	myTree = E(input);
	expect(input,'*');
	totalEval(myTree,myCol,varc);

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
	/*printf("This is a wff\n");*/
	return EXIT_SUCCESS;
}

