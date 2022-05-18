x:[int] = None

x = [1, 2, 3]
x[True]   # Bad
False[0]  # Bad
[][0]     # Bad
x[True] = x[False] = 1
