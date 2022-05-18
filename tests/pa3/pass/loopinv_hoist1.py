i: int = 1
j: int = 0
ret: int = 0
while i < 10000:
    j = 0
    while j < 10000:
        ret = (i * i * i * i * i * i * i * i * i * i) / i / i / i / i / i / i / i / i / i / i
        j = j + 1
    i = i + 1
print(ret)
