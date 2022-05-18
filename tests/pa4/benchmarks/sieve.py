# A resizable list of integers
class Vector(object):
    items: [int] = None
    size: int = 0

    def __init__(self:"Vector"):
        self.items = [0]

    # Returns current capacity
    def capacity(self:"Vector") -> int:
        return len(self.items)

    # Increases capacity of vector by one element
    def increase_capacity(self:"Vector") -> int:
        self.items = self.items + [0]
        return self.capacity()

    # Appends one item to end of vector
    def append(self:"Vector", item: int) -> object:
        if self.size == self.capacity():
            self.increase_capacity()

        self.items[self.size] = item
        self.size = self.size + 1

    # Appends many items to end of vector
    def append_all(self:"Vector", new_items: [int]) -> object:
        item:int = 0
        for item in new_items:
            self.append(item)

    # Removes an item from the middle of vector
    def remove_at(self:"Vector", idx: int) -> object:
        if idx < 0:
            return

        while idx < self.size - 1:
            self.items[idx] = self.items[idx + 1]
            idx = idx + 1

        self.size = self.size - 1

    # Retrieves an item at a given index
    def get(self:"Vector", idx: int) -> int:
        return self.items[idx]

    # Retrieves the current size of the vector
    def length(self:"Vector") -> int:
        return self.size

# A faster (but more memory-consuming) implementation of vector
class DoublingVector(Vector):
    doubling_limit:int = 1000

    # Overriding to do fewer resizes
    def increase_capacity(self:"DoublingVector") -> int:
        if (self.capacity() <= self.doubling_limit // 2):
            self.items = self.items + self.items
        else:
            # If doubling limit has been reached, fall back to
            # standard capacity increases
            self.items = self.items + [0]
        return self.capacity()

# Makes a vector in the range [i, j)
def vrange(i:int, j:int) -> Vector:
    v:Vector = None
    v = DoublingVector()
    
    while i < j:
        v.append(i)
        i = i + 1

    return v

# Sieve of Eratosthenes (not really)
def sieve(v:Vector) -> object:
    i:int = 0
    j:int = 0
    k:int = 0

    while i < v.length():
        k = v.get(i)
        j = i + 1
        while j < v.length():
            if v.get(j) % k == 0:
                v.remove_at(j)
            else:
                j = j + 1
        i = i + 1

# Input parameter
n:int = 50

# Data
v:Vector = None
i:int = 0

# Crunch
v = vrange(2, n)
sieve(v)

# Print
while i < v.length():
    print(v.get(i))
    i = i + 1

