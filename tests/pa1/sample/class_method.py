class Foo(object):
    x:int = 0
    def set(self:"Foo", x:int) -> object:
        self.x = x
f = Foo()
f.set(1)
print(f.x)
