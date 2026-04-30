
data AvlTree t = Empty | Root t (AvlTree t) (AvlTree t) 
    deriving (Eq, Ord)

height :: AvlTree t -> Int
height Empty = 0
height (Root _ left right) = 1 + (max (height left) (height right))


balanceType :: AvlTree t -> Int
balanceType Empty = 0
balanceType (Root _ left right) = (height left) - (height right)


balanceLL :: AvlTree t -> AvlTree t
balanceLL (Root n (Root n1 lst1 rst1) rst) = Root n1 lst1 (Root n rst1 rst)
balanceLL t = t

balanceRR :: AvlTree t -> AvlTree t --oppsitive of the ll rotation pattern matching  
balanceRR (Root n lst (Root n1 lst1 rst1)) = Root n1 (Root n lst lst1) rst1
balanceRR t = t

balanceLR :: AvlTree t -> AvlTree t
balanceLR (Root n lst rst) = balanceLL (Root n (balanceRR lst) rst) --rotate the left child to the right then rotate whole tree left
balanceLR t = t

balanceRL :: AvlTree t -> AvlTree t
balanceRL (Root n lst rst) = balanceRR (Root n lst (balanceLL rst)) -- opposite of lr : rotate the right child to the left then rotate whole tree right
balanceRL t = t



--balance the tree: i call the balance tree function to check if it is unbalanced and what type of rotation is needed, then i call the adeguate rotation function

balanceTree :: AvlTree t -> AvlTree t
balanceTree Empty = Empty
balanceTree (Root n left right) = let balanceleft = balanceTree left
                                      balanceright = balanceTree right

                                  in if balanceType (Root n balanceleft balanceright) > 1 && balanceType balanceleft >= 0 then
                                          balanceLL (Root n balanceleft balanceright)
                                      else if balanceType (Root n balanceleft balanceright) < -1 && balanceType balanceright <= 0 then
                                          balanceRR (Root n balanceleft balanceright)
                                      else if balanceType (Root n balanceleft balanceright) > 1 && balanceType balanceleft < 0 then
                                          balanceLR (Root n balanceleft balanceright)
                                      else if balanceType (Root n balanceleft balanceright) < -1 && balanceType balanceright > 0 then
                                          balanceRL (Root n balanceleft balanceright)
                                      else
                                          Root n balanceleft balanceright
                                    



--inserting element into avl tree, similar to bin tree but i also make sure that the tree is still balanced after the insertion

addNode :: (Ord t) => t -> AvlTree t -> AvlTree t
addNode a Empty = Root a Empty Empty
addNode x (Root a left right)
    | x < a     = balanceTree (Root a (addNode x left) right) --add node then balance tree if needed
    | otherwise = balanceTree (Root a left (addNode x right))



instance Show t => Show (AvlTree t) where --defining my own show function for the avl tree
    show Empty = " Empty"
    show (Root x left right) =
        "Root " ++ show x ++ " ( left: " ++ show left ++ "  right: " ++ show right ++ ")"
