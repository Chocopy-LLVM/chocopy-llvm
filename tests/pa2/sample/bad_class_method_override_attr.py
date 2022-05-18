class A(object):
    f:int = 3
class B(A):
    def f(self:B) -> int:
        return 3
A()
