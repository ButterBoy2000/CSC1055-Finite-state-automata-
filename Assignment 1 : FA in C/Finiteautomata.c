/* stefania omoede ogun
sid: 2370091
I acknowledge the dcu academic integrity policy in this submitted work */

#include "Finiteautomata.h"


//function prototype in Finiteautomata.h



//function definiton 


FA createFA(FA machine)
{
	int capacity = 10;
	char **set_state = NULL; //pointer to store array of strings
	char **accept = NULL;
	Transition *table = NULL;
	char *start_state = NULL;



	//memory allocation
	set_state = (char**)malloc(capacity * sizeof(char*));
	if(!set_state)
	{
		printf("Memory allocation failed\n");
	}

	accept = (char**)malloc(capacity * sizeof(char*));
	if(!accept)
	{
		printf("Memory allocation failed\n");
	}

	table = (Transition*)malloc(capacity * sizeof(Transition));

	if(!table)
	{
		printf("Memory allocation failed\n");
	}


	machine.set_state = set_state;
	machine.table = table;
	machine.start_state = NULL;
	//printf("%d wasddd\n", machine.start_state == NULL);
	machine.accept = accept;
	machine.num_states = 0;
	machine.capacity = capacity;
	machine.num_transition = 0;

	printf("Finite automata created.\n");


	return machine;
}


//adds a state to the finite automata

void addState(FA *machine, char* state, int start, int accept)
{




    if((*machine).num_states >= (*machine).capacity)
    {
    	(*machine).capacity *= 2;
    	(*machine).set_state = realloc((*machine).set_state, (*machine).capacity * sizeof(char*));
    	(*machine).accept = realloc((*machine).accept, (*machine).capacity * sizeof(char*));
    	(*machine).table = realloc((*machine).table, (*machine).capacity * sizeof(Transition));


	
	    if(!(*machine).set_state || !(*machine).accept || !(*machine).table)
	    {
		  printf("Memory allocation failed\n");
		  exit(1);
	    }

    }
    
	(*machine).set_state[(*machine).num_states] = strdup(state);
	printf("State added: %s\n", state); 
	




	if((*machine).start_state == NULL && start == 0)
	{
		(*machine).start_state = strdup(state);
		 printf("Start state added: %s\n", state); 

	}
	else if (start == 0){
		printf("Start state %s not added, Start state already present: %s\n", state, (*machine).start_state);

	}


	if(accept == 0)
		{
			(*machine).accept[(*machine).num_states] = strdup(state);
			 printf("Acceptance state added: %s\n", state); 

		}


	(*machine).num_states += 1; 

	
}


//checking to see if state is in machine
int inFa(FA *machine, char *state)
{
	int value = 1;
	int size = (*machine).num_states;
	for(int i = 0; i < size; i++)
	{
		if(strcmp((*machine).set_state[i], state) == 0)
			{ value = 0;
	        }
	}
	return value;
}


//adding a transition from one state to another in the machine

void addTransition(FA *machine, char* from_state, char* symbol, char* to_state)
{
	int start = inFa(machine, from_state);
	int ends = inFa(machine, to_state);
	int i = (*machine).num_transition;

    if (start == 0 && ends == 0)
	{
		(*machine).table[i].curr_state = strdup(from_state);
		(*machine).table[i].next_state = strdup(to_state);
		(*machine).table[i].symbol = symbol[0]; //taking just the first char
		(*machine).num_transition += 1;

		if(strcmp(symbol, "\0") == 0)
		{
		  printf("Transition from state %s to state %s via Epsilon, added.\n",(*machine).table[i].curr_state, (*machine).table[i].next_state);
		
		}
		else
		{
		  printf("Transition from state %s to state %s via  %c, added.\n",(*machine).table[i].curr_state, (*machine).table[i].next_state, (*machine).table[i].symbol);

		}


	}

	else 
	{
		printf("Inputed state(s) is not present in the machine. No transition added.\n"); 
	}


}



//helper function given a state it returns array of epsilon transitions from start set
Transition* epsilonTransitions(FA machine, char* state) 
{
	int size = machine.num_transition;
	Transition *result = malloc((size + 1) * sizeof(Transition));
	int epsilon = 0;


	for(int i = 0; i < size; i++)
	{
		if(strcmp(machine.table[i].curr_state, state ) == 0 && machine.table[i].symbol == '\0')
		{
			result[epsilon++] = machine.table[i]; //i match it then i increment
		}

	}


	result[epsilon].curr_state = NULL;
    result[epsilon].next_state = NULL;
	return result;
}


