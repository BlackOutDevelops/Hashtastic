#ifndef __HASHTASTIC_H
#define __HASHTASTIC_H


#include <limits.h>

#define UNUSED   INT_MIN
#define DIRTY    INT_MAX
#define HASH_ERR 0
#define HASH_OK  1
#define DEFAULT_CAPACITY 5


typedef enum ProbingType { LINEAR, QUADRATIC } ProbingType;

typedef struct HashStats
{
	// This field keeps track of how many insert, search, or delete operations
	// take place over this hash table's lifetime.
	int opCount;

	// This field keeps track of how many collisions occur when performing
	// insert, search, or delete operations on this hash table.
	int collisions;

} HashStats;

typedef struct HashTable
{
	// Your hash table will store integer keys in this array.
	int *array;

	// The current capacity of your hash table (the length of 'array').
	int capacity;

	// The size of your hash table (the number of elements it contains).
	int size;

	// A pointer to the hash function for this hash table (initially NULL).
	unsigned int (*hashFunction)(int);

	// Probing type: LINEAR or QUADRATIC. Initialize to LINEAR by default.
	ProbingType probing;

	// A struct within a struct for maintaining stats on this hash table:
	// number of operations performed and number of collisions encountered.
	HashStats stats;

} HashTable;


// Functional Prototypes

HashTable *makeHashTable(int capacity);

HashTable *destroyHashTable(HashTable *h);

int setProbingMechanism(HashTable *h, ProbingType probing);

int setHashFunction(HashTable *h, unsigned int (*hashFunction)(int));

int isAtLeastHalfEmpty(HashTable *h);

int expandHashTable(HashTable *h);

int insert(HashTable *h, int key);

int search(HashTable *h, int key);

int delete(HashTable *h, int key);

double difficultyRating(void);

double hoursSpent(void);


#endif
