i: int = 0
j: int = 0
a: int = 2
ret: int = 0
while i < 10000000:
    j = 0
    while j < 2:
        ret = (a * a * a * a * a * a * a * a * a * a) / a / a / a / a / a / a / a / a / a / a
        j = j + 1
    i = i + 1
print(ret)
