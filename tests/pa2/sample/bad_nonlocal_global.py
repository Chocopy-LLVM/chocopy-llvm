x:int = 1
y:int = 2
z:int = 3

def foo() -> object:
    global x # OK
    nonlocal y # No such nonlocal var
    global w # No such global var
    global int # No such global var

    z:bool = True # OK

    def bar() -> object:
        global x # OK
        nonlocal z # OK
        nonlocal y # No such nonlocal var
        global foo # No such global var
        nonlocal bar # No such nonlocal var

        pass

    bar()

foo()

