# Test of 'input' function.

s: str = ""

s = input()
while len(s) > 0:
    print(s)
    s = input()
