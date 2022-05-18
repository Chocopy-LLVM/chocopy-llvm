c:int=0
a:int=0
b:int=0
d:int=0
f:int=0
g:int=0
while c<100000000:
    a=9*5*5*2+7
    b=a*a*a*a*a*a
    d=b*b*b*b*b*b
    f=d*d*d*d*d*d
    g=f*f*f*f*f*f
    c=c+1
print(g)