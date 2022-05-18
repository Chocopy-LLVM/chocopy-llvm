x: [int] = None
x = [4,7,9,2,0,6,1,3,5,8]
i:int=0
def minloc(a: [int], low: int, high: int) -> int:
    i: int = 0
    x: int = 0
    k: int = 0
    k = low
    x = a[low]
    i = low + 1
    while i <= high:
        if a[i] < x:
            x = a[i]
            k = i
        i = i + 1
    return k
def sort(a: [int], low: int, high: int) -> object:
    global i
    x: int = 0
    k: int = 0
    i = low
    while i < high - 1:
        t: int = 0
        k = minloc(a, low, high)
        t = a[k]
        a[k] = a[i]
        a[i] = t
        i = i + 1
    return
sort(x,0,len(x))
i=0
while i<10:
    print(x[i])
    i=i+1