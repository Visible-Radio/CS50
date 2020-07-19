#include <cs50.h>
#include <stdio.h>
#include <math.h>

//function declarations
int countLetters(string userInput);
int countWords(string userInput);
int countSentences(string userInput);
int grade(string userInput);



int main(void)
{



//input section

string userInput = get_string("Enter text to be analysed: ");

//printf("Letters: %i \n", countLetters(userInput));

//printf("Words: %i \n", countWords(userInput));

//printf("Sentences: %i \n", countSentences(userInput));

int gradeo=grade(userInput);

if (gradeo < 0)
{
    printf("Before Grade 1\n");
}
else if (gradeo > 16)
{
    printf("Grade 16+\n");
}
else
{
    printf("Grade %i\n", gradeo);
}


//printf("%i\n", grade(userInput));

}//end of main()



//function definitions


int countLetters(string userInput)
{

    int letterCount = 0;
    for (int i=0; userInput[i] != '\0'; i++)
    {
       if ((userInput[i] >= 65 && userInput[i] <=90) || (userInput[i] >= 97 && userInput[i] <=122))
       {
           letterCount ++;
       }
    }
    return letterCount;
}


int countWords(string userInput)
{
    int spaceCount = 0;
    for (int i=0; userInput[i] != '\0'; i++)
    {
        if (userInput[i] == 32)
        {
            spaceCount++;
        }
    }
    return spaceCount + 1;
}



// count punctuation
int countSentences(string userInput)
{
    int sentenceCount = 0;
    for (int i=0; userInput[i] != '\0'; i++)
    {
        if (userInput[i] == '!' || userInput[i] == '?' || userInput[i] == '.')
        {
            sentenceCount++;
        }
    }
    return sentenceCount;
}

// calculation
// index = 0.0588 * L - 0.296 * S - 15.8
// L is letters per 100 words
// S is sentences per 100 words

int grade(string userInput)
{
    float letters = countLetters(userInput);
    float words = countWords(userInput);
    float sentences = countSentences(userInput);
    float L = (letters*100)/words;
    float S = (sentences*100)/words;
    float index = (0.0588 * L) - (0.296 * S) - 15.8;
    int grade = roundl(index);
    //printf("Letters per 100 words: %f\n", L);
    //printf("Sentences per 100: %f\n", S);
    //printf("index: %f\n", index);
    return grade;
}




