class A(object):
    x:int = 1
    def get_A(self: "A") -> int:
        return self.x
class B(A):
    def __init__(self: "B") -> object:
        pass
class C(B):
    z:bool = True
    def set_A(self: "C", val: int) -> object:
        self.x = val
a:A = None
b:B = None
c:C = None
a = A()
b = B()
c = C()
a.get_A(1)
b.get_Z()
c.set_A()
c.set_A(False)
c.set_A(None)

