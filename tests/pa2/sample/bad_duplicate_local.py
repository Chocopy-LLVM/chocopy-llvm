x:int = 1
y:int = 2
z:int = 3
def foo(x:int) -> object:
    y:int = 4  # OK
    x:int = 5  # Duplicate declaration
    global z   # OK
    global y   # Duplicate declaration
    def x() -> int: # Duplicate declaration
        return 0
    pass
def bar(x:int, x:int) -> int: # Duplicate params
    return x
foo(1)

