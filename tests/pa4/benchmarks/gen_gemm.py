import random
def get_array():
    res:str="["
    for j in range(16):
        res+="["
        for i in range(1024):
            if i !=1023:
                res+="0,"
            else:
                res+="0"
        if j !=15:
            res+="],"
        else:
            res+="]"

    res+="]"
    return res

def get_random_input():
    for j in range(32):
        for i in range(1024):
            print(random())

print(get_array())