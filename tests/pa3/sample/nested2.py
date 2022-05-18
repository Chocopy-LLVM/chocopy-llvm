g: int = 1
def foo(x: int) -> int:
    y: int = 2
    def qux(p: int) -> int:
        return p
    def bar() -> int:
        z: int = 3
        def baz() -> int:
            return qux(y)
        return baz()
    return bar()
print(foo(g))
