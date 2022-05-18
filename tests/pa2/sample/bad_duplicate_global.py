x:int = 1

def foo() -> object:
    pass

# `foo` cannot be redefined in the same scope
def foo() -> object:
    pass

# `print` cannot be redefined in the same scope
def print(val:object) -> object:
    pass

# `x` cannot be redefined in the same scope
x:int = 2


foo()
