stefania 
sid: xxxxxx
I acknowledge the dcu academic integrity policy in this submitted work 


## avl data type construction:
as the only difference between an AVl tree and a binary search tree is that the AVl tree performs rotation operations to keep the tree balance
i utilise the same data definition from the lab 5 task to construct the AVL data type


## helper functions

### height
calculates the height of a subtree and is used by the balance tree function to determine how unbalanced the node is

### balanceType 
gives the balance factor of a node that is later used to calculate if the tree is balance or left/right-heavy


## rotation functions

the rotation functions were implemented based on the definition in the lab 6 sheet
balanceLL Root n (Root n1 lst1 rst1) rst = Root n1 lst1 (Root n rst1 rst)
the rr rotation function is a mirror of the ll roation function 


## reference

### AVL definition
csc1048 computability and complexity lab 5 sample answer

### DSA AVL Tree theory and example
w3schools.com/ds_data_avltrees.php

### LL rotation function
csc1048 computability and complexity lab 6 task description

### Custom ahow function
http://stackoverflow.com/questions/18606827/how-to-write-customised-show-function-in-haskell