// give array of strings representing the epsilon-closure
char** closureSet(FA machine, char* state)
{
    char **set = malloc(machine.capacity * sizeof(char*));
    int size = 0;
    char **stack = malloc(machine.capacity * sizeof(char*));
    int top = 0;

    set[size++] = strdup(state);
    stack[top++] = strdup(state);

    while(top > 0)
    {
        char *current = stack[--top];

        // get all epsilon transitions from current
        Transition *eps = epsilonTransitions(machine, current);
        for(int i = 0; eps[i].curr_state != NULL; i++)
        {
            char *next = eps[i].next_state;

            // check if next is already in closureSet
            int found = 0;
            for(int j = 0; j < size; j++)
            {
                if(strcmp(set[j], next) == 0)
                {
                    found = 1;
                    break;
                }
            }

            if(!found)
            {
                set[size++] = strdup(next);
                stack[top++] = strdup(next);
            }
        }

        free(eps);
        free(current);
    }

    free(stack);
    set[size] = NULL;

    return set; 

    //maybe integrate the printing in the function? so both return and prints?, will ask david monday; easy to change
}


//closure function

void closure(FA machine, char* state)
{
	char** set = closureSet(machine, state);
	printf("{ ");

	for(int i = 0; set[i + 1] != NULL; i++)
	{
		printf("%s ", set[i + 1]); //dont print first not in his example but its usefull to have
		free(set[i + 1]);
	}
	printf("}\n");
	free(set);
}



//function to check if deterministic
int deterministic(FA machine)
{
    for(int i = 0; i < machine.num_transition; i++)
    {
        // Check for epsilon transitions
        if(machine.table[i].symbol == '\0')
        {
            printf("The Finite Automata is not deterministic: epsilon transition present.\n");
            return 1;
        }

        // Check for duplicate transitions from the same state on same symbol
        for(int j = i + 1; j < machine.num_transition; j++)
        {
            if(strcmp(machine.table[i].curr_state, machine.table[j].curr_state) == 0
               && machine.table[i].symbol == machine.table[j].symbol)
            {
                printf("The Finite Automata is not deterministic: multiple transitions via the same symbol.\n");
                return 1;
            }
        }
    }

    printf("The Finite Automata is deterministic.\n");
    return 0;
}




    //prima creo un array with the possible transition from a to b
    //poi iterate l'array con cue transition e vedo se ci sono delle epsilon transition
    //aggiungo quelle al array
    //per non avere loop infinito controllo che non ci sia gia la transizione nel nuovo array

char** nextSet(FA machine, char* state, char* symbol) //debating between void or return char, but returning chat makes it easier for accept impementation, so maybw also closure returns and, or make a printing function that call this? so it is still void?
{
    char **startClosure = closureSet(machine, state);
    char **temp = malloc(machine.capacity * sizeof(char*));
    int tempSize = 0;

    for (int i = 0; startClosure[i] != NULL; i++)
    {
        for (int j = 0; j < machine.num_transition; j++)
        {
            if ((strcmp(machine.table[j].curr_state, startClosure[i]) == 0) &&
                (machine.table[j].symbol == symbol[0]))
            {
                int found = 0;
                for (int k = 0; k < tempSize; k++)
                {
                    if (strcmp(temp[k], machine.table[j].next_state) == 0)
                    {
                        found = 1;
                        break;
                    }
                }
                if (!found)
                    temp[tempSize++] = strdup(machine.table[j].next_state);
            }
        }
    }
    char **result = malloc(machine.capacity * sizeof(char*));
    int resultSize = 0;

    for (int i = 0; i < tempSize; i++)
    {
        char **closure = closureSet(machine, temp[i]);

        for (int j = 0; closure[j] != NULL; j++)  //should i make this a function? from here to
        {
            int found = 0;
            for (int k = 0; k < resultSize; k++)
            {
                if (strcmp(result[k], closure[j]) == 0)
                {
                    found = 1;
                    break;
                }
            }
            if (!found)
                result[resultSize++] = strdup(closure[j]);
        }

        for (int j = 0; closure[j] != NULL; j++) free(closure[j]);
        free(closure); //here
    }
    for (int i = 0; i < tempSize; i++) free(temp[i]);
    free(temp);
    for (int i = 0; startClosure[i] != NULL; i++) free(startClosure[i]);
    free(startClosure);

    result[resultSize] = NULL;


     /*  printf("{ ");
    for (int i = 0; i < resultSize; i++)
    {
        printf("%s ", result[i]);
    }
    printf("}\n"); 

*/
    return result;
}



void next(FA machine, char* state, char* symbol) //calls other function, nextset
{
	char** set = nextSet(machine, state, symbol);
	printf("{ ");

	for(int i = 0; set[i] != NULL; i++)
	{
		printf("%s ", set[i]);
		free(set[i]);
	}
	printf("}\n");
	free(set);
}


