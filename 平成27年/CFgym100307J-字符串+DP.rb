fin = open('join.in', 'r')
fout = open('join.out', 'w')
n = fin.gets.to_i
dp = Array.new(n + 1, 1)
pre = Array.new(n + 1, 0)
pos = {}
dp[0] = 0
(1..n).each{ |i|
	str = fin.gets
	data = str.split ' '
	str = data[0].delete ':'
	data.each{ |s|
		next if s == str
		u = pos[s];
		if u and dp[u] + 1 > dp[i] then
			dp[i] = dp[u] + 1
			pre[i] = u
		end
	}
	unless pos[str] and dp[pos[str]] >= dp[i]
		pos[str] = i
	end
}
ans = 1
k = 1
dp.each_with_index{ |x, i|
	if x > ans then
		ans = x
		k = i
	end
}
v = []
while k > 0
	v.push k
	k = pre[k]
end
v.reverse!
fout.printf "%d\n",ans
v.each{ |x|
	fout.printf "%d ",x
}
fout.printf "\n"
fin.close
fout.close
