#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>

// JTREKYAVOGDXPSNCUIZLFBMWHQ // test substitution key
// key passed to Main() at the command line
// preserve case from plain text.  Lower case in plaintext will be lower case in ciphertext.
// pass numbers and punctuation transparently
// validate user input - key must be 26 chars long, no numbers etc
// case depends on the plaintext. Upper or lower case chars in the key will be considered identical


int main(int argC, string argV[]){

    //printf("argC: %i  argV: %s \n", argC, argV[1]);


    char alphabet[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    string plainText;
    char key[26];
    int alphabetLength = 0;
    int plainTextLength = 0;
    int argVLength = 0;

    for (int i = 0; argV[1][i] != '\0'; i++){                       //validate that the user provided key is 26 chars
        argVLength += 1;
    }

    if (argVLength != 26){
        printf("Key must contain 26 alphabetic characters.\n");
        exit(1);
    }

    for (int i = 0; argV[1][i] != '\0'; i++){
        if (argV[1][i] >= 97 && argV[1][i] <=122){                  //check if current character is lower case
            key[i] = argV[1][i] -32;                                //convert to upper case and store in array key[]
        }else if (argV[1][i] >= 65 && argV[1][i] <=90){             //check if current character is upper case
            key[i] = argV[1][i];                                    //store in array key[]
        }else{                                                      //If the key has incorrect characters, ie non lower or upper case alphabetic characters
            printf("Key must contain 26 alphabetic characters.\n");
            exit(1);
        }
        }//end of loop

    plainText = get_string("plaintext: ");

   //printf("argVLength: %i \n", argVLength);

    for (int i = 0 ; alphabet[i] != '\0'; i++ ){        //determine length of alphabet.  This shouldn't change
        alphabetLength += 1;
    }

     for (int i = 0 ; plainText[i] != '\0'; i++ ){      //determine length of the user's input
        plainTextLength += 1;
    }



    //printf("alphabet length = %i \n", alphabetLength);
    //printf("plainText length = %i \n", plainTextLength);
    printf("ciphertext: ");

    for (int i=0; i < plainTextLength; i++){                //iterate through the user input

        if (plainText[i] >= 32 && plainText[i] <= 64){      //permit numbers and special characters in the ASCII table to pass transparently
            printf("%c", plainText[i]);
        }

        for (int j=0; j < alphabetLength; j++){             //iterate through the alphabet.  Remember alphabet is defined in UPPER CASE 65 through 90.

             if (plainText[i] == alphabet[j]){              //compare the user input at index i to alphabet at index j
                printf("%c", key[j]);                       //if there is a match, print out the contents of user provided key at position j

            }else if (plainText[i] == (alphabet[j] + 32)){
                printf("%c", key[j] + 32);
            }

        }



    }




    printf("\n");
    return 0;
}//END OF MAIN