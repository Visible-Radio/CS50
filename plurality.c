#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;                    // candidate.name
    int votes;                      // candidate.votes
}
candidate;

// Array of candidates
candidate candidates[MAX];          //create 9 instances of struct 'candidate' inside an array called 'candidates'
                                    // access: candidates[index].name or candidates[index].votes
// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
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
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))                    // if vote() returns false
        {
            printf("Invalid vote.\n");
            //i--;                          // if the user enters someone not on the ballot, simply decrement the index so they can try again
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    //search for the name the voter entered in candidates[index].name
    //if the name is found, increment candidates[index].votes += 1
    for (int i=0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            candidates[i].votes += 1;
            return true;
        }
    }

    // if the user enters a name that is not on the ballot, return false.  Handling above will output "invalid vote"
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{

    //determine what the greatest number of vote was
    int mostVotes = 0;
    for (int i=0; i < candidate_count; i++)
    {
        if (candidates[i].votes > mostVotes)
        {
            mostVotes = candidates[i].votes;
        }
    }
    //iterate through again to see who the winners are
    for (int i=0; i < candidate_count; i++)
    {
        if (candidates[i].votes == mostVotes){
            printf("%s\n", candidates[i].name);
        }
    }

    return;
}