//check if just one char
//call next on the one char starting from the start state adding the symbol
//check if acceptance state is in the array, 
void accepts(FA machine, char* word) //genuenly to bigg
{
    if (machine.start_state == NULL)
    {
        printf("No start state present.");
        return;
    }

    int accept = 0;

    if (strlen(word) == 0)
    {
        char **startClosure = closureSet(machine, machine.start_state);

        for (int i = 0; startClosure[i] != NULL; i++)
        {
            for (int j = 0; j < machine.num_states; j++)   // ✅ changed
            {
                if (machine.accept[j] != NULL &&
                    strcmp(startClosure[i], machine.accept[j]) == 0)
                {
                    accept = 1;
                    break;
                }
            }
            if (accept) break;
        }

        if (accept)
            printf("Accepted: the Epsilon transition is part of the FA language.\n");
        else
            printf("Rejected: the Epsilon transition is not part of the FA language.\n");

        for (int i = 0; startClosure[i] != NULL; i++)
            free(startClosure[i]);
        free(startClosure);
        return;
    }

    else if (strlen(word) == 1)
    {
        char **startNext = nextSet(machine, machine.start_state, word);

        for (int i = 0; startNext[i] != NULL; i++)
        {
            for (int j = 0; j < machine.num_states; j++)   //changed
            {
                if (machine.accept[j] != NULL &&
                    strcmp(startNext[i], machine.accept[j]) == 0)
                {
                    accept = 1;
                    break;
                }
            }
            if (accept) break;
        }

        if (accept)
            printf("Accepted: %s is part of the FA language.\n", word);
        else
            printf("Rejected: %s is not part of the FA language.\n", word);

        for (int i = 0; startNext[i] != NULL; i++)
            free(startNext[i]);
        free(startNext);
        return;
    }

    else
    {
        char **currSet = closureSet(machine, machine.start_state);

        for (int i = 0; word[i] != '\0'; i++)
        {
            char **nextStates = NULL;
            int count = 0;
            char symbol[2] = { word[i], '\0' };

            for (int j = 0; currSet[j] != NULL; j++)
            {
                char **eachNext = nextSet(machine, currSet[j], symbol);

                for (int k = 0; eachNext[k] != NULL; k++)
                {
                    int seen = 0;
                    for (int l = 0; l < count; l++)
                    {
                        if (strcmp(nextStates[l], eachNext[k]) == 0)
                        {
                            seen = 1;
                            break;
                        }
                    }

                    if (!seen)
                    {
                        nextStates = realloc(nextStates, (count + 2) * sizeof(char *));
                        nextStates[count] = strdup(eachNext[k]);
                        count++;
                        nextStates[count] = NULL;
                    }
                }

                for (int m = 0; eachNext[m] != NULL; m++)
                    free(eachNext[m]);
                free(eachNext);
            }

            for (int j = 0; currSet[j] != NULL; j++)
                free(currSet[j]);
            free(currSet);

            currSet = nextStates;

            if (currSet == NULL || currSet[0] == NULL)
            {
                printf("Rejected: %s is not part of the FA language.\n", word);
                return;
            }
        }

        for (int i = 0; currSet[i] != NULL; i++)
        {
            for (int j = 0; j < machine.num_states; j++)
            {
                if (machine.accept[j] != NULL &&
                    strcmp(currSet[i], machine.accept[j]) == 0)
                {
                    accept = 1;
                    break;
                }
            }
            if (accept) break;
        }

        if (accept)
            printf("Accepted: %s is part of the FA language.\n", word);
        else
            printf("Rejected: %s is not part of the FA language.\n", word);

        for (int i = 0; currSet[i] != NULL; i++)
            free(currSet[i]);
        free(currSet);
    }
}





