#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define ISBINARY(a) a =='^' || a=='v' || a=='=' || a=='>'
#define BOOL(n) n? 'T':'F'

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

void printNodesUn(Tree,FILE *);
void printNodesBin(Tree t,FILE *buff)
{
	if(t!=NULL){
		if(t->left && t->left->val == '~')
			printNodesUn(t->left,buff);
		else
			printNodesBin(t->left,buff);
		fprintf(buff,"%c",(t->val));
		if(t->right && t->right->val== '~')
			printNodesUn(t->right,buff);
		else
			printNodesBin(t->right,buff);
	}
}


void printNodesUn(Tree t,FILE *buff)
{
	if(t!=NULL){
		fprintf(buff,"%c",(t->val));
		if(t->left && t->left->val == '~')
			printNodesUn(t->left,buff);
		else
			printNodesBin(t->left,buff);
	}
}

void printTreeHead(Tree t,int commac,FILE *buff)
{

	if(t!=NULL){
		if(t->val >= 'A' && t->val <='Z'){
			if(commac)
				fprintf(buff,",");
			fprintf(buff,"%c",(t->val));
		}
		printTreeHead(t->left,commac++,buff);
		printTreeHead(t->right,commac++,buff);
		if(ISBINARY(t->val)){
			fprintf(buff,",");
			printNodesBin(t,buff);
		}
		else if(t->val == '~'){
			fprintf(buff,",");
			printNodesUn(t,buff);
		}
		commac++;
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

void printTree(Tree t,Column *c, int pos, int count,int commac,FILE *buff)
{
	if(t!=NULL){
		if(t->val >= 'A' && t->val <='Z'){
			if(commac)
				fprintf(buff,",");
			fprintf(buff,"%c",BOOL(getval((t->val),c,pos,count)));
		}
		
		printTree(t->left,c,pos,count,commac++,buff);
		printTree(t->right,c,pos,count,commac++,buff);
		if(ISBINARY(t->val))
			fprintf(buff,",%c",BOOL(evalTree(t,c,pos,count)));
		else if(t->val == '~')
			fprintf(buff,",%c",BOOL(evalTree(t,c,pos,count)));
	}
}

int * totalEval(Tree t, Column *c, int count,int print_flag,int file_flag,FILE *buff)
{
	int pos=0;
	int *final = malloc(sizeof(int) * pow(2,count));
	if (print_flag){
		printTreeHead(t,0,stdout);
		fprintf(stdout,"\n");
	}
	if(file_flag){
		printTreeHead(t,0,buff);
		fprintf(buff,"\n");
	}	
	
	while(pos<pow(2,count)){
		final[pos] = evalTree(t,c,pos, count);
		if(print_flag){
			printTree(t,c, pos, count,0,stdout);
			fprintf(stdout,"\n");
		}

		if(file_flag){
			printTree(t,c, pos, count,0,buff);
			fprintf(buff,"\n");
		}
		pos++;
	}
	return final;
}

void checkResult(int *result,int size)
{
	int i=0;
	int fflag = 0;
	int tflag = 0;
	for(i=0;i<size;i++){
		if(result[i] == 0)
			fflag++;
		if(result[i] == 1)
			tflag++;
	}
	if(fflag && tflag)
		printf("Invalid & Consistent\n");
	if(!fflag)
		printf("Valid\n");
	if(!tflag)
		printf("Inconsistent\n");

}

void Erecognizer(char **input,int print_flag,int file_flag,FILE *buff){
	Tree myTree = NULL;
	Column *myCol;
	int varc = 0;
	int i,j;
	int *result;

	myCol = return_mask(*input,&varc);
	myTree = E(input);
	expect(input,'*');
	/*checkResult(totalEval(myTree,myCol,varc,print_flag),pow(2,varc));*/
	result = totalEval(myTree,myCol,varc,print_flag,file_flag,buff);
	if(!print_flag)
		checkResult(result, pow(2,varc));

}

void print_help()
{
	printf( "Well Formed Formula checker\n"
			"Allowed variable names = [A-Z]\n"
			"Logical connectives:\n"
			"^ : and binary operator\n"
			"v : or binary operator\n"
			"~ : not unary operator\n"
			"= : equivalent binary operator. (<->)\n"
			"> : implies binary operator. (->) \n"
			"Valid wff: Evaluates to True only\n"
			"Inconsistent wff: Evaluates to False only\n");
}


int main(int argc, char **argv)
{	
	int size = 0, i;
	int print_flag = 0;
	int input_flag = 0;
	int file_flag = 0;
	int opt;
	char *input;
	char *filename;
	FILE *buff = stdout;

	while((opt = getopt(argc,argv,"hpe:o:")) != -1){
		switch (opt) {
			case 'h': print_help();
					  break;
			case 'p': print_flag = 1;
					  break;
			case 'e': input_flag = 1;
			          for(i=0;optarg[i];i++)
					  	size++;
					  input = malloc(sizeof(char) * (size+1));
				      input = strcpy(input,optarg);
 				      input = strcat(input,"*");	
					  break;
			case 'o': file_flag = 1;
					  filename = malloc(sizeof(char) * (strlen(optarg)+1));
					  filename = strcpy(filename,optarg);
					  break;
			default:  fprintf(stderr, "Usage: %s -e expression [-p] [-h] [-o filename]\n",
							  		argv[0]);
					  exit(EXIT_FAILURE);
		}
	}
	
	if(input_flag){
			if(file_flag){
				buff = fopen(filename,"w");
			}
			Erecognizer(&input,print_flag,file_flag,buff); 
	}

	return EXIT_SUCCESS;
}

