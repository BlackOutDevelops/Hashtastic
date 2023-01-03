// Joshua Frazer jo227789
// COP 3502, Spring 2020
// Using Atom & Linux Bash Shell on Windows 10.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Hashtastic.h"

// Auxilary function copied from primes.c given by Szumlanski
int nextPrime(int n);

// Function to create hash table
HashTable *makeHashTable(int capacity)
{
    int i, count = 1;
    HashTable *hash = malloc(sizeof(HashTable));

    // Validates if hash table was dynamically allocated successfully
    if (hash == NULL)
    {
        free(hash);
        return NULL;
    }

    // Sets capacity of hash table
    if (capacity <= 0)
        hash->capacity = DEFAULT_CAPACITY;
    else
        hash->capacity = capacity;

    hash->hashFunction = NULL;
    hash->array = malloc(sizeof(int) * hash->capacity);

    // Validates if array of hash was dynamically allocated successfully
    if (hash->array == NULL)
    {
        free(hash->array);
        return NULL;
    }

    // for loop that sets UNUSED for all elements in array
    for (i = 0; i < hash->capacity; i++)
        hash->array[i] = UNUSED;

    hash->probing = LINEAR;

    return hash;
}

// Frees the hash table from existence
HashTable *destroyHashTable(HashTable *h)
{
    if (h == NULL)
        return NULL;

    free(h->array);
    free(h);

    return NULL;
}

// Sets the probing type of the hash table
int setProbingMechanism(HashTable *h, ProbingType probing)
{
    if (h == NULL)
        return HASH_ERR;

    if (probing == LINEAR)
        h->probing = LINEAR;
    else if (probing == QUADRATIC)
        h->probing = QUADRATIC;
    else
        return HASH_ERR;

    return HASH_OK;
}

// Sets the hash being used for the hash table
int setHashFunction(HashTable *h, unsigned int (*hashFunction)(int))
{
    if (h == NULL)
        return HASH_ERR;

    h->hashFunction = *hashFunction;
    return HASH_OK;
}

// Checks whether or not the hash table is at least half empty
int isAtLeastHalfEmpty(HashTable *h)
{
    if (h == NULL)
        return 0;

    if (h->capacity > 0)
    {
      if (h->size <= h->capacity/2)
          return 1;
      else
          return 0;
    }
    else
        return 0;
}

// Expand hash table function for when hash table array is at least half empty
int expandHashTable(HashTable *h)
{
    int i, j, *expandedArray, size, index;

    if (h == NULL)
        return HASH_ERR;

    // Inserts old values into new dynamically allocated array through linear probing
    if (h->probing == LINEAR)
    {
        size = (h->capacity * 2 + 1);
        expandedArray = malloc(sizeof(int) * size);

        for (i = 0; i < size; i++)
            expandedArray[i] = UNUSED;

        if (expandedArray == NULL)
            return HASH_ERR;

        for (i = 0; i < h->capacity; i++)
        {
            if (h->array[i] != UNUSED && h->array[i] != DIRTY)
            {
                for (j = 0; j < size; j++)
                {
                    index = (h->hashFunction(h->array[i]) + j) % size;

                    if (expandedArray[index] == UNUSED)
                    {
                        expandedArray[index] = h->array[i];
                        break;
                    }
                    else
                        h->stats.collisions++;
                }
            }
        }
    }
    // Inserts old values into new dynamically allocated array through quadratic probing
    else if (h->probing == QUADRATIC)
    {
        size = nextPrime(h->capacity * 2 + 1);
        expandedArray = malloc(sizeof(int) * size);

        for (i = 0; i < size; i++)
            expandedArray[i] = UNUSED;

        if (expandedArray == NULL)
            return HASH_ERR;

        for (i = 0; i < h->capacity; i++)
        {
            if (h->array[i] != UNUSED && h->array[i] != DIRTY)
            {
                for (j = 0; j < size; j++)
                {
                    index = (h->hashFunction(h->array[i]) + j * j) % size;

                    if (expandedArray[index] == UNUSED)
                    {
                        expandedArray[index] = h->array[i];
                        break;
                    }
                    else
                        h->stats.collisions++;
                }
            }
        }
    }
    // Free old hash array and start anew
    free(h->array);
    h->capacity = size;
    h->array = malloc(sizeof(int) * size);

    // Reinsert old values from the dynamically allocated array to new hash array
    for (i = 0; i < h->capacity; i++)
    {
        h->array[i] = expandedArray[i];
    }

    // Free dynamically allocated array created in this function
    free(expandedArray);
    return HASH_OK;
}

