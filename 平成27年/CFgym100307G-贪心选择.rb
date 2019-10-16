# 从文件中读入整数
def getIn(fin)
	buf = fin.gets.split ' '
	Proc.new{
		if buf.size == 0
			buf = fin.gets.split ' '
		end
		buf.shift.to_i
	}
end

# 计算斜坡上某一点的高度
def getHeight(x0, y0, x1, y1, x)
	((x - x0) * y1 + (x1 - x) * y0) / (x1 - x0)
end

# 反向计算所在位置
def getPos(x0, x1, y1, x2, y2, at, decrease)
	((at + decrease * x0) * (x2 - x1) + (x1 * y2 - x2 * y1)) / ((x2 - x1) * decrease + (y2 - y1))
end

# 实数二分答案
EPS = 1e-10
def binarySearch(left, right)
	while(right > left + EPS)
		mid = (left + right) / 2
		if (yield mid)
			right = mid
		else
			left = mid
		end
	end
	right
end

# 取得输入
fin = open('green.in', 'r')
fout = open('green.out', 'w')
getInt = getIn(fin)
n = getInt.call
m = getInt.call
alpha = (getInt.call).to_f / 180.0 * Math::PI
decrease = Math.tan(alpha)	#单位横坐标的下降率
h = Array.new(n){
	getInt.call
}
x = Array.new(m)
y = Array.new(m)
(0..m-1).each{ |i|
	x[i] = getInt.call
	y[i] = getInt.call
}
# 优化土地，移出最右端被完全遮盖的部分
m = 1 + (0..m - 1).max{ |i, j|
	(y[i] - y[j]) + decrease * (x[i] - x[j])
}
# 按高度顺序
order = (0..n - 1).sort{ |i, j|
	h[i] <=> h[j]
}
# 进行最优的放置
at = y[0].to_f	# 本轮放置点的日照高度，注意x[0]到at的全部日照高度都被接收了
atx = x[0].to_f	# 本轮的放置点的横坐标
ati = 0			# atx所在的线段(ati..ati+1)
res = 0.0		# 最终答案：已经接收到的日照高度
place = Array.new(n)
order.each{ |i|
	# 放置当前光板
	place[i] = atx
	at += h[i]
	res = [res, at - y[0]].max
	# 找到第一条不会被遮盖住的线段
	ati = (ati..m - 2).find{ |i|
		at < y[i + 1] + (x[i + 1] - x[0]) * decrease
	}
	if ati then
		# 找到放置位置
		atx = getPos(x[0], x[ati], y[ati], x[ati + 1], y[ati + 1], at, decrease)
	else
		# 取最后的位置
		# 注意之前已经把最后的高斜率下降的部分去除了，所以这个点放最高的杆就是最优解
		ati = m - 2
		atx = x[m - 1]
		at = y[m - 1] + (x[m - 1] - x[0]) * decrease
	end
}
fout.puts res
place.each{ |x|
	fout.puts x
}
fin.close
fout.close

