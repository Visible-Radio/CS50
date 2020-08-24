from cs50 import get_int


# Cleaner way of picking out digits from a multidigit int
# From https://stackoverflow.com/a/39644726
# Similar to what I used in credit.c but with the advantage of
# "indexing" randomly into the int instead of taking bites
# out of it gradually from the right
# Divide by a power of 10 to move desired digit to 1s position
# Mod 10 to pick this digit off
# Zero indexed from right side

# def get_digit(number, n):
#    return number // 10**n % 10

# get_digit(987654321, 0)
# 1

# get_digit(987654321, 5)
# 6

def get_digit(number, n):
    return number // 10**n % 10

def count_digits(number):
    digit_counter=0
    while (True):
        digit_counter +=1
        subtrahend=number % 10
        difference=number-subtrahend
        if difference == 0:
            return digit_counter
        number=number/10

user_input = get_int("Number: ")
digit_count = count_digits(user_input)

# multiply every other digit by two starting with the second
# least significant
product_digit_sum=0
for i in range(1,digit_count,2):
    product=get_digit(user_input, i)*2
    product_digit_sum+=int((product%10 + ((product-product%10)/10)))

# Sum every other number starting with the least significant
digit_sum=0
for i in range(0,digit_count,2):
    digit_sum+=get_digit(user_input,i)

digit_sum+=product_digit_sum
if digit_sum % 10 == 0:

    #Mastercard and VISA may be 16 digits long
    if digit_count == 16:
        leading_digit=get_digit(user_input, 15)
        second_digit=get_digit(user_input, 14)

        #check if leading digit is a 4 for Visa
        if leading_digit==4:
            print("VISA")

        #check if leading digit is a 5 for Mastercard
        elif leading_digit==5:
            if (second_digit==1 or second_digit==2 or second_digit==3 or second_digit==4 or second_digit==5):
                print("MASTERCARD")

        else:
            print("INVALID")

    #Some VISAs are 13 digits long
    elif digit_count == 13:
        leading_digit=get_digit(user_input, 12)
        if leading_digit==4:
            print("VISA")
        else:
            print("INVALID")

    elif digit_count==15:
        #check for leading digits of 34 and 37
        leading_digit=get_digit(user_input,14)
        second_digit=get_digit(user_input,13)
        if leading_digit==3 and (second_digit==4 or second_digit==7):
            print("AMEX")
        else:
            print("INVALID")

    else:
        print("INVALID")

else:
    print("INVALID")