// Inserts whatever key is given to hash table
int insert(HashTable *h, int key)
{
    int index, i, check;

    // Checks if hash is null, hashFunction is null, and if table is atleast half empty
    if (h == NULL)
        return HASH_ERR;
    if (h->hashFunction == NULL)
        return HASH_ERR;
    if (isAtLeastHalfEmpty(h) == 0)
        check = expandHashTable(h);
    if (check == HASH_ERR)
        return HASH_ERR;

    // Inserts through linear probing
    if (h->probing == LINEAR)
        for (i = 0; i < h->capacity; i++)
        {
            index = (h->hashFunction(key) + i) % h->capacity;

            if (h->array[index] == UNUSED)
            {
                h->array[index] = key;
                h->size++;
                h->stats.opCount++;
                return HASH_OK;
            }
            else
                h->stats.collisions++;
        }
    // Inserts through linear probing
    else if (h->probing == QUADRATIC)
        for (i = 0; i < h->capacity; i++)
        {
            index = (h->hashFunction(key) + i * i) % h->capacity;

            if (h->array[index] == UNUSED)
            {
                h->array[index] = key;
                h->size++;
                h->stats.opCount++;
                return HASH_OK;
            }
            else
                h->stats.collisions++;
        }

    return HASH_ERR;
}

// Searches for a key within the hash table
int search(HashTable *h, int key)
{
    int index, i;

    if (h == NULL)
        return -1;
    if (h->hashFunction == NULL)
        return -1;

    // Searches through linear probing
    if (h->probing == LINEAR)
        for (i = 0; i < h->capacity; i++)
        {
            index = (h->hashFunction(key) + i) % h->capacity;

            if (h->array[index] == key)
            {
                h->stats.opCount++;
                return index;
            }
            else if (h->array[index] == UNUSED)
            {
                h->stats.opCount++;
                return -1;
            }
            else
                h->stats.collisions++;
        }
    // Searches through quadratic probing
    else if (h->probing == QUADRATIC)
        for (i = 0; i < h->capacity; i++)
        {
            index = (h->hashFunction(key) + i * i) % h->capacity;

            if (h->array[index] == key)
            {
                h->stats.opCount++;
                return index;
            }
            else if (h->array[index] == UNUSED)
            {
                h->stats.opCount++;
                return -1;
            }
            else
                h->stats.collisions++;
        }

    h->stats.opCount++;
    return -1;
}

// Deletes a key within the hash table
int delete(HashTable *h, int key)
{
    int index, i;

    if (h == NULL)
        return -1;
    if (h->hashFunction == NULL)
        return -1;

    index = search(h, key);

    if (index != -1)
    {
        h->array[index] = DIRTY;
        h->size--;
        return index;
    }
    else
        return -1;
}

double difficultyRating(void)
{
    return 2.0;
}

double hoursSpent(void)
{
    return 6.5;
}

// Auxilary function copied from primes.c given by Szumlanski
int nextPrime(int n)
{
    int i, root, keepGoing = 1;

    if (n % 2 == 0)
    	++n;

    while (keepGoing)
    {
    	keepGoing = 0;

    	// =============================================================
    	// Compilation Note Related to sqrt() Function
    	// =============================================================
    	// Be sure to #include <math.h> and use the -lm flag to compile:
    	//
    	//    gcc Hashtastic.c -lm
    	// =============================================================

    	root = sqrt(n);

    	for (i = 3; i <= root; i++)
    	{
    		if (n % i == 0)
    		{
    			// Move on to the next candidate for primality. Since n is odd,
    			// we don't want to increment it by 1. That would give us an
    			// even integer greater than 2, which would necessarily be
    			// non-prime.
    			n += 2;
    			keepGoing = 1;

    			// Leave for-loop. Move on to next iteration of while-loop.
    			break;
    		}
    	}
    }

    return n;
}
