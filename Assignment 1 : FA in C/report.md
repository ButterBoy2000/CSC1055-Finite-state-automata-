CSC1055 Comparative Programming Languages

Assignment 1: Finite State Automata

Group BL

| Student              | Student ID | Programming language |
| -------------------- | ---------- | -------------------- |
| xxxxxxxx      |xxxxxxx   | Java                 |
| Stefania  | xxxxxx   | C                    |


# Table of contents

[**1. Definition of the Finite State Automata ****1**](#1-definition-of-the-finite-state-automata)

[1.1 Definition of a State 1](#11-definition-of-a-state)

[1.2 Definition of a Transition 1](#12-definition-of-a-transition)

[**2. Functions ****2**](#2-functions)

[2.1 addState 2](#21-addstate)

[2.2 addTransition 2](#22-addtransition)

[2.3 closure 3](#23-closure)

[2.4 next 3](#24-next)

[2.5 accepts 3](#25-accepts)

[2.6 deterministic 4](#26-deterministic)

[2.7 toDFA 4](#27-todfa)

[3. Differences and similarities: which is better? 4](#3-differences-and-similarities-which-is-better)


#

# 1. Definition of the Finite State Automata

Before designing each function we first thought about which data structure to utilise for the FSA implementation.

In Java the FA is implemented as its own class having as attributes HashSets and HashMaps with references to states and transitions (which are their own classes).

When thinking about the implementation in c: the first idea was to utilise a linked list to represent the FA but we decided against it as it would need to traverse the entire linked list while implementing most of the functions.

Another possibility was utilising a hash map to represent the FA but, contrary to JAVA, c does not have any simple implementations for hashmaps.

Ultimately I opted for implementing the FA with a struct.


## 1.1 Definition of a State

### 1.1.1 JAVA:

Thanks to the OOP concept of encapsulation, State is defined as a separate class with three attributes: its name(string), isStart(boolean) and isAccepting(boolean). Through constructor chaining, a new State can be created either by inputting all three parameters or just the name, with the booleans automatically set to false.


### 1.1.2 C:

Contrary to JAVA, C does not have a built in object oriented feature that allows the user to define a new class; this could be somewhat emulated using struct but decided to simply store the states as a char\* (a string in JAVA). 

The start state is represented by a char\*, as the FA can only have one start state.

Both the acceptance states and the set of states are represented by an array of strings.


## 1.2 Definition of a Transition

### 1.2.1 JAVA:

Transitions are modeled by a class with three attributes: before(string), after(string) and symbol(character). Constructor chaining allows you to add empty transitions by either inputting the ‘\0’ character or omitting it from the parameters.


### 1.2.2 C:

The transitions are stored in an an array of Transition structs containing three members: current\_state(char \*), symbol (char) and next\_state(char\*).

The empty transition epsilon is represented by the empty string literal “”(represented in memory by ‘\0’).


# 2. Functions

Java allows method overloading and thus, it’s easy to create different behaviors for the same function based on the passed parameters.


## 2.1 addState

### 2.1.1 JAVA: 

The addState function was overloaded to accept both the entirety of the state’s parameters (name, isState, isAccepting) or simply its name and have the two booleans set to false.

This function adds a state to the list of states of the automaton unless it already exists.

Within the State constructor, isState and isAccepting are booleans because Java does not allow you to use 1s and 0s as true and false.


### 2.1.2 C:

While thinking about the design of the functions in C, the absence of overloading meant that each time the user called the functions all parameters needed to be defined.

Using booleans to check for validity was one of the option considered while designing helper functions; C, however, does not allow 

0s and 1s are used to indicate true or false, thanks to that conditions like

If (!found) or if(found) are possible although the variable “found” is an integer.

The addState function, first reallocates the arrays memory, if needed, then based on the parameters it adds the starts to the correct array.

It also conducts a check to make sure that after an initial start state has been assigned it does not get overwritten.

\
\



## 2.2 addTransition

The addTransition function implementation follow a similar logic in both the languages: 

In C the function checks to make sure that the states being added are part of the FA using the helper function inFA() which iterates through the set of states and returns a 0 if the state is present.

In Java, by having implemented the states as a HashMap, it’s possible to create a set of all keys and by comparing the inputted before and after states to those, check whether they exist.


## 2.3 closure

### 2.3.1 JAVA: 

The closure method is a 2-part function. It accepts an initial state and sends it into a recursive version of itself with an empty set of states which will be filled by the after states of all empty transitions. It will then call itself with this after state as the initial state.

The function will not add the initial state to the final set and, whenever I need it to be included, I add it manually to the set (such as in next, accepts and toDFA).


### 2.3.2 C:

The closure function simply prints the output of the helper function closureSet which also utilises another helper function epsilonTransitions function. 

The epsilonTransitions function returns an array of possible epsilon transitions that go directly from the inputted  state to another state via epslion.

The closureSet then uses it to look for all transitions (direct and indirect) from the inputted state.

One of the possible ways to implement this function was through recursion but we decided against it and opted for a stack instead.

 Recursion is much slower and utilises more memory becoming more problematic the bigger the FA gets especially since almost all of the utilised arrays memories are dynamically allocated.

<https://stackoverflow.com/questions/22443763/recursion-vs-stack-implementation-why-does-recursion-return-stackoverflow-when>


## 2.4 next

The next function works in a similar way in both languages: First we call our closure function to get our starting set of states. We then go through each of those states and for each transition that has that state as its before and the inputted character as its symbol, we add the after state to a new set of states. On that set we will finally do a closure on each of the states contained.

In C all the computation is done by the helper function nextSet while the next function simply formats and prints it out. 


## 2.5 accepts

### 2.5.1 JAVA:

In this function we use two sets of states, current and next. Current is initialized as the closure set of the start state. The inputted word is traversed character by character and for each of those we call next() on each of the states in current and add them to next. We do this to avoid a concurrent modification error. We then set current as next, check that it isn’t empty and return true if any of the states in it are accepting states.


### 2.5.2 C:

The function checks to make sure that an initial state is present, it is then divided in 3 parts:\
In the first part it checks if the empty symbol (epsilon) is part of FA the language by calling closure and checking if the set contains an acceptance state.

The second part checks if a single non-empty char is part of the language by calling nextSet and also checking to see if the set contains an acceptance state.

The final part checks if a string is part of the language by iterating through each char in the word calling next set, adding them to an empty array if not seen then moving to the next char.


## 2.6 deterministic

This method iterates over the FSA’s transitions while checking for duplicate fromState-Symbol pairs. When it finds either a duplicate pair or an epsilon transition it returns false, otherwise if it passes both checks it will return true.


### 2.7.1 JAVA:

In Java we use a set that starts empty and gets filled with each new pair. If a new pair is found within that set, we immediately return false.


### 2.7.2 C:

In C we simply utilise a loop that iterates through the transition table making sure that there are no multiple transitions from the same state with the same input.


## 2.7 toDFA

Both functions utilise a queue to keep track of unvisited sets of states and a set of each newly created NFA state so that duplicates aren’t added again. It will add states and transitions at the same time. We initially tried to first create all the states but realized that it wasn’t feasible afterwards to get all transitions.

\



## 3. Differences and similarities: which is better?

Spoiler: it’s Java

Java is definitely much easier to implement and later on maintain due to its easy-to-follow structure and flow of code. It’s also easier to make changes to single classes or attributes without breaking the whole code. Encapsulation allows us to hide attributes that users shouldn’t be able to access directly and allows the system to be more secure.

The garbage collection means, once again, that implementing the FSA is easier because you don’t have to worry about memory allocation. On the flip side, you can never guess when exactly the collection will happen and, thus, you risk taking up larger amounts of memory that could have been freed.

It’s also important to note that not all four main OOP principles are necessarily usable when creating an FSA: there is no need or use for abstraction and inheritance as all the implemented methods have to be part of the same class.

As mentioned earlier, C requires the programmer to keep track of memory allocation but can allow you to make sure no memory is ever unused or left taken up by a pointer.

Theoretically, C implementations should be faster, more efficient and take up less memory with larger FSAs, however with smaller ones there are close to no differences between the languages.
