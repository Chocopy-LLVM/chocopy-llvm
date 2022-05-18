x:str = "abc"
a:str = ""
b:str = ""
c:str = ""

def str_get(s:str, i:int) -> str:
    return s[i]

a = str_get(x, 0)
b = str_get(x, 1)
c = str_get(x, 2)
print(a)
print(b)
print(c)
