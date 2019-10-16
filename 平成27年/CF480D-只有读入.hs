module Main where

import Data.List
import Data.Array
import qualified Data.Map.Strict as Map
import Control.Monad
import Text.ParserCombinators.Parsec

data Parcel = Parcel {
	iV :: Int,
	oV :: Int,
	wV :: Int,
	sV :: Int,
	vV :: Int
} deriving (Eq,Show)
instance Ord Parcel where
	(<=) a b = ((oV a) <= (oV b)) ||
		((oV a) == (oV b) && (iV a) >= (iV b))
type DP = [Map.Map Int Int]

-- parse inputs
cells = sepBy cell ((char ' ') <|> (char '\n'))
cell = many (noneOf " \n")
parseData :: String -> Either ParseError [String]
parseData = parse cells "(unknown)"

getData n (Right input) =
	sort $ getParcel n input []
	where
		getParcel 0 input acc = acc
		getParcel n (a:b:c:d:e:res) acc =
			getParcel (n - 1) res (Parcel{iV=read a,oV=read b,wV=read c,sV=read d,vV=read e}:acc)


