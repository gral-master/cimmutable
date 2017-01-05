-- Settings necessary to use fromList and toList on our data structures.
import GHC.Exts (IsList(..))

-- An intermediate node in a 2-3 tree, parameterized
-- based on the type of its child.
data Node a = Branch3 a a a -- The node can have 3 children.
            | Branch2 a a   -- ...or only two children.
            deriving Show


instance IsList (Node a) where
  type Item (Node a) = a
  
  toList (Branch2 x y) = [x, y]
  toList (Branch3 x y z) = [x, y, z]
  
  fromList [x, y] = Branch2 x y
  fromList [x, y, z] = Branch3 x y z
  fromList _ = error "Node must contain two or three elements"


-- Parameterize the affix by the type of data it stores.
-- This is equivalent to lists of length 1 to 4.
data Affix a = One a
             | Two a a
             | Three a a a
             | Four a a a a
             deriving Show

-- Let affixes be treated as lists.
instance IsList (Affix a) where
  type Item (Affix a) = a
  
  toList (One x)        = [x]
  toList (Two x y)      = [x, y]
  toList (Three x y z)  = [x, y, z]
  toList (Four x y z w) = [x, y, z, w]
  
  fromList [x]          = One x
  fromList [x, y]       = Two x y
  fromList [x, y, z]    = Three x y z
  fromList [x, y, z, w] = Four x y z w
  fromList _ = error "Affix must have one to four elements"
  
-- The following functions could be much more efficient.
-- We use the simplest implementation possible.
affixPrepend :: a -> Affix a -> Affix a
affixPrepend x = fromList . (x :) . toList

affixAppend :: a -> Affix a -> Affix a
affixAppend x = fromList . (++ [x]) . toList

-- As usual, the type parameter represents what type
-- of data is stored in this finger tree.
data FingerTree a 
  = Empty      -- We can have empty trees.
  | Single a   -- We need a special case for trees of size one.
  
  -- The common case with a prefix, suffix, and link to a deeper tree.
  | Deep {
    prefix :: Affix a,             -- Values on the left.
    deeper :: FingerTree (Node a), -- The deeper finger tree, storing deeper 2-3 trees.
    suffix :: Affix a              -- Values on the right.
  }
  deriving Show

-- Use <| to prepend. It's the finger tree analogue of : for lists. 
infixr 5 <|
(<|) :: a -> FingerTree a -> FingerTree a

-- Base case #1: If this is an empty finger tree, make it one element.
x <| Empty = Single x

-- Base case #2: For a single tree, upgrade it to a deep one.
-- Remember that the list syntax is actually creating 'Affix a' values.
x <| Single y = Deep [x] Empty [y]

-- Recursive case: if we have a prefix with four elements, we have to
-- use the last 2 elements with the new one to create a node, and then
-- we prepend that node to the deeper finger tree which contains nodes.
x <| Deep [a, b, c, d] deeper suffix = Deep [x, a] (node <| deeper) suffix
  where
    node = Branch3 b c d
    
-- Non-recursive case: we can just prepend to the prefix.
x <| tree = tree { prefix = affixPrepend x $ prefix tree }

t1 = Empty
t2 = 1 <| t1
t3 = 2 <| t2
t4 = 3 <| t3
t5 = 4 <| t4
t6 = 5 <| t5
t7 = 6 <| t6
t8 = 7 <| t7
t9 = 8 <| t8
t10 = 9 <| t9
t11 = 10 <| t10

t = foldl (<|) (Single 1) [2..100]

main = do
  print "----T1----"
  -- print t1
  print "----T2----"
  print t2
  print "----T3----"
  print t3
  print "----T4----"
  print t4
  print "----T5----"
  print t5
  print "----T6----"
  print t6
  print "----T7----"
  print t7
  print "----T8----"
  print t8
  print "----T9----"
  print t9
  print "---T10----"
  print t10
  print "---T11----"
  print t11

{-|
  let layer3 = Empty in
  let layer2 = Deep prefix layer3 suffix
    where
      prefix = [Branch2 'i' 's', Branch2 'i' 's']
      suffix = [Branch3 'n' 'o' 't', Branch2 'a' 't']
  in
  let layer1 = Deep prefix layer2 suffix
    where
      prefix = ['t', 'h']
      suffix = ['r', 'e', 'e']
  in
  let exampleTree = layer1
  in
  exampleTree
-}
