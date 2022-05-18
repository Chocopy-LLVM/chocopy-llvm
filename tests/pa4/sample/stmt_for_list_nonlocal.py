x:int = 0
def crunch(zz:[[int]]) -> object:
    z:[int] = None
    global x
    def make_z() -> object:
        nonlocal z
        for z in zz:
            pass # Set z to last element in zz

    make_z()
    for x in z:
        pass # Set x to last element in z

crunch([[1,2],[2,3],[4,5],[6,7]])
print(x) 