FA toDFA(FA machine) //just by value since i dont need to modify machine, maybe do void instead of empty? but then i have pass by value and modify the nfa directly;
//not sure if that's what he's asking; should ask david
{
    if(deterministic(machine) == 0)
    {
        printf("FA is already a DFA.\n");
        exit(1);
    }

    FA DFA = createFA(DFA); //initialising dfa that i will return later

    char **startClosure = closureSet(machine, machine.start_state);

    char stateName[100][10]; //future names
    char **stateSet[100]; //should i dymanic immediatly? maybe realloc better?
    int count = 0; //keep track dfa state

    int f = 0, b = 0;
    int queue[100];

    sprintf(stateName[count], "q%d", count); //name the start state, maybe will change to s instead
    addState(&DFA, stateName[count], 0, 1); //yes to being start no to being accept
    stateSet[count] = startClosure; //dfa state bein nfa closure from start state

    queue[b++] = count;
    count++; //tracking created states

    while (f < b) //till all state done
    {

        //taking next dfa state
        int currIndex = queue[f++];
        char *currName = stateName[currIndex];
        char **currSet = stateSet[currIndex];

        for (int t = 0; t < machine.num_transition; t++)
        {
            char symbol = machine.table[t].symbol;
            if (symbol == '\0') continue; //too late to change epsilon rappresentation, but makes sense in c imo, should probably write it in read me how to use

            char **newSet = NULL; 
            int newSize = 0;

            for (int i = 0; currSet[i] != NULL; i++) //checking transition of states
            {
                for (int j = 0; j < machine.num_transition; j++)
                {
                    if (strcmp(machine.table[j].curr_state, currSet[i]) == 0 && machine.table[j].symbol == symbol)
                    {
                        char **closure = closureSet(machine, machine.table[j].next_state);

                        for (int k = 0; closure[k] != NULL; k++)
                        {
                            int found = 0;
                            for (int m = 0; m < newSize; m++)
                            {
                                if (strcmp(newSet[m], closure[k]) == 0)
                                {
                                    found = 1;
                                    break;
                                }
                            }

                            if (!found)
                            {
                                newSet = realloc(newSet, (newSize + 2) * sizeof(char *)); //not the best practice maybe too small chucks?
                                newSet[newSize++] = strdup(closure[k]);
                                newSet[newSize] = NULL;
                            }
                        }

                        for (int k = 0; closure[k] != NULL; k++)
                            free(closure[k]); //free bits then free whole
                        free(closure);
                    }
                }
            }

            if (newSet == NULL || newSet[0] == NULL)
            {
                if (newSet) free(newSet); //need to free array even if its empty, giving me issue if not
                continue;
            }

            int existingIndex = -1; //just c convention
            for (int s = 0; s < count; s++)
            {
                int same = 1;

                for (int i = 0; newSet[i] != NULL; i++) //checking if it exits in stateset too
                {
                    int found = 0;
                    for (int j = 0; stateSet[s][j] != NULL; j++)
                    {
                        if (strcmp(newSet[i], stateSet[s][j]) == 0)
                        {
                            found = 1;
                            break;
                        }
                    }
                    if (!found)
                    {
                        same = 0; //change flag to zero if no match
                        break;
                    }
                }

                int frstSize = 0, sndSize = 0;
                while (newSet[frstSize] != NULL) frstSize++;
                while (stateSet[s][sndSize] != NULL) sndSize++;
                if (frstSize != sndSize) same = 0; //change to zero also if different length.

                if (same)
                {
                    existingIndex = s;
                    break; //no point in checking more, should have found same set
                }
            }

            char nextName[10]; 
            if (existingIndex == -1)
            {
                sprintf(nextName, "q%d", count); //ask david what he names the dfa states 
                addState(&DFA, nextName, 1, 1); //just regular states
                stateSet[count] = newSet;
                strcpy(stateName[count], nextName);
                queue[b++] = count;
                count++;
            }
            else
            {
                strcpy(nextName, stateName[existingIndex]);
                for (int i = 0; newSet[i] != NULL; i++) free(newSet[i]);
                free(newSet);
            }

            int duplicate = 0;
            for (int i = 0; i < DFA.num_transition; i++)
            {
                if (strcmp(DFA.table[i].curr_state, currName) == 0 &&
                    strcmp(DFA.table[i].next_state, nextName) == 0 &&
                    DFA.table[i].symbol == symbol)
                {
                    duplicate = 1;
                    break;
                }
            }
            if (!duplicate) //making sure it dosnt already exist 
            {
                char symbolStr[2] = {symbol, '\0'}; //maybe change addtrans to support char instead of char*?, not a big deal adding '\0' tho
                addTransition(&DFA, currName, symbolStr, nextName);
            }
        }
    }

    for (int i = 0; i < count; i++)
    {
        char **set = stateSet[i];
        for (int j = 0; set[j] != NULL; j++)
        {
            for (int a = 0; a < machine.num_states; a++)
            {
                if (machine.accept[a] != NULL &&
                    strcmp(set[j], machine.accept[a]) == 0)
                {
                    addState(&DFA, stateName[i], 1, 0); //feel like it's clear that 1 is no and 0 is yes but idk, maybe write it in read me and explain why i choose those two numbers
                }
            }
        }
    }

    printf("DFA successfully created form given NFA.\n");
    return DFA; //should i make function to print dfa components?
}






    



//gcc -o FA FA.c Finiteautomata.c (fist)
//gcc -o Finiteautomata Finiteautomata.c FA.c (second)


