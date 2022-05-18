def concat(x:[int], y:[int]) -> [int]:
    return x + y

z:[int] = None
i:int = 0

z = concat([1,2,3], [4,5,6])

while i < len(z):
    print(z[i])
    i = i + 1

