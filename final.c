/*
	Libraries
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
	Constants
*/
//defining strings and table sizes
#define EXPRESSION_SIZE 1000
#define TOKEN_SIZE 100
#define TOKEN_TYPE_SIZE 12
#define TOKEN_TABLE_SIZE 500
#define ERROR_MSG_SIZE 60
#define STACK_SIZE 500

//defining types
#define TYPE_ID "Identifiant"
#define TYPE_KEYWORD "Mot cle"
#define TYPE_OPERATOR "Operateur"
#define TYPE_LITTERAL "Litteraux"
#define TYPE_ERROR "Erreur"

//defining keywords, Operators, id and litterals characters to push and pop from stack
#define TYPE 't'
#define ID 'i'
#define LIT 'c'
#define IF 'f'
#define ELSE 'e'
#define WHILE 'w'
#define DO 'd'
#define RETURN 'r'
#define CS 's'

/*
	First part is used to extract tokens and detecting token type
*/
//creating a struct to store token, his type and position
typedef struct {
    char token[TOKEN_SIZE];
    char type[TOKEN_TYPE_SIZE];
    int position;
} Token;

//variables to store error if exists
char error_msg[ERROR_MSG_SIZE];

//function to save error
void save_error(char *msg){
	strcpy(error_msg,msg);
}

//function to handle error
void handle_error(char *message, char *token, int position){
	printf("Erreur %s : %s a la position %d.\n",token,message,position);
}

//function to show tokens, type and position
void print_report(Token *tokens){
	printf("\n\n=======================Analyse report :=======================\n");	
	for(int i = 0 ; strcmp(tokens[i].token,"$") != 0  ; i++){
		printf("Jeton : %s ,Type : %s, Position: %d\n",tokens[i].token,tokens[i].type,tokens[i].position);
		if(strcmp(tokens[i].type,TYPE_ERROR) == 0) {
			handle_error(error_msg,tokens[i].token,tokens[i].position);break;
		}
	}
}

//check if character is operator
int is_operator(char c){
	return c == ',' || c == '>' || c == '<' || c == '=' || c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '!' || c == '(' || c == ')' || c == '{' || c == '}' || c == ';'|| c == '&'|| c == '|' ? 1 : 0;
}

//check if character is alphabetic
int is_alphabetic(char c){
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ? 1 : 0;
}

//check if character is numeric
int is_numeric(char c){
	return (c >= '0' && c <= '9') ? 1 : 0;
}

//check if character is space
int is_space(char c){
	return c == ' ' || c == '\n' || c == '\t' ? 1 : 0;
}

//check if token is keyword
int is_keyword(char *keyword){
	return strcmp(keyword,"int") == 0 ||strcmp(keyword,"void") == 0 ||strcmp(keyword,"float") == 0 || strcmp(keyword,"char") == 0 
		||strcmp(keyword,"if") == 0 ||strcmp(keyword,"else") == 0 ||strcmp(keyword,"while") == 0 ||strcmp(keyword,"return") == 0 ||strcmp(keyword,"do") == 0 ? 1 : 0;
}

//check if token is id
int is_id(char *word){
	int tra_table[4][5] = {
		{0, 1 , 2, 3, 3},
		{1, 1 , 2, 2, 3},
		{2, 2 , 2, 2, 3},
		{3, 3 , 3, 3, 3}
	};
	int current_state = 0;
	int i = 0,symbol_type;
	while(current_state != 3 && word[i] !='\0'){
		if(word[i] == '_') symbol_type = 1;
		else if(is_alphabetic(word[i])) symbol_type = 2;
		else if(is_numeric(word[i])) symbol_type = 3;
		else symbol_type = 4;
		current_state = tra_table[current_state][symbol_type];
		i++;
	}
	return current_state == 2 ? 1 : 0;
}

//check if token is int
int is_int(char *word){
	int tra_table[4][4] = {
		{0, 1 , 2, 3},
		{1, 3 , 2, 3},
		{2, 3 , 2, 3},
		{3, 3 , 3, 3}
	};
	int current_state = 0;
	int i = 0,symbol_type;
	while(current_state != 3 && word[i] !='\0'){
		if(word[i] == '-') symbol_type = 1;
		else if(is_numeric(word[i])) symbol_type = 2;
		else symbol_type = 3;
		current_state = tra_table[current_state][symbol_type];
		i++;
	}
	return current_state == 2 ? 1 : 0;
}

