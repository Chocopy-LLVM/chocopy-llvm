i: int = 0
j: int = 0
k: int = 0
o: int = 0
p: int = 0
q: int = 0
a: int = 2
ret: int = 0
while i < 1000000:
    j = 0
    while j < 2:
        k = 0
        while k < 2:
            o = 0
            while o < 2:
                p = 0
                while p < 2:
                    q = 0
                    while q < 2:
                        if a > 1:
                            j = j + 1
                        ret = (a * a * a * a * a * a * a * a * a * a) / a / a / a / a / a / a / a / a / a / a
                        q = q + 1
                    p = p + 1
                o = o + 1
            k = k + 1
        j = j + 1
    i = i + 1
print(ret)
