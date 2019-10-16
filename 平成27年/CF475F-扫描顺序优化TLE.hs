-- Haskell常数大没办法，算法是对的
module Main where

import Data.List
import Data.Tuple
import qualified Data.Set as Set
import Control.Monad

data Split = Lef Int | Righ Int | Up Int | Down Int | None
	deriving (Show, Eq)
type Point = (Int, Int)
type PList = [Point]
type PSet = Set.Set Point

calcIter :: PSet -> PSet -> PSet -> PSet -> Split
calcIter xs xrs ys yrs =
  if (Set.size xs <= 1) then None
  else let
    (x1, xs') = (Set.deleteFindMin xs)
    (xr1, xrs') = (Set.deleteFindMax xrs)
    (y1, ys') = (Set.deleteFindMin ys)
    (yr1, yrs') = (Set.deleteFindMax yrs)
    x2 = (Set.findMin xs')
    xr2 = (Set.findMax xrs')
    y2 = (Set.findMin ys')
    yr2 = (Set.findMax yrs')
  in
    if      (fst x2) - (fst x1) > 1   then Lef $ (fst x1) + 1
    else if (fst xr1) - (fst xr2) > 1 then Righ $ (fst xr2) + 1
    else if (fst y2) - (fst y1) > 1   then Up $ (fst y1) + 1
    else if (fst yr2) - (fst yr1) > 1 then Down $ (fst yr2) + 1
    else calcIter xs' xrs' ys' yrs'

update :: PList -> (PSet, PSet) -> (PSet, PSet)
update (p:ps) (sNew, sOld) = update ps ((Set.insert (swap p) sNew), (Set.delete (swap p) sOld))
update [] (sNew, sOld) = (sNew, sOld)

calc :: PSet -> PSet -> Int
calc xs ys =
  let
    res = calcIter xs xs ys ys
  in 
    case res of
      (Lef x) ->  let (xs1, xs2) = Set.split (x, 0) xs
                      (ys1, ys2) = update (Set.toList xs1) (Set.empty, ys)
                  in (calc xs1 ys1) + (calc xs2 ys2)
      (Righ x) ->  let (xs1, xs2) = Set.split (x, 0) xs
                       (ys2, ys1) = update (Set.toList xs2) (Set.empty, ys)
                   in (calc xs1 ys1) + (calc xs2 ys2)
      (Up y) -> let (ys1, ys2) = Set.split (y, 0) ys
                    (xs1, xs2) = update (Set.toList ys1) (Set.empty, xs)
                in (calc xs1 ys1) + (calc xs2 ys2)
      (Down y) -> let (ys1, ys2) = Set.split (y, 0) ys
                      (xs2, xs1) = update (Set.toList ys2) (Set.empty, xs)
                  in (calc xs1 ys1) + (calc xs2 ys2)
      None -> 1

readPoint :: String -> Point
readPoint str = let (x:y:_) = words str in (read x, read y)
getSet :: Int -> IO (PSet, PSet)
getSet n = do
  xls <- forM [1..n] $ \k -> getLine >>= return . readPoint
  yls <- return $ fmap swap xls
  return ((Set.fromList xls), (Set.fromList yls))
main :: IO ()
main = do
  n <- getLine >>= return .read
  (xs, ys) <- getSet n
  putStrLn $ show (calc xs ys)