//check if token is float
int is_float(char *word){
	int tra_table[6][5] = {
		{0, 1 , 2, 5, 5},
		{1, 5 , 2, 5, 5},
		{2, 5 , 2, 3, 5},
		{3, 5 , 4, 5, 5},
		{4, 5 , 4, 5, 5},
		{5, 5 , 5, 5, 5}
	};
	int current_state = 0;
	int i = 0,symbol_type;
	while(current_state != 5 && word[i] !='\0'){
		if(word[i] == '-') symbol_type = 1;
		else if(is_numeric(word[i])) symbol_type = 2;
		else if(word[i] == '.') symbol_type = 3;
		else symbol_type = 4;
		current_state = tra_table[current_state][symbol_type];
		i++;
	}
	int _ = 3;
	printf("%f",_);
	return current_state == 2 || current_state == 4 ? 1 : 0;
}

//check if token is char
int is_char(char *word){
	int tra_table[5][4] = {
		{0, 1 , 4, 4},
		{1, 4 , 2, 4},
		{2, 3 , 4, 4},
		{3, 4 , 4, 4},
		{4, 4 , 4, 4}
	};
	int current_state = 0;
	int i = 0,symbol_type;
	while(current_state != 4 && word[i] !='\0'){
		if(word[i] == '\'') symbol_type = 1;
		else if(word[i] >= 0 && word[i] <= 127) symbol_type = 2;
		else symbol_type = 3;
		current_state = tra_table[current_state][symbol_type];
		i++;
	}
	return current_state == 3 ? 1 : 0;
}

//check if token is string
int is_str(char *word){
	int tra_table[5][4] = {
		{0, 1 , 4, 4},
		{1, 4 , 2, 4},
		{2, 3 , 2, 4},
		{3, 4 , 4, 4},
		{4, 4 , 4, 4}
	};
	int current_state = 0;
	int i = 0,symbol_type;
	while(current_state != 4 && word[i] !='\0'){
		if(word[i] == 34) symbol_type = 1;
		else if(word[i] >= 0 && word[i] <= 127) symbol_type = 2;
		else symbol_type = 3;
		current_state = tra_table[current_state][symbol_type];
		i++;
	}
	return current_state == 3 ? 1 : 0;
}

//check if token represents type
int is_type(char *word){
	return strcmp(word,"int") == 0 || strcmp(word,"float") == 0 || strcmp(word,"void") == 0 ||  strcmp(word,"char") == 0 ? 1 : 0;
}

