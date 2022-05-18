class a(object):
    def __init__(self:"a"):
        print("a")
class b(a):
    def __init__(self:"b"):
        print("b")
class c(b):
    def __init__(self:"c"):
        print("c")
class d(c):
    def __init__(self:"d"):
        print("d")
class e(d):
    def __init__(self:"e"):
        print("e")
class g(e):
    def __init__(self:"g"):
        print("g")
sb:a=None
sb=g()