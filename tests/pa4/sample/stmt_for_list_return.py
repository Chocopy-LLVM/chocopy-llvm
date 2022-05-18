def print_list(z:[int]) -> object:
    x:int = 0
    for x in z:
        print(x)
        if x >= 30:
            return

print_list([10,20,30,40])
