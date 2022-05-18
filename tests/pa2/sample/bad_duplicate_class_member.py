class A(object):
    x:int = 1 # OK

    def foo(self: "A") -> int: # OK
        return 0

    x:int = 1 # Duplicate

    def foo(self: "A") -> int: # Duplicate
        return 0

A()
