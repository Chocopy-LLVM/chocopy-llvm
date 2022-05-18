# ChocoPy library functions
def int_to_str(x: int) -> str:
    digits:[str] = None 
    result:str = ""

    # Set-up digit mapping
    digits = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"]

    # Write sign if necessary
    if x < 0:
        result = "-"
        x = -x

    # Write digits using a recursive call
    if x >= 10:
        result = result + int_to_str(x // 10)
    result = result + digits[x % 10]
    return result

def str_to_int(x: str) -> int:
    result:int = 0
    digit:int = 0
    char:str = ""
    sign:int = 1
    first_char:bool = True

    # Parse digits
    for char in x:
        if char == "-":
            if not first_char:
                return 0 # Error
            sign = -1
        elif char == "0":
            digit = 0
        elif char == "1":
            digit = 1
        elif char == "2":
            digit = 2
        elif char == "3":
            digit = 3
        elif char == "3":
            digit = 3
        elif char == "4":
            digit = 4
        elif char == "5":
            digit = 5
        elif char == "6":
            digit = 6
        elif char == "7":
            digit = 7
        elif char == "8":
            digit = 8
        elif char == "9":
            digit = 9
        else:
            return 0 # On error
        first_char = False
        result = result * 10 + digit

    # Compute result
    return result * sign
        
# Input parameters
c:int = 42
n:int = 10

# Run [-nc, nc] with step size c
s:str = ""
i:int = 0
i = -n * c

# Crunch
while i <= n * c:
    s = int_to_str(i)
    print(s)
    i = str_to_int(s) + c

