#include <stdio.h>
#include <cs50.h>


/*
input           // long storing actual full number to be checked
checkNum        // process variable, initially ==input, the number to be checked
subtrahend      // process variable, the result of mod opp
difference      // process variable, result of subtraction opp
quotient        // process variable, result of division opp
counter         // tracks the number of digits in input
sampleDigit     // bool that toggles to sample every other digit starting from the 2nd least significant
E1              // int storing the sum of every other digit starting from the least significant
E2              // int storing the sum of the digits of the products of every other digit of the input starting from the second least significant
Et              // int storing the final checksum of E1 + E2


12345   % 10   = 5          checkNum % 10               = subtrahend
12345   - 5    = 12340      checkNum - subtrahend       = difference
12340   / 10   = 1234       difference / 10             = checkNum


1234    % 10   = 4
1234    - 4    = 1230
1230    / 10   = 123


123     % 10   = 3
123     - 3    = 120
120     / 10   = 12


12      % 10   = 2
12      - 2    = 10
10      / 10   = 1


1       % 10   = 1
1       - 1    = 0          checkNum - subtrahend = 0   last digit detected when subtrahend = 0
0       / 10   = 0

*/

int main(void){

long checkNum;
int subtrahend;
long difference;
bool sampleDigit = false;
int counter = 0;
int c, s, d, E1, E2, Et, LDAMEX, LDMAST, LDVISA;
E1 = 0;
E2 = 0;

do{
        checkNum = get_long("Number: ");

    }
    while (checkNum<1);

while (checkNum > 0){

    counter++;
    subtrahend  = checkNum % 10;
    difference  = checkNum - subtrahend;
    checkNum    = difference / 10;

    //do the following when sampleDigit = true, ie, sampling every other digit starting with the 2nd least significant

    if (sampleDigit==true){

        c=subtrahend * 2;              // c = 8 * 2
        printf("c:%i ", c);
        s=c%10;                        // s  = 16 % 10    s==6
        printf("s:%i ", s);
        d=c-s;                         // d  = 16 - 6     d==10
        printf("d:%i ", d);
        c=d/10;                        // c  = 10 / 10     c==1
        printf("c1:%i ", c);
        E2=E2+s+c;                     // E2  = 0 + 6 + 1    E2==7
        printf("E2:%i \n", E2);


    }else{

        //do the following when sampleDigit = false, ie, sampling every other digit starting with the least significant

        E1 = E1 + subtrahend;
        printf("E1:%i \n", E1);

    }

    //when the counter reaches these thresholds, store checkNum.  This is how the leading digits are captured for comparison later

    if (counter == 13){
        LDAMEX = checkNum;
    }else if (counter == 14){
        LDMAST = checkNum;
    }else if (counter == 12 || counter == 15){
        LDVISA = checkNum;
    }

    printf("checkNum:%li subtrahend:%i count:%i\n", checkNum, subtrahend, counter);
    sampleDigit = !sampleDigit;

}//End While Loop

    Et = E1 + E2;           // Final checksum
    printf("Checksum: %i Length: %i \n", Et, counter);

    //Check the length of the number, the leading digits and the result of the checksum to determine card type and validity

    if ((LDAMEX == 34 || LDAMEX == 37) && (counter == 15) && (Et % 10 == 0)){
        printf("AMEX\n");
    }else if ((LDMAST == 51 || LDMAST == 52 || LDMAST == 53 || LDMAST == 54 || LDMAST == 55) && (counter == 16) && (Et % 10 == 0)){
        printf("MASTERCARD\n");
    }else if ((LDVISA == 4) && (counter == 13 || counter == 16) && (Et % 10 == 0)){
        printf("VISA\n");
    }else{
        printf("INVALID\n");
    }


}//END OF PROGRAM