next:int = 0

def next_int() -> int:
    global next
    next = next + 1
    return next

def make_list() -> [int]:
    return [next_int(), next_int(), next_int()]

print(make_list()[next_int() - 3])
