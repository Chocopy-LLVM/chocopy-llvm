def test_vec(a: int, b: int, c: int, d: int, e: int, f: int, g: int, h: int, x: int) -> int:
    if x > 0:
        a = e + 4
        b = f + 4
        c = g + 4
        d = h + 4
    return a + b + c + d


print(test_vec(1, 2, 3, 4, 5, 6, 7, 8, 9))
