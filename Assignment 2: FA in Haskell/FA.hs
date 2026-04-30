
{-refernces:
deconstruction of constume data type: https://stackoverflow.com/questions/10115623/accessing-members-of-a-custom-data-type-in-haskell
defining a data type: CSC1048-Introduction to haskell pdf
custom show function: https://stackoverflow.com/questions/18606827/how-to-write-customised-show-function-in-haskell
map and filters :https://learnyouahaskell.com/higher-order-functions#:~:text=map%20takes%20a%20function%20and,is%20and%20how%20it's%20defined.&text=You've%20probably%20noticed%20that,achieved%20with%20a%20list%20comprehension.
-}


module FA where --not needed tho

import qualified Data.Map.Strict as Map
import Data.Map.Strict (Map)
import Data.List (intercalate, nub)





type TransitionTable = Map.Map (String, Char) [String] -- key: beginning state and simbol, value, destination state

data FA = Empty | FA{
  states :: [(String,Bool,Bool)],
  symbols :: [Char],
  transitions :: TransitionTable, --from  the haskell dfa module, higher order function, maybe a transition table would be better
  start :: String, --maybe remove
  accept :: [String]

}
  

{- add state is first, then its add trnastion, lookup required to make sure states are there; same state should not be added twice -}

--when the FA is empty
addState :: (String,Bool,Bool) -> FA -> FA
addState (s,True,False) Empty = FA [(s, True, False)] [] Map.empty s []
addState (s, False, True) Empty = FA [(s, False, True)] [] Map.empty "" [s]
addState (s, False, False) Empty = FA [(s, False, False)] [] Map.empty "" []
addState (s, True,True) Empty = FA [(s, True, True)] [] Map.empty s [s] 

--when the FA is not empty
addState (s,True,False) (FA st sy tr start acc) 
  | null start = FA (st ++ [(s,True,False)]) sy tr s acc
  | otherwise = error "Start state already present"

addState (s,False,True) (FA st sy tr start acc)
  | any (\(name,_,_) -> name == s) st = error "Acceptance state already present" --you cannot insert two states with the same name even if one is a start state and the other is an accept state
  | otherwise = FA (st ++ [(s,False,True)]) sy tr start (acc ++ [s])

addState (s,False,False) (FA st sy tr start acc)
  |  any (\(name,_,_) -> name == s) st = error "State already present"
  | otherwise = FA (st ++ [(s,False,False)]) sy tr start acc



addTransition :: (String,Char,String) -> FA -> FA
addTransition _ Empty = error "The FA is empty, transitions cannot be added"

addTransition (from, symbol, to) (FA st sy tr start acc)
  |inFA from st == False = error ("The state " ++ from ++ " does not exit.")
  |inFA to st == False = error ("The state " ++ from  ++ " does not exit.")
  |otherwise = let key = (from, symbol)
                   look = Map.findWithDefault [] key tr
                   newlook  = look ++ [to]
                   newTable    = Map.insert key newlook tr
                   newSymbol  = if symbol `elem` sy then sy else sy ++ [symbol]
               in FA st newSymbol newTable start acc 
--lookup make sure in fa
--lookup make sure not already in transtion table



closure :: FA -> String -> [String]
closure Empty _ = error "The FA is empty, no states exist."
closure (FA st sy tr start acc) state
  | inFA state st == False = error ("The state " ++ state ++ " does not exist.")
  |otherwise = let list = Map.findWithDefault [] (state , 'ε') tr
                   states = filter (`notElem` [state]) list
               in state : concatMap (closure (FA st sy tr start acc)) states
                    


next :: FA -> String -> Char -> [String]
next Empty _ _ = error "The FA is empty"

next (FA st sy tr start acc) state _
  | not (inFA state st) = error ("The state " ++ state ++ " does not exist")

next (FA st sy tr start acc) state symbol =
  let epsilon   = closure (FA st sy tr start acc) state
      states = concatMap (\s -> Map.findWithDefault [] (s, symbol) tr) epsilon
      list     = concatMap (closure (FA st sy tr start acc)) states
  in duplicates list



