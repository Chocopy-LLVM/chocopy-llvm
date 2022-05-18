def test() -> int:
    a: [int] = None
    i: int = 0
    a = [1, 2, 3, 4, 1, 2, 3, 4]
    for i in [1, 2, 3, 4, 5, 6, 7, 8]:
        a[i] = a[i] + 1
    return a[0]


print(test())