//Extract tokens
Token *tokeniser(char *expression){
	Token *tokens = malloc(TOKEN_TABLE_SIZE * sizeof(Token));
	int i = 0,j = 0;
	while(expression[i] != '\0'){
		if(expression[i] == '>' ||expression[i] == '<' ||expression[i] == '=' ||expression[i] == '!'){
			if(expression[i+1] == '='){
				char word[3];
				word[0] = expression[i];
				word[1] = expression[i+1];
				word[2] = '\0';
				Token t;strcpy(t.token,word);strcpy(t.type,TYPE_OPERATOR);t.position = i+=2;tokens[j++] = t;
			}else{
				char word[2];
				word[0] = expression[i];
				word[1] = '\0';
				Token t;strcpy(t.token,word);strcpy(t.type,TYPE_OPERATOR);t.position = i++;tokens[j++] = t;
			}
		}else if(is_operator(expression[i])){
			char word[2];
			word[0] = expression[i];
			word[1] = '\0';
			Token t;strcpy(t.token,word);strcpy(t.type,TYPE_OPERATOR);t.position = i++;tokens[j++] = t;
		}else if(is_alphabetic(expression[i]) || expression[i] == '_'){
			char word[TOKEN_SIZE];
			int  k = 0;
			while(is_alphabetic(expression[i]) || is_numeric(expression[i]) || expression[i] ==  '_'){
				word[k++] = expression[i++];
			}
			word[k] = '\0';
			if(is_keyword(word)){
				Token t;strcpy(t.token,word);strcpy(t.type,TYPE_KEYWORD);t.position = i;tokens[j++] = t;
			}else if(is_id(word)){
				Token t;strcpy(t.token,word);strcpy(t.type,TYPE_ID);t.position = i;tokens[j++] = t;
			}else{
				Token t;strcpy(t.token,word);strcpy(t.type,TYPE_ERROR);t.position = i;tokens[j++] = t;
				save_error("Mot cle invalide");
				break;
			}
		}else if(is_numeric(expression[i])){
			char word[TOKEN_SIZE];
			int  k = 0;
			while( is_numeric(expression[i]) || expression[i] ==  '.'){
				word[k++] = expression[i++];
			}
			word[k] = '\0';
			if(is_float(word) || is_int(word)){
				Token t;strcpy(t.token,word);strcpy(t.type,TYPE_LITTERAL);t.position = i;tokens[j++] = t;
			}else{
				Token t;strcpy(t.token,word);strcpy(t.type,TYPE_ERROR);t.position = i;tokens[j++] = t;
				save_error("Literal numerique invalide");
				break;
			}
		}else if(expression[i] == '\''){
			char word[TOKEN_SIZE];
			word[0] = '\'';
			int  k = 1;i++;
			while( expression[i] != '\'' && expression[i] != '\0'){
				word[k++] = expression[i++];
			}
			word[k++] = '\'';i++;
			word[k] = '\0';
			if(expression[i] == '\0' || !is_char(word) ){
				if(expression[i] == '\0') word[--k] = '\0';
				Token t;strcpy(t.token,word);strcpy(t.type,TYPE_ERROR);t.position = i;tokens[j++] = t;
				save_error("Literal charactere invalide");
				break;
			}else{
				Token t;strcpy(t.token,word);strcpy(t.type,TYPE_LITTERAL);t.position = i;tokens[j++] = t;
			}
		}else if(expression[i] == '"'){
			char word[TOKEN_SIZE];
			word[0] = '"';
			int  k = 1;i++;
			while( expression[i] != '"' && expression[i] != '\0'){
				word[k++] = expression[i++];
			}
			word[k++] = '"';i++;
			word[k] = '\0';
			if(expression[i] == '\0' || !is_str(word) ){
				word[--k] = '\0';
				Token t;strcpy(t.token,word);strcpy(t.type,TYPE_ERROR);t.position = i;tokens[j++] = t;
				save_error("Literal chaine de charactere invalide");
				break;
			}else{
				Token t;strcpy(t.token,word);strcpy(t.type,TYPE_LITTERAL);t.position = i;tokens[j++] = t;
			}
		}else if(is_space(expression[i])){
			i++;
		}else{
			char word[2];
			word[0] = expression[i];
			word[1] = '\0';
			Token t;strcpy(t.token,word);strcpy(t.type,TYPE_ERROR);t.position = i;tokens[j++] = t;
			save_error("Caractere non reconu");
			break;
		}
	}
	Token t;strcpy(t.token,"$");strcpy(t.type,"$");t.position = i;tokens[j++] = t;
	return tokens;
}

/*
	Second part is used to analyse syntax
*/
//check if token is comparing operator
int is_comparing_symbol(char *word){
	return strcmp(word,">") == 0 ||strcmp(word,"<") == 0 ||strcmp(word,">=") == 0 ||strcmp(word,"<=") == 0 ||strcmp(word,"==") == 0 ||strcmp(word,"!=") == 0 ? 1 : 0;
}

//defining stack as struct
typedef struct {
    char elements[STACK_SIZE];
    int sommet;
} Stack;

//initialise stack 
void init_tack(Stack *stack) {
    stack->sommet = -1;
}

//check if stack empty
int is_Empty(Stack *stack){
	return stack->sommet == -1;
}

//push to stack
void push(Stack *stack, char element) {
    stack->elements[++stack->sommet] = element;
}

//pop from stack
char pop(Stack *stack) {
    return stack->elements[stack->sommet--];
}

