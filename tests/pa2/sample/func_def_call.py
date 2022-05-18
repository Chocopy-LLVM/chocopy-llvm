def foo(x:str, y:bool) -> int:
    return bar()

def bar() -> int:
    return 1

foo("Hello", False)
