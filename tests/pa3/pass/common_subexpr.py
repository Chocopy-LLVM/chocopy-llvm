def remove_sub(a:int,b:int,c:int,d:int)-> int:
    a=b+c
    b=a-d
    c=b+c
    d=a-d
    return d
print(remove_sub(1,2,3,4))