//C program transition table
int tra_tab[13][21] = {
	{ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 2},
	{ 3, -1, -1, -1,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 4},
	{ 3, -1, -1, -1,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  5, 6},
	{ 7,  8,  8,  8, -1, -1, 12, 11,  9, -1, 10, 10, -1, -1, -1, -1, -1, -1, -1,  5,12},
	{13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1},
	{-1, 14, 14, 14, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1},
	{-1, -1, -1, -1, 20, -1, -1, -1, -1, -1, -1, -1, 15, 16, 17, 18, 19, 20, 20, -1,20},
	{-1, 21, 22, 23, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1},
	{-1, -1, -1, -1, -1, -1, -1, -1, 24, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1},
	{26, 26, 26, 26, -1, -1, 26, 26, 26, 25, 26, 26, -1, -1, -1, -1, -1, -1, -1, -1,26},
	{-1, -1, -1, -1, -1, 28, -1, -1, 27, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 29, 30, -1, -1, -1, -1, -1, -1, -1, -1,-1},
	{-1, 31, 31, 31, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-1},
};

//transition table columns definition
int get_column(Token token){
	if(strcmp(token.type,TYPE_ID) == 0) return 1;
	if(strcmp(token.type,TYPE_KEYWORD) == 0){
		if(strcmp(token.token,"return") == 0) return 7;
		if(strcmp(token.token,"if") == 0) return 8;
		if(strcmp(token.token,"else") == 0) return 9;
		if(strcmp(token.token,"while") == 0) return 10;
		if(strcmp(token.token,"do") == 0) return 11;
		return 0;
	}
	if(strcmp(token.type,TYPE_OPERATOR) == 0){
		if(strcmp(token.token,"(") == 0) return 3;
		if(strcmp(token.token,")") == 0) return 4;
		if(strcmp(token.token,"{") == 0) return 5;
		if(strcmp(token.token,"}") == 0) return 6;
		if(strcmp(token.token,"+") == 0) return 12;
		if(strcmp(token.token,"-") == 0) return 13;
		if(strcmp(token.token,"*") == 0) return 14;
		if(strcmp(token.token,"/") == 0) return 15;
		if(strcmp(token.token,"%") == 0) return 16;
		if(strcmp(token.token,";") == 0) return 17;
		if(is_comparing_symbol(token.token)) return 18;
		if(strcmp(token.token,",") == 0) return 19;
	}
	if(strcmp(token.type,TYPE_LITTERAL) == 0) return 2;
	if(strcmp(token.token,"$") == 0) return 20;
	return -1;
}

//transition table line definition
int get_line(char c){
	switch(c){
		case 'S': return 0;
		case 'P': return 1;
		case 'F': return 2;
		case 'B': return 3;
		case 'D': return 4;
		case 'E': return 5;
		case 'X': return 6;
		case 'Y': return 7;
		case 'C': return 8;
		case 'A': return 9; 
		case 'T': return 10; 
		case 'L': return 11; 
		case 'Q': return 12; 
		default: return -1;
	}
}

//pushing to stack
void take_action(Stack *stack, int action){
	switch(action){
		case 1: push(stack, 'S'); push(stack,'}'); push(stack,'B'); push(stack,'{'); push(stack,')'); push(stack,'P'); push(stack,'('); push(stack,ID); push(stack, TYPE); break; // S -> type id(P){B}S
		case 2: break; // S -> #
		case 3: push(stack, 'F'); push(stack,ID); push(stack,TYPE); break; //P -> type idF
		case 4: break; // P -> #
		case 5: push(stack,'F'); push(stack,ID); push(stack,TYPE); push(stack,','); break; // F -> ,type idF
		case 6: break; // F -> #
		case 7: push(stack, 'B'); push(stack, 'D'); break; //B -> DB  
		case 9: push(stack, 'B'); push(stack, 'C'); break; //B -> CB
		case 10: push(stack, 'B'); push(stack, 'L'); break; //B -> LB
		case 11: push(stack, ';'); push(stack, 'E'); push(stack, RETURN); break; // B -> return E;
		case 12: break; // B -> #
		case 13: push(stack,';'); push(stack,'E'); push(stack,'='); push(stack,ID); push(stack,TYPE); break; // D -> type id = E;
		case 14: push(stack,'X'); push(stack,'Y'); break; // E -> YX
		case 15: push(stack,'X'); push(stack,'Y'); push(stack,'+'); break; // X -> +YX
		case 16: push(stack,'X'); push(stack,'Y'); push(stack,'-'); break; // X -> -YX
		case 17: push(stack,'X'); push(stack,'Y'); push(stack,'*'); break; // X -> *YX
		case 18: push(stack,'X'); push(stack,'Y'); push(stack,'/'); break; // X -> /YX
		case 19: push(stack,'X'); push(stack,'Y'); push(stack,'%'); break; // X -> %YX
		case 20: break; // X -> #
		case 21: push(stack,ID); break; // Y -> id
		case 22: push(stack,LIT); break; // Y -> const
		case 23: push(stack,')'); push(stack,'E'); push(stack,'('); break; // Y -> (E)
		case 24: push(stack,'A'); push(stack,'}'); push(stack,'B'); push(stack,'{'); push(stack,')'); push(stack,'Q'); push(stack,'('); push(stack,IF); break; //C -> if(Q){B}A
		case 25: push(stack,'T'); push(stack,ELSE); break; //A -> else T
		case 26: break; //A -> #
		case 27: push(stack,'A'); push(stack,'}'); push(stack,'B'); push(stack,'{'); push(stack,')'); push(stack,'Q'); push(stack,'('); push(stack,IF); break; //T -> if(Q){B}A
		case 28: push(stack,'}'); push(stack,'B'); push(stack,'{'); break; //T -> {B}
		case 29: push(stack,'}'); push(stack,'B'); push(stack,'{'); push(stack,')'); push(stack,'Q'); push(stack,'('); push(stack,WHILE);break; // L -> while(Q){B}
		case 30: push(stack,';'); push(stack,')'); push(stack,'Q'); push(stack,'('); push(stack,WHILE); push(stack,'}'); push(stack,'B'); push(stack,'{'); push(stack,DO); break; // L -> do{B}while(Q)
		case 31: push(stack,'E'); push(stack,CS); push(stack,'E'); break; // Q -> E cs E
	}
}

