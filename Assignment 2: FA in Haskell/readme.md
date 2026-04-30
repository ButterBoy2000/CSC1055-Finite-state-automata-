to compile you must have ghci installed

1. open your terminal go to the dir where the FA.hs file is present and type

ghci
:load FA

(if you apport any changes just type
:reload FA)

2. two ways to test the code, you could either manually run it (i'll insert test cases bellow) or copy and past this in the FA file


## Non-manual testing 

paste this underneath FA.hs
type :reload FA
type main

```
main :: IO ()
main = do
    -- Create empty FA
    let fa0 = Empty
    print fa0

    -- Add states
    let fa1 = addState ("q0", True, True) fa0   -- start & accept
    let fa2 = addState ("q1", False, True) fa1  -- accept only
    let fa3 = addState ("q2", False, False) fa2 -- normal state
    print fa3

    -- Add transitions
    let fa4 = addTransition ("q0",'a',"q1") fa3
    let fa5 = addTransition ("q0",'a',"q2") fa4
    let fa6 = addTransition ("q1",'b',"q2") fa5
    let fa7 = addTransition ("q2",'c',"q0") fa6
    let fa8 = addTransition ("q2",'ε',"q2") fa7
    let fa9 = addTransition ("q2",'ε',"q1") fa8
    print fa9

    -- Test closures
    putStrLn "Closure tests:"
    print $ closure fa9 "q2" 
    print $ closure fa9 "q0"
    print $ closure fa9 "q1"

    -- Test next
    putStrLn "Next tests:"
    print $ next fa9 "q0" 'a'
    print $ next fa9 "q1" 'b' 
    print $ next fa9 "q2" 'c'
    print $ next fa9 "q2" 'a' 

    -- Test deterministic
    putStrLn "Deterministic test:"
    print $ deterministic fa9

    -- Convert to DFA
    let dfa = toDFA fa9
    putStrLn "DFA:"
    print dfa

    -- Test deterministic on DFA
    putStrLn "Deterministic DFA test:"
    print $ deterministic dfa 
      -- Test accepts
    putStrLn "Accepts tests:"
    print $ accepts fa9 "ab"     
    print $ accepts fa9 "ac"      
    print $ accepts dfa "ab"    
    print $ accepts dfa "ac"    
    print $ accepts dfa "a"      
 ```

## Manual testing

  --haskell variables are immutable, so it returns a new FA, when testing it you must write it like this thanks :>

  either you declare a new empty fa, example: let fa0 = Empty
  or you can declare a new fa and add a state: let fa1 = addState ("q0", True, False) Empty
  
  epsilon transitions are represented by the character 'ε'

  Test cases:
   let fa0 = Empty --making sure and empty fa is creatable
   print fa0 --printing the fa
   fa0 <- return(addState ("q0", True, True) fa0) --making sure it adds a start and acceptance state
   fa0 <- return (addState ("q1", False, True) fa0) --making sure it add just a start state
   fa0 <-return(addState ("q2", False, False) fa0) --making sure it only adds it to the state
   fa0 <-return(addState ("q2", True, False) fa0) --an error message is thrown and you can no longer work on the fa, you have to start anew

  testing transitions:
   fa0 <- return(addTransition ("q0",'a',"q1") fa0)
   fa0 <- return(addTransition ("q0",'a',"q2") fa0)
   fa0 <- return(addTransition ("q1",'b',"q2") fa0) 
   fa0 <- return(addTransition ("q2",'c',"q0") fa0)
   fa0 <- return(addTransition ("q2",'ε',"q2") fa0) 
   fa0 <- return(addTransition ("q2",'ε',"q1") fa0) 
  
  testing closure:
   print (closure fa0 "q2")
   print (closure fa0 "q0") 
   print (closure fa0 "q1") 
  

  testing next:
    print (next fa0 "q0" 'a') 
    print (next fa0 "q1" 'b') 
    print (next fa0 "q2" 'c') 
    print (next fa0 "q2" 'a') 

    testing deterministic:
    print (deterministic fa0) 
    let dfa = toDFA fa0 
    print dfa 
    print (deterministic dfa) 
    testing accepts:
    print (accepts fa0 "ab") 
    print (accepts fa0 "ac")
    print (accepts dfa "ab") 
