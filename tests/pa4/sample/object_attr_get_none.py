class A(object):
    a:int = 42

class B(A):
    b:bool = True

    def __init__(self:"B"):
        print("B")

a:A = None
b:B = None

a = B()
print(a.a)
print(b.a)
print(b.b)
