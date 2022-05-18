opa:int=0
opb:int=0
opc:int=0
opd:int=0
i:int=0

def max()->int:
    global opa
    global opb
    global opc
    global opd
    opa = 0*1*2*3*4*5*6*7
    opb = 1*2*3*4*5*6*7*8
    opc = 2*3*4*5*6*7*8*9
    opd = 3*4*5*6*7*8*9*10
    if opa<opb:
        if opb<opc:
            if opc<opd:
                return opd
    return 0

while i<200000000:
    max()
    i=i+1

print(opa)
print(opb)
print(opc)
print(opd)