#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
    //int margin;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;


// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void print_preferencess(int preferences[MAX][MAX]);
void print_ranks(int ranks[], string output[MAX]);
void print_pairs();
void print_preferences();
void mergeSort(pair array[], int left, int right);
void merge(pair array[], int pleft, int qmiddle, int rright);
void checkLoser(int loser, int winner, int originalLoser, int originalWinner);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++) // for each voter...  i steps through voters
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];                 // set up an array of the correct size for the number of candidates.  this will be overwritten by the next voter

        // Query for each rank
        for (int j = 0; j < candidate_count; j++) // for each voter, for each rank.  j steps through the voter's ordered list
        {
            string name = get_string("Rank %i: ", j + 1);   //number the voter's list from 1, not 0

            if (!vote(j, name, ranks)) // if vote() returns false do the following... // pass j, name and ranks to vote function
            {
                printf("Invalid vote.\n");
                return 3;
            }

        } // END OF j LOOP

        // Since ranks[] gets overwritten each pass through the outer i loop
        // its contents need to be moved somewhere to be stored before
        // collecting the next voters list of votes

        record_preferences(ranks);
        print_ranks(ranks, candidates); // added for troubleshooting and testing
        printf("\n");
        print_preferences(ranks);   // added for troubleshooting and testing

        printf("\n");

    } // END OF i LOOP


    add_pairs();
    //print_pairs();
    sort_pairs();
    print_pairs();

    lock_pairs();
/*
    print_winner();
    return 0;
*/
}//END OF MAIN



bool vote(int rank, string name, int ranks[])

    // Update ranks given a new vote.
    // vote() receives incoming parameters rank (from j above), name, and ranks[]
    // where rank is the current voter's ranking of the candidate passed as 'name'
{
    //  iterate through the array of storing the names of the candidates, candidates[]

    for (int i = 0 ; i < candidate_count; i++)
    {

     if (strcmp(candidates[i], name) == 0)

        {
            ranks[rank] = i;                 // store the index corresponding to the candidate in the position in ranks[] designated by rank (j from loop in main)
            return true;
        }


    }//END OF i LOOP

    return false;

}//END OF FUNCTION vote()

void record_preferences(int ranks[])
{
  int comparitor1;
  int comparitor2;

  for (int i=0;i<candidate_count;i++)
    {
        for (int j=0 ;j<candidate_count;j++)
        {

            if (i!=j) // check for self-comparison
                {
                    /*
                    populate the 2D array left to right top to bottom
                    i and j are grid coordinates

                    lets start in row 0(Alice). i=0, j=0
                    We skip Alice because i=j, i=0, j++ j=1
                    Now i!=j.  Determine if we should increment at i=0, j=1

                    if the j column candidate is ranked lower than the i row candidate
                    j = 1 for bob
                    SEARCH through the CONTENTS of ranks
                    find the array position of the candidate represented  by j(1) - Bob
                    turns out it's 0
                    find the array position of the candidate represented by i(0) - Alice
                    turns out it's 1

                    is 0 less than 1?
                    no? that means Bob is ranked higher
                    do not increment at i=0, j=1

                    */

                    for (int k=0; k < candidate_count;k++)
                    {
                        if (ranks[k] == j)
                            {
                                comparitor1 = k;
                            }
                        else if (ranks[k] == i)
                            {
                                comparitor2 = k;
                            }

                    } // END OF K LOOP

                    if (comparitor1 > comparitor2)
                    {
                         preferences[i][j] ++;
                    }

                } // END check for self-comparison

        } // END OF J LOOP

    } // END OF I LOOP



    return;


} // END OF FUNCTION record_preferences()




void add_pairs(void)
{
    // Record pairs of candidates where one is preferred over the other
    // examine the preferences table and populate the pairs array with .winner and .loser for each [index]
    // left to right, top to bottom
    // Alice v Bob, Alice v Charlie Alice v Sally, etc
    // compare preferences [0][1] to preferences [1][0]
    // if preferences [0][1] > preferences [1][0]
    // pairs[0].winner = candidates[0], pairs[0].loser = candidates[1]
    // compare preferences [0][2] to preferences [2][0]
    // if preferences [0][2] > preferences [2][0]
    // pairs[1].winner = candidates[0], pairs[1].loser = candidates[2]
    // update pair count
    // tied candidates do not get added to the array

    pair_count = 0;
    int pairs_index=0;
    for (int i=0; i < candidate_count-1; i++) // iterate through rows of preferences
        {
            for (int j=i; j < candidate_count; j++) // iterate through columns of preferences
                {
                    if (i!=j && preferences[i][j] != preferences [j][i]) // check for self-comparison and tied candidates
                    {
                        if (preferences[i][j] > preferences[j][i])
                            {
                                pairs[pairs_index].winner = i;
                                pairs[pairs_index].loser = j;
                                //pairs[pairs_index].margin = preferences[i][j]-preferences[j][i]
                            }
                            else
                            {
                                pairs[pairs_index].winner = j;
                                pairs[pairs_index].loser = i;
                                //pairs[pairs_index].margin = preferences[j][i]-preferences[i][j]
                            }
                        // print out on the fly for testing purposes
                        printf("Index: %i  Winner: %10s  Loser: %10s    Strength: %i\n", pairs_index, candidates[pairs[pairs_index].winner], candidates[pairs[pairs_index].loser], preferences[pairs[pairs_index].winner][pairs[pairs_index].loser]);

                        pair_count++;
                        pairs_index++;
                    } // END check for self comparison && tied candidates
                } // END OF J LOOP // preferences column index //
        } // END OF I LOOP // preferences row index //



    return;
} // END OF FUNCTION add_pairs()



// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{

    // void mergeSort(int array[], int left, int right) where left is the index to start at, right the index to stop at
    //


    mergeSort(pairs, 0, pair_count-1);


    return;
} // END OF FUNCTION sort_pairs()



void merge(pair array[], int pleft, int qmiddle, int rright)
{
    // Merge portion of merge sort opperation
    // merge needs:
    // array being sorted
    // the first and last indecies of the first sub array
    // the last index of the second sub array
    // compare two sub arrays sampled from array[] and combine their contents into array[] in the correct order
    // mergeSort() passes the coordinates to be sampled from array[] to generate the sub arrays


    // determine SIZE of sub arrays
    // this is +1 to the number of indexes
    int subLeftSize = (qmiddle - pleft) + 1;
    int subRightSize = rright - qmiddle;

    // generate two sub arrays
    pair subLeft[subLeftSize];
    pair subRight[subRightSize];

    // move contents from array being sorted to the sub arrays

    for (int i=0; i < subLeftSize; i ++)
    {
        subLeft[i].winner = array[pleft+i].winner;
        subLeft[i].loser = array[pleft+i].loser;

    }

    for (int j=0; j < subRightSize; j ++)
    {
        subRight[j].winner = array[qmiddle+1+j].winner;
        subRight[j].loser = array[qmiddle+1+j].loser;

    }

    // combine the sub arrays into the main array with their elements correctly ordered.  for tideman we need to largest number first...
    // iterate through the main array, populating it with the elements from the subs

    int i = 0;
    int j = 0;
    int k = pleft; // counter for the array being sorted.  setting = to left maybe required to avoid overwriting what has already being sorted as subsequent sub arrays are incorporated



    while (i < subLeftSize && j < subRightSize)
    {

        if (preferences[subLeft[i].winner][subLeft[i].loser] >= preferences[subRight[j].winner][subRight[j].loser])
        {
            array[k].winner = subLeft[i].winner;
            array[k].loser = subLeft[i].loser;
            i++;                                // we just inserted an element from subLeft, so point at the next eligible element
        }
        else
        {
            array[k].winner = subRight[j].winner;
            array[k].loser = subRight[j].loser;
            j++;                                 // since we just inserted an element from subRight, move its pointer forward to cue up the next element
        }
        k++;                                     // we have just added an element from either subLeft or SubRight, so ++ the main array index
    }

    // these arrays are all different lengths and their indexes are advanced differently depending on their contents
    // there may be left overs in either one of the sub arrays when all the elements from the shorter of the two have been incorporated into the main array
    // since the sub arrays have already been sorted, the remnants get added directly to the main array where we left off with [k]
    // we need to finish incrementing i and j to their last index.  i < subLeftSize, j < subRightSize

    while (i < subLeftSize)
    {
        array[k].winner = subLeft[i].winner;
        array[k].loser = subLeft[i].loser;
        i++;
        k++;
    }

    while (j < subRightSize)
    {
        array[k].winner = subRight[j].winner;
        array[k].loser = subRight[j].loser;
        j++;
        k++;
    }



} // END OF FUNCTION merge()




void mergeSort(pair array[], int left, int right)
{
    if (left < right)
    {
        int middle = (left+right)/2;

        mergeSort(array, left, middle);
        mergeSort(array, middle+1, right);

        merge(array, left, middle, right);

    }



} // END OF FUCNTION mergeSort()



// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    /*
    Look at pairs[0]
    Is the loser in this pair the winner in any locked pair?
    go check...
    If not, lock the pair.


    create a separate function...something like:

    checkLoser(target, pairs_index)

    that takes as input the current place in the sorted pairs array
    and the id of the losing candidate from the pair to look for in the .winner slots in the rest of the sorted pairs array

    checkLoser will be called recursively until it runs through the entire pairs array and doesn't turn up the loser in a the winning position
    or, it finds itself in the losing position again...
    */


    for (int i=0; i < pair_count; i++)
    {

    checkLoser(pairs[i].loser, pairs[i].winner, pairs[i].loser, pairs[i].winner);
    printf("%s  %s  %s\n", candidates[pairs[i].winner], candidates[pairs[i].loser], locked[pairs[i].winner][pairs[i].loser] ? "locked" : "");

    /*

        if (checkLoser(pairs[i].loser, pairs[i].winner, i) == 0)
            {
                locked[pairs[i].winner][pairs[i].loser] = true;     // Confirmed to work with check50

            }

    printf("%s  %s  %s\n", candidates[pairs[i].winner], candidates[pairs[i].loser], locked[pairs[i].winner][pairs[i].loser] ? "locked" : "");


    */

    }

    return;

}

void checkLoser(int loser, int winner, int originalLoser, int originalWinner)
{


    for (int i = 0; i < pair_count; i ++)
    {

    /*
    check to see if its locked
    if it is, recurse
    it is not locked, lock originals
    */

    // need a dang base case

    if (pairs[i].winner == loser && loser == originalLoser && (locked[pairs[i].winner][pairs[i].loser] ==true))
    {
        return;
    }




    if (locked[pairs[i].winner][pairs[i].loser] ==true)
        {
            if (pairs[i].winner == loser)
            {

                printf("recursion...it's happening \n");
                checkLoser(pairs[i].loser, pairs[i].winner, originalLoser, originalWinner);

            }
        }
        else    // if not locked, lock the originals and return
        {
            locked[originalWinner][originalLoser] = true;
            return;
        }



    }



}



// Print the winner of the election
void print_winner(void)
{

    return;
}


void print_ranks(int ranks[], string output[MAX])
{
    // Print the ranks table for troubleshooting purposes
    printf("\n");
    printf("%9s", "Index:    ");
    for (int i =0; i < candidate_count; i++)
    {
        printf("%10i ", i);
    }
    printf("\n");

    printf("Contents: ");
    for (int k=0; k < candidate_count; k ++)
    {
        printf("%10s %i",candidates[ranks[k]], ranks[k]);
    }
    printf("\n");


} // END OF FUNCTION print_ranks


void print_preferences(int ranks[])
{
    // Print the preferences table for troubleshooting purposes
    printf("candidate_count: %i\n\n", candidate_count);

    //Determine the length of the longest string stored in candidates[]
    int longest_name =0;
    int comp = 0;
    for (int i=0; i < candidate_count; i++)
    {
        comp = strlen(candidates[i]);
        if (comp > longest_name)
            longest_name = strlen(candidates[i]);

    }

    //printf("longest_name: %i\n\n", longest_name);
    int table_width = (longest_name + 1) + (candidate_count + ((candidate_count-1) *3) );
    //printf("table_width: %i\n\n", table_width);

    for (int i=0; i < longest_name; i ++)
    {
        // print the blank space
        for (int k=0; k < longest_name + 1; k++)
        {
            printf("%1c", ' ');
        }


        for (int j=0; j < candidate_count; j ++)
            {
                //if we are still within the bounds of the name being printed
                if (strlen(candidates[j]) > i )
                {
                    printf("%c   ", candidates[j][i]);

                }
                else
                {
                    //if not, print spaces
                    printf("%2s","    ");
                }

            }
            printf("\n"); // Carriage return after each row of letters per iteration of i
    }

    printf("\n");

    for (int l=0; l < candidate_count; l++) // l is line index
    {

        for (int m=0; m < table_width;m++) // m location of print head in row
        {
            //if we are still within the bounds of the name being printed
            if (strlen(candidates[l]) > m )
                printf("%c", candidates[l][m]);

            // if we are done printing the name, but this name is not the longest
            else if (m < longest_name + 1)
                printf("%1c", ' ');


        }

        for (int n=0; n < candidate_count; n++)
        {
            // print the data from preferences for the current row
            printf("%i   ", preferences[l][n]);


        }
         printf("\n"); // Carriage return after each row of letters per iteration of l
    }


} // END OF FUNCTION print_preferencess

void print_pairs()
{
    // Let's see if I sorted the pairs array correctly
    // we need to see the winner for each pair and their tally of votes

    printf("\n");
    printf("Pairs array sorted by strength of victory\n");

    for (int i = 0; i < pair_count; i++)
    {

    printf("Index: %i     Winner: %8s (%i)     Loser: %8s (%i)    StOV: %i\n",
    i, candidates[pairs[i].winner], preferences[pairs[i].winner][pairs[i].loser],
    candidates[pairs[i].loser], preferences[pairs[i].loser][pairs[i].winner], preferences[pairs[i].winner][pairs[i].loser]);

    }
    printf("\n");
}