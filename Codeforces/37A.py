n = raw_input()
s = raw_input().split()
d = {}
for i in s:
	d[i] = d.get(i, 0) + 1
print max(d.values()), len(d)
