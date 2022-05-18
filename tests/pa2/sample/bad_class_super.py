class A(B): # Super is not defined
    x:int = 1
z:int = 2
class B(z): # Super is not a class
    x:int = 1
class C(int): # Cannot extend special classes
    x:int = 1
A()
