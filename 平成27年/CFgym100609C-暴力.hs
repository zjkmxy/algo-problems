module Main where

import Data.List
import Control.Monad
import Data.Char
import System.IO

-- 列表的循环移位
shift (x:xs) = xs ++ [x]
shift [] = []

-- 取出列表的某一段元素
takeBetween a b = take (b - a + 1) . drop a

-- 循环一圈，看移位后是否重合
singleRound ans1 ans2 =
	let
		mid1 = takeBetween 1 4 ans1
		mid2 = takeBetween 1 4 ans2
		testRound _ _ 0 = False
		testRound t1 t2 n = if t1 == t2 then True else testRound t1 (shift t2) (n - 1)
	in
		if (ans1!!0 /= ans2!!0) || (ans1!!5 /= ans2!!5)
		then False
		else if testRound mid1 mid2 4
			then True
		-- 如果1号等于6号，需要反向测试颜色：DAABC AABDD AACDD CABDD BACDD DAABC
			else (ans1!!0 == ans1!!5) && (testRound mid1 (reverse mid2) 4)

-- 完全的重合性测试（枚举1号点）
-- 这是必须的，因为：DABDD DBBDD DABDD ABBDD BABDD BABDD
fullTest ans1 ans2 =
	let
		s1 (a1:a2:a3:a4:a5:a6:_) = [a1,a2,a3,a4,a5,a6]
		s2 (a1:a2:a3:a4:a5:a6:_) = [a2,a6,a3,a1,a5,a4]
		s3 (a1:a2:a3:a4:a5:a6:_) = [a3,a6,a4,a1,a2,a5]
		s4 (a1:a2:a3:a4:a5:a6:_) = [a4,a1,a3,a6,a5,a2]
		s5 (a1:a2:a3:a4:a5:a6:_) = [a5,a2,a1,a4,a6,a3]
		s6 (a1:a2:a3:a4:a5:a6:_) = [a6,a2,a5,a4,a3,a1]
		allshift = [s1, s2, s3, s4, s5, s6]
		dotest s = singleRound ans1 (s ans2)
	in
		any dotest allshift

-- 非首元素排序
restSort (a1:as) = a1 : (sortBy compare as)

-- 根据答案构筑视角
makeViews ans =
	let
		s1 (a1:a2:a3:a4:a5:a6:_) = [a1,a2,a3,a4,a5]
		s2 (a1:a2:a3:a4:a5:a6:_) = [a2,a1,a3,a6,a5]
		s3 (a1:a2:a3:a4:a5:a6:_) = [a3,a1,a2,a4,a6]
		s4 (a1:a2:a3:a4:a5:a6:_) = [a4,a1,a3,a6,a5]
		s5 (a1:a2:a3:a4:a5:a6:_) = [a5,a1,a2,a4,a6]
		s6 (a1:a2:a3:a4:a5:a6:_) = [a6,a2,a3,a4,a5]
		allview = [s1, s2, s3, s4, s5, s6]
		doView s = restSort (s ans)
	in
		sortBy compare (map doView allview)

-- 更新答案
updateAnswer _ _ (a1:a2:_) = [a1, a2]
updateAnswer sort_input ans ans_list =
	if ((makeViews ans) == sort_input)
		&& (not $ any (fullTest ans) ans_list)
	then ans:ans_list
	else ans_list

-- 暴力枚举可能的答案
enumAnswer sort_input begin_list = 
	let maybeAns = permutations begin_list
	in foldr (updateAnswer sort_input) [] maybeAns

-- 根据输入得到答案
getAnswer line =
	let
		sort_input = sortBy compare (map restSort (words line))
		head_letter = head (transpose sort_input)
		answers = enumAnswer sort_input head_letter
	in if length answers == 0
		then "Impossible\n"
		else concat $ map (\x -> x ++ "\n") answers

-- 主函数
main :: IO ()
main = do
	line <- readFile "cube.in"
	writeFile "cube.out" (getAnswer line)

