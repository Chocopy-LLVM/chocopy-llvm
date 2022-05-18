x:int = 1
y:int = 2
z:int = 3
def foo(x:int) -> object:
    y:int = 4  # OK
    global z   # OK
    def qux() -> int: 
        y = 1  # Bad, nonlocal not declared
        return 0
    z = 1 # OK
    pass
def bar(x:int) -> int: 
    z = 1 # Bad, global not declared
    return x
foo(1)