deterministic :: FA -> Bool
deterministic Empty = error "The FA is empty"
deterministic (FA st sy tr start acc) = all noepsilon (Map.toList tr) && all transition (Map.toList tr)
  where 
  noepsilon ((_, symbol), _) = symbol /= 'ε'
  transition ((from, symbol), tos) = length tos <= 1



toDFA :: FA -> FA --thinking back i could have just imported sets and used that instead of lists  
toDFA Empty = Empty
toDFA nfa
  | deterministic nfa = nfa
  | otherwise =
      let nfaSyms = filter (/= 'ε') (symbols nfa)
          startStateSet = closure nfa (start nfa)
          startName = intercalate "," (duplicates startStateSet)

          -- BFS
          buildDFA :: [ [String] ] -> Map.Map (String, Char) String -> [ [String] ] -> Map.Map (String, Char) String
          buildDFA [] trAcc _ = trAcc
          buildDFA (curr:rest) trAcc seen =
              let currName = intercalate "," (duplicates curr)
                  newTransitions = foldl (\acc sym ->
                      let nextStates = duplicates $ concatMap (\s -> next nfa s sym) curr
                          nextClosure = duplicates $ concatMap (closure nfa) nextStates
                          nextName = intercalate "," nextClosure
                      in if null nextClosure then acc
                         else Map.insert (currName, sym) nextName acc
                      ) trAcc nfaSyms
                  newStates = filter (`notElem` seen) $
                              map (\sym -> duplicates $ concatMap (closure nfa) (concatMap (\s -> next nfa s sym) curr)) nfaSyms
              in buildDFA (rest ++ newStates) newTransitions (seen ++ newStates)

          dfaTrans = buildDFA [startStateSet] Map.empty [startStateSet]
          dfaStateNames = nub $ startName : Map.elems dfaTrans

          dfaAccept = [s | s <- dfaStateNames,
                          any (`elem` accept nfa) (splitState s)]
      in FA
         (map (\s -> (s, s == startName, s `elem` dfaAccept)) dfaStateNames)
         nfaSyms
         (Map.map (\v -> [v]) dfaTrans) -- wrap dest in list to match TransitionTable type
         startName
         dfaAccept



accepts :: FA -> String -> Bool
accepts Empty _ = error "The FA is empty"
accepts (FA st sy tr start acc) input
    | not (inFA start st) = error "Start state is not defined"
    | otherwise =
        let startSet = closure (FA st sy tr start acc) start
            valid sym = sym == 'ε' || sym `elem` sy
            finalStates = foldl
                (\currentStates sym ->
                    if not (valid sym)
                        then error ("Symbol '" ++ [sym] ++ "' not in FA alphabet")
                        else duplicates (concatMap (\s -> next (FA st sy tr start acc) s sym) currentStates)
                )
                startSet
                input
        in any (`elem` acc) finalStates



{-not working id not costume cause trnasition is function-}
instance Show FA where
  show (FA st sy tr start acc) = --works, dont think i need to show the bools anyways
     "States: " ++ show (map(\(name,_,_) -> name) st) 
     ++ "\nSymbols: " ++ sy
     ++ "\nTransitions: "++ concat [from ++ "--> " ++ [symbol] ++ " -->" ++ show tos ++ "\n" | ((from,symbol), tos) <- Map.toList tr] 
     ++ "\nStart State: " ++ show start 
     ++ "\nAccept States: " ++ show acc --costume show function, cant use the defualt one cause cant print functions
  show Empty = "The FA is empty: no states, symbols, or transitions defined."
  --just need to use print to display output, maybe i'll change the formatting later




inFA :: String -> [(String,Bool,Bool)] -> Bool
inFA name stateList = any(\(n,_,_) -> n == name) stateList


duplicates :: (Eq a) => [a] -> [a]
duplicates [] = []
duplicates (x:xs) = x : duplicates (filter (/= x) xs)


splitState :: String -> [String]
splitState = words . map (\c -> if c == ',' then ' ' else c)
