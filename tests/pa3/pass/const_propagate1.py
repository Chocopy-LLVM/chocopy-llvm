i: int = 0
idx: int = 0

while i < 100000000:
    idx = 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1
    i = i + idx * idx * idx * idx * idx * idx * idx * idx / (idx * idx * idx * idx * idx * idx * idx * idx)
print(idx*idx)
