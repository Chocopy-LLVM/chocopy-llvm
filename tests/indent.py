#!/usr/bin/python3
import os

path = "./recursion"
files = os.listdir(path)
s = []
for file in files:
    if not os.path.isdir(file):
        # print(file)
        with open(path + "/" + file, "r") as f:
            count = 0
            iter_f = iter(f)
            s = ""
            for line in iter_f:
                # print(line[:8]==" INDENT ")
                while True:
                    if line[:8] == " INDENT ":
                        count += 1
                        line = line[8:].strip(" ")
                    elif line[:7] == "INDENT ":
                        count += 1
                        line = line[7:].strip(" ")
                    elif line[:6] == "INDENT":
                        count += 1
                        line = line[6:].strip(" ")
                    elif line[:8] == " DEDENT ":
                        count -= 1
                        line = line[8:].strip(" ")
                    elif line[:7] == "DEDENT ":
                        count -= 1
                        line = line[7:].strip(" ")
                    elif line[:6] == "DEDENT":
                        count -= 1
                        line = line[6:].strip(" ")
                    else:
                        break
                try:
                    for i in range(count):
                        s += "    "
                except:
                    pass
                s += line

                # s+="\n"
        with open(path + "/" + file, "w") as f:
            f.write(s)
# print(s) #打印结果
