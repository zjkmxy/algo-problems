name = "napk"
count = 10

system 'g++ tmp.cpp -o tmp.exe -std=c++11 -O2'
if $?.to_i != 0 then
	system "pause"
	exit
end
(0..count).each { |i|
	puts "Testcase #{i}:"
	system "tmp.exe <#{name}#{i}.in >my.out"
	system "fc #{name}#{i}.out my.out"
	if $?.to_i != 0 then
		system "pause"
	end
}
system "pause"