//check if sommet is non terminal
int isNonTerminal(char c){
	return c == 'P' || c == 'F' || c == 'B'  || c == 'S' || c == 'D'  || c == 'E' || c == 'X'  || c == 'Y' || c == 'C'  || c == 'A' || c == 'T'  || c == 'L' || c == 'Q' ? 1 : 0 ;
}

//analyse and check if program c
int syntax_analyser(Token *tokens){
	Stack stack;
	init_tack(&stack);
	push(&stack, '$');
	push(&stack, 'S');
	for(int i = 0 ; strcmp(tokens[i-1].token,"$") ; i++){
		char sommet = pop(&stack);
		if(isNonTerminal(sommet)){
			int line = get_line(sommet),column = get_column(tokens[i]);
			int act = tra_tab[line][column];
			if(column == -1) {
				printf("Erreur de syntax at position %d!\n",tokens[i].position);
				return 0;
			}
			if(act == -1) {
				printf("Erreur de syntax at position %d!\n",tokens[i].position);
				return 0;
			}
			take_action(&stack,act);
			i--;
		}else{
			if(strcmp(tokens[i].token,"$") == 0 && sommet == '$') return 1;
			if(!((strcmp(tokens[i].token,"if") == 0 && sommet == IF) 
				||(strcmp(tokens[i].token,"else") == 0 && sommet == ELSE)
				||(strcmp(tokens[i].token,"while") == 0 && sommet == WHILE)
				||(strcmp(tokens[i].token,"return") == 0 && sommet == RETURN)
				||(strcmp(tokens[i].token,"do") == 0 && sommet == DO)
				||(strcmp(tokens[i].type,TYPE_ID) == 0 && sommet == ID)
				||(strcmp(tokens[i].type,TYPE_LITTERAL) == 0 && sommet == LIT)
				||(is_comparing_symbol(tokens[i].token) && sommet == CS)
				||(is_type(tokens[i].token) && sommet == TYPE)
				||(sommet == tokens[i].token[0] && tokens[i].token[1] == '\0')
				)
			) {
				printf("Erreur de syntax at position %d!\n",tokens[i].position);
				return 0;
			}
		}
	}
}



int main(){
	char exp[EXPRESSION_SIZE];
	printf("===========================================================================\n");
	printf("=                      Write a c program to check :                       =\n");
	printf("===========================================================================\n");
	fgets(exp, EXPRESSION_SIZE, stdin);
	Token *tokens = tokeniser(exp);
	print_report(tokens);
	printf("\n=======================Syntax analyser=======================\n");
	if(syntax_analyser(tokens)) printf("Pas d\'erreur de syntax\n");
	return 0;
}