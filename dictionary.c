// Implements a dictionary's functionality
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

const unsigned int N = 300017;  // Number of buckets in hash table 300017
node* table[N];
int entry_count = 0;
bool loaded = false;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
   // call hash function on incoming argument
   // if table[hash(word)] == NULL, return false
   // if table[hash(word)] != NULL, look through the linked list for the word
        // if you reach ptr->next==NULL without finding the word, return false

    char lower_case_word[LENGTH+1];
    strcpy(lower_case_word,word);

    for (int i=0; i < strlen(lower_case_word); i++)
    {
        lower_case_word[i]=tolower(lower_case_word[i]);
    }


    unsigned int hash_index = hash(word);
    node* ptr = table[hash_index];

    if (ptr == NULL)
    // she no there
    {
        return false;
    }

    else if (strcmp(ptr->word, lower_case_word) == 0)
    // she here
    {
        return true;
    }
    else
    {
    // goootta look for her
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
            if (strcmp(ptr->word, lower_case_word) == 0)
            // she here
            {
                return true;

            }

        }
        return false;
    }
}



// Hashes word to a number
unsigned int hash(const char *word)
{
    /*
    FROM:
    https://github.com/hathix/cs50-section/blob/master/code/7/sample-hash-functions/good-hash-function.c
    */
    unsigned long hash = 5381;
    for (const char* ptr = word; *ptr != '\0'; ptr++)
    {
     hash = ((hash << 5) + hash) + toupper(*ptr);
    }
    return hash % N;

}

bool load(const char *dictionary)
/*Loads dictionary into memory, returning true if successful
else false. char* dictionary read from command line and passed
to load function by driver program */
{
    printf("Bytes for empty hash table: %lu \n", sizeof(node)*N);
    printf("Bytes per int %lu \n", sizeof(int));

    //Open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (!file)
    {
        return false;
    }

    int collision_count = 0;
    char word_in[LENGTH +1];
    unsigned int hash_index;
    unsigned int nodes_created =0;

    // set all initial slots to NULL
    for (unsigned int i=0; i < N; i++)
    {
        table[i] = NULL;
    }


    //loop through dicitionary file
    while (fscanf (file, "%s", word_in)!=EOF)
    {

        node* new_node = malloc(sizeof(node));
        if (new_node==NULL)
        {
            free(new_node);
            printf("Couldn't malloc");
            exit(1);

        }
        strcpy(new_node->word, word_in);
        hash_index = hash(word_in);
        nodes_created++;

        if (table[hash_index] == NULL)
        // first node
        {

            new_node->next = NULL;
            table[hash_index] = new_node;

        }

        else //if (table[hash_index] != NULL)
        // This slot already contains other items
        {
            collision_count++;

            new_node->next = table[hash_index];
            table[hash_index] = new_node;

        }

        entry_count++;

        //printf("%s \n", table[hash_index]->word);
        //free(node);
    }

    printf("entry count: %i\n", entry_count);
    printf("collision count: %i\n", collision_count);
    printf("nodes created: %i\n", nodes_created);
    fclose(file);
    loaded = true;




    return true;
}

unsigned int size(void)
// Returns number of words in dictionary if loaded else 0 if not yet loaded
{
    if (loaded == true)
    {
        return entry_count;
    }
    else
    {
        return 0;
    }

}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    unsigned int nodes_freed = 0;
    unsigned int void_slot = 0;
    /*
    Free every node we malloc'd
    with a temporary pointer
    go to head of list
    if node->next is NULL
        free that node
    if node->next is not NULL
        set ptr->next to node->next
        free current node
    */

    // iterate through hash table top to bottom
    for (unsigned int i=0; i < N; i++)
    {

        if (table[i] == NULL)
        {
            void_slot++;
            continue;
        }
        else

        {
            if (table[i]->next == NULL)
            {
                free(table[i]);
                nodes_freed++;
            }
            else
            {
                node* head = table[i];
                node* ptr = head;
                while (head != NULL)
                {
                    /*
                    point to what will be the new head
                    remove the old head
                    move head pointer to new head
                    */

                    ptr = head->next;
                    free(head);
                    head = ptr;
                    nodes_freed++;



                }
            }
        }
    }

    printf("Nodes freed: %i \n", nodes_freed);
    printf("Void slots: %i \n", void_slot);
    return true;
}
