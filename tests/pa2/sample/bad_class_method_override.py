class A(object):
    def foo(self:"A", x:int) -> int:
        return x
    def bar(self:"A", x:int) -> int:
        return x
    def baz(self:"A", x:int) -> int:
        return x
    def qux(self:"A", x:int) -> int:
        return x
class B(A):
    def foo(self:"B", x:int) -> int: # OK override
        return 0
    def bar(self:"B") -> int: # Bad override
        return 0
    def baz(self:"B", x:int) -> bool: # Bad override
        return True
    def qux(self:"B", x:bool) -> int: # Bad override
        return 0
B()

