from cs50 import get_int

while (True):
    n = get_int("Positive Integer: ")
    if n > 0 and n <=8:
        break

s =1

for i in range(n):
    for j in range(n):
        if j < (n-s):
            #print spaces
            print(" ", end="")
        else:
            #print blocks
            print("#", end="")

    print("  ", end="")

    for k in range(s):
        #print hashes
        print("#", end="")
    s+=1
    print()
