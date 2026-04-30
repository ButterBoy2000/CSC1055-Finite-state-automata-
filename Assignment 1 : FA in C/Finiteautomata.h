#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//#include <unistd.h>
#define pr(...) printf(__VA_ARGS__) //renaming printf



//declaring innner struct
typedef struct Transition Transition; //then get populated with add transiton function, meaning function retunrs struct?
typedef struct FA FA;


struct Transition {

	char *curr_state;
	char symbol;
	char *next_state;
};

struct FA
{
	char **set_state;
	Transition *table;
	char *start_state;
	char **accept;	
	int num_states;
	int capacity;
	int num_transition;

}; 


// function definition
FA createFA(FA machine);
void addState(FA *machine, char* state, int start, int accept);
void addTransition(FA *machine, char* from_state, char* symbol, char* to_state);
int inFa(FA *machine, char *state);
//Transition existTransition(char *curr_state, char symbol);
Transition* epsilonTransitions(FA machine, char* state);
char** closureSet(FA machine, char* state);
void closure(FA machine, char* state);
int deterministic(FA machine);
char** nextSet(FA machine, char* state, char* symbol);
void next(FA machine, char* state, char* symbol);
void accepts(FA machine, char* word);
FA toDFA(FA machine);