module Main where

import Data.List
import Data.Bits
import Data.Char
import Control.Monad

main :: IO ()
main = do
	lineT <- getLine
	mapM_ domain $ take (read lineT :: Int) [1..]

-- Don't know how to do it elegantly now =_=
domain :: Int -> IO ()
domain _ = do
	line <- getLine
	if isDead $ map digitToInt line
	then putStrLn "DIES"
	else putStrLn "LIVES"

-- Judge whether the list will die
isDead :: [Int] -> Bool
isDead [] = False
isDead (_:[]) = True 
isDead arr@(x1:res) = if (and $ map (== 0) arr)
	then True  -- A list full with 0 is dead, otherwise an even-length list is alive.
	else let   -- Take it apart and calculator each.
		(odd, even) = takePart x1 res ([], [])
	in
		(isDead odd) && (isDead even)

-- Get thr fitst element, a list, an accumulator, and break it into even/odd parts.
takePart :: Int -> [Int] -> ([Int], [Int]) -> ([Int], [Int])
takePart _ [] (odd, even) = (odd, even)
takePart _ (_:[]) (_, _) = ([], []) -- When the length of list is even, it's always alive
takePart x1 (x2:x3:res) (odd, even) = (takePart x3 res ((xor x1 x3):odd, x2:even))
