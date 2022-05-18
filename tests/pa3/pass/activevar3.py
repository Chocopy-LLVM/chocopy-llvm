globl: [int] = None
a: [int] = None
i: int = 0
j: int = 0
globl = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
a = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

while i < 25:
    a[i] = i
    i = i + 1

i = 0
while i < 5:
    j = 0
    while j < 5:
        x: int = 0
        y: int = 0
        x = i + j
        y = i * j
        if a[x] > a[y]:
            globl[i * 5 + j] = a[x]
        else:
            globl[i * 5 + j] = a[y]
        j = j + 1
    i = 1 + i

i = 0
while i < 5:
    j = 0
    while j < 5:
        print(globl[i * 5 + j])
        j = j + 1
    i = 1 + i
