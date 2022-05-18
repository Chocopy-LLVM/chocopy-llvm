a:str = "no"
b:str = "o"
c:str = ""
d:str = ""
e:str = ""

def cat2(a:str, b:str) -> str:
    return a + b

def cat3(a:str, b:str, c:str) -> str:
    return a + b + c

c = cat2(b, a)
d = cat2(a, a)
e = cat3(a, b, cat2(b, b))

print(c)
print(d)
print(e)
