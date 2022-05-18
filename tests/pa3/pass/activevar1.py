def gcd(u: int, v: int) -> int:
    if v == 0:
        return u
    else:
        return gcd(v, u - u / v * v)
x: int = 7
y: int = 8
temp: int = 0
if x < y:
    temp = x
    x = y
    y = temp
temp = gcd(x, y)
print(temp)
