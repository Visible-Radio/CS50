from cs50 import get_string

def countLetters(userInput):
    letterCount =0
    for character in userInput:
        if character.isalpha():
            letterCount+=1
    return letterCount

def countSentences(userInput):
    sentenceCount = userInput.count('!')
    sentenceCount += userInput.count('?')
    sentenceCount += userInput.count('.')
    return sentenceCount

def calcGrade(userInput):
    # calculation
    # index = 0.0588 * L - 0.296 * S - 15.8
    # L is letters per 100 words
    # x is sentences per 100 words
    w = (1+userInput.count(' '))
    l = (countLetters(userInput)*100)/w
    x = (countSentences(userInput)*100)/w
    index = round((0.0588 * l) - (0.296 * x) - 15.8)
    return index
    #print(f"Raw index: {index}")

userInput = get_string("Enter text to be analysed: ")
#print(f"Letters: {countLetters(userInput)}")
#print(f"Words:{1+userInput.count(' ')}")

grade = calcGrade(userInput)

if grade < 0:
    print("Before Grade 1")

elif grade > 16:
    print("Grade 16+")

else:
    print(f"Grade {grade}")

