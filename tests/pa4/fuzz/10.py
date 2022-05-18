arr:[int] =None
i:int=0
def insertionSort(arr:[int]):
    global i
    key:int=0
    j:int=0
    count:int = 0
    for i in arr:
        key = i
        j = count - 1
        while j >=0 and key < arr[j] :
                arr[j+1] = arr[j]
                j = j- 1
        arr[j+1] = key
        count=count+1
arr=[12, 11, 13, 5, 6]
insertionSort(arr)
print ("Sorted array is:")
for i in arr:
    print (i)