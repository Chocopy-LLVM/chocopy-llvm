class A(object):
    a:int = 42

a1:A = None
a2:A = None
a3:A = None
a4:A = None

a1 = A()
a2 = a1
a3 = A()

print(a1 is a1)
print(a1 is a2)
print(a1 is a3)
print(a1 is a4)
print(a1 is None)
print(a4 is None)
print(None is None)
