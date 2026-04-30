/* addstate(name,start or not, acceptance or not)
addTransition (current state, symbol, new state)
accepts(string) start from state 1 follow stransition and see if it gets to one acceptance state
? how do i do if its a nfa?

closure(current state), returns array of strings?, return empty set if no e transitions

next(current state, input value)
deterministic(dfa name), if e yes, if more than one transition for same input yes, if
neither no

todfa(returns table of dfa?)
*/
#include "Finiteautomata.h"



int main(void) 
{
	FA M1 = createFA(M1); //initialising an empty FA


	addState(&M1, "0", 0, 1); //adding a start state
	addState(&M1, "1", 0, 1); //trying to add another start state
	addState(&M1, "2", 1, 1); //regular state
	addState(&M1, "3", 1, 1);//regular state
	addState(&M1, "4", 1, 1);//regular state
	addState(&M1, "5", 1, 1);//regular state
	addState(&M1, "6", 1, 1);//regular state
	addState(&M1, "7", 1, 1);//regular state
	addState(&M1, "8", 1, 1);//regular state
	addState(&M1, "9", 1, 1);//regular state
	addState(&M1, "10", 1, 0); //acceptance state


	addTransition(&M1, "0", "", "1"); //adding transitions from example
	addTransition(&M1, "0", "", "7");
	addTransition(&M1, "1", "", "2");
	addTransition(&M1, "1", "", "4");
	addTransition(&M1, "2", "a", "3");
	addTransition(&M1, "3", "", "6");
	addTransition(&M1, "6", "", "7");
	addTransition(&M1, "6", "", "1");
	addTransition(&M1, "7", "a", "8");
	addTransition(&M1, "8", "b", "9");
	addTransition(&M1, "9", "b", "10");
	addTransition(&M1, "4", "b", "5");
	addTransition(&M1, "5", "", "6");



	closure(M1,"3"); //getting closure from state 3
	closure(M1, "7"); //empty set

	deterministic(M1); //should be false
	next(M1, "4","b"); 
	next(M1, "5", "a");

	//m1 language (a|b)*abb , a or b zero or more times followed by abb
	accepts(M1, ""); //should refuse
	accepts(M1, "a"); //should refuse
	accepts(M1, "aaaaaaabb"); 
	accepts(M1, "bbbabb");
	accepts(M1, "aa");
	accepts(M1, "aaaaaaaaabb");
	accepts(M1, "aaaaaaaaab");
	accepts(M1, "abb");

    FA DFA1 = toDFA(M1); //making a dfa from the nfa M1
    deterministic(DFA1); //making sure it says true

    for (int i = 0; i < DFA1.num_transition; i++) {

    printf("Transition: from %s to  %s via %c\n", DFA1.table[i].curr_state,DFA1.table[i].next_state,DFA1.table[i].symbol); //making sure transitions are correct

    }



	FA M2 = createFA(M2); //making sure i didnt hardcode the example nfa by mistake
	addState(&M2, "0", 1, 0);
	addState(&M2, "1", 1, 0); //trying to add another start state
	addState(&M2, "2", 0, 1);
	addTransition(&M2, "0", "a", "1");
	addTransition(&M2, "0", "a", "0");
	addTransition(&M2, "1", "b", "2");
	addTransition(&M2, "2", "a", "0");
	addTransition(&M2, "2", "a", "1");
	deterministic(M2);




	FA M3 = createFA(M3);
	addState(&M3, "6", 0, 1);
	addState(&M3, "7", 1, 1);
	addState(&M3, "8", 1, 1);
	addState(&M3, "9", 1, 1);
	addState(&M3, "10", 1, 0);  
	addTransition(&M3, "6", "a", "7");
	addTransition(&M3, "6", "b", "8");
	addTransition(&M3, "8", "b", "9");
	addTransition(&M3, "9", "a", "10");
	addTransition(&M3, "10", "a", "6");
	deterministic(M3);   


  FA DFA2 = toDFA(DFA1); //already a dfa


return 0;
}


