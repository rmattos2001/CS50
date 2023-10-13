// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 25876; // Looks like a nice number, hash of 45 z's + 1 idk

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // The idea is the same as in free_bucket(), but instead of calling free;
    // We compare the strings
    node *cursor = table[hash(word)];
    if (cursor != NULL)
    {
        while (cursor->next != NULL)
        {
            if (strcasecmp(word, cursor->word) == 0)
            {
                return true;
            }
            cursor = cursor->next;
        }
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    //return toupper(word[0]) - 'A';
    int hash = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        hash += (tolower(word[i]) - 'a') * (i + 1);
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    return false;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    return false;
}
