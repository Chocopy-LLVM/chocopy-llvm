def foo() -> int:
    if True:
        return 1
    # Bad: all paths MUST return int

foo()
