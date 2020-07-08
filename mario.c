#include <stdio.h>
#include <cs50.h>

int main(void){

int n = 0;          // This will be user input.  It corresponds to the height of the pyramid (number of rows) and the width of each half of the pyramid
int s = 1;          // This will be the number of hashes to be printed for the current line.


    do{
        n = get_int("Enter Pyramid Height: ");

    }
    while (n<1 || n>8);


    for (int i=0; i<n; i++){        // Outer loop is a row index with row 0 at the top of the pyramid

        for (int j=0; j<n; j++){    //FIRST Inner loop responsible for printing contents of first half of individual rows
            if (j < (n-s)){
                printf(" ");
            }else{
                printf("#");
            }
        }//END OF FIRST INNER LOOP

        printf("  ");

        for (int k=n; k>0; k--){    //SECOND Inner loop responsible for printing contents of second half of individual rows
            if (k > (n-s)){
                printf("#");
            }else{
                printf(" ");
            }
        }//END OF SECOND INNER LOOP

    printf("\n");
    s++;                            // Increment s once per row.  s is deducted from n in the inner loops to determine when to print " " or "#"
    }//END OF OUTER LOOP

printf("\n");

}//END OF PROGRAM



/*
PSEUDOCODE

*/