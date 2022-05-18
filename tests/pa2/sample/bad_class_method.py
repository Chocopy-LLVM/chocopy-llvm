class A(object):
    x:int = 1
    def foo(self:"A") -> int: # OK
        return 0
    def bar() -> int: # Needs self param
        return 0
    def baz(self:int) -> int: # Incorrect self type
        return 0
A()

