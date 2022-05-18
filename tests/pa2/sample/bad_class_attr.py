class A(object):
    x:int = 1

    def foo(self:"A") -> int:
        return 0

class B(A):
    x:int = 2  # Bad
    foo:str = "" # Bad

A()


