// Sean Szumlanski
// COP 3502, Spring 2020

// ========================
// Hashtastic: testcase01.c
// ========================
// A test of hash table insertion with quadratic probing. Also tests collision
// counting, op counting, and hash table expansion.


#include "Hashtastic.h"
#include <stdio.h>
#include <stdlib.h>

unsigned int hash(int key)
{
	return ((key / 10) % 10) * 37 + (key % 10);
}

void check(HashTable *h, int capacity, int ops, int collisions)
{
	if (h->capacity != capacity || h->stats.opCount != ops || h->stats.collisions != collisions)
	{
		printf("[Expected] capacity: %d, ops: %d, collisions: %d\n", capacity, ops, collisions);
		printf("[Actual] capacity: %d, ops: %d, collisions: %d\n", h->capacity, h->stats.opCount, h->stats.collisions);
		printf("\nfail whale :(\n");
		exit(1);
	}
}

void failwhale(int index, int key)
{
	printf("Incorrect value in final hash table at index %d: %d\n", index, key);
	printf("\nfail whale :(\n");
	exit(1);
}

int main(void)
{
	int capacity, ops, collisions, i;

	// This is how the final hash table should be populated in this test case.
	int result[] = {
		UNUSED, UNUSED, 54, 26, UNUSED, UNUSED, 83, UNUSED, 14, UNUSED, UNUSED
	};

	HashTable *h = makeHashTable(5);
	setHashFunction(h, hash);
	setProbingMechanism(h, QUADRATIC);
	check(h, capacity = 5, ops = 0, collisions = 0);

	// Current Hash Table:
	// +----+----+----+----+----+
	// |    |    |    |    |    |
	// +----+----+----+----+----+
	//   0    1    2    3    4

	insert(h, 26);
	check(h, capacity = 5, ops = 1, collisions = 0);

	// Current Hash Table (after inserting 26):
	// +----+----+----+----+----+
	// | 26 |    |    |    |    |
	// +----+----+----+----+----+
	//   0    1    2    3    4

	insert(h, 83);
	check(h, capacity = 5, ops = 2, collisions = 0);

	// Current Hash Table (after inserting 83):
	// +----+----+----+----+----+
	// | 26 |    |    |    | 83 |
	// +----+----+----+----+----+
	//   0    1    2    3    4

	insert(h, 54);
	check(h, capacity = 5, ops = 3, collisions = 2);

	// Current Hash Table (after inserting 54, which collided with 83 and 26):
	// +----+----+----+----+----+
	// | 26 |    |    | 54 | 83 |
	// +----+----+----+----+----+
	//   0    1    2    3    4

	insert(h, 14);
	check(h, capacity = 11, ops = 4, collisions = 4);

	// Inserting 14 causes an expansion, which results in a few collision.
	// During the expansion, the keys are inserted in the following order
	// (because this is the order in which they appear in the array, from left
	// to right):
	//
	//    26, 54, 83
	//
	// As those keys are inserted, here's how the table changes at each step:
	//
	//    26 hashes to index 3 (with no collisions).
	//    +----+----+----+----+----+----+----+----+----+----+----+
	//    |    |    |    | 26 |    |    |    |    |    |    |    |
	//    +----+----+----+----+----+----+----+----+----+----+----+
	//      0    1    2    3    4    5    6    7    8    9    10
	//
	//    54 hashes to index 2 (with no collisions).
	//    +----+----+----+----+----+----+----+----+----+----+----+
	//    |    |    | 54 | 26 |    |    |    |    |    |    |    |
	//    +----+----+----+----+----+----+----+----+----+----+----+
	//      0    1    2    3    4    5    6    7    8    9    10
	//
	//    83 hashes to index 2 and collides with 54 and 26 before
	//    ending up at index 6 in the array.
	//    +----+----+----+----+----+----+----+----+----+----+----+
	//    |    |    | 54 | 26 |    |    | 83 |    |    |    |    |
	//    +----+----+----+----+----+----+----+----+----+----+----+
	//      0    1    2    3    4    5    6    7    8    9    10
	//
	// Finally, after the table is properly expanded and the collision count
	// is incremented from 2 to 4 (to account for the two collisions that 83
	// encountered), 14 is inserted into the table. It hashes to index 8:
	//
	// +----+----+----+----+----+----+----+----+----+----+----+
	// |    |    | 54 | 26 |    |    | 83 |    | 14 |    |    |
	// +----+----+----+----+----+----+----+----+----+----+----+
	//   0    1    2    3    4    5    6    7    8    9    10

	for (i = 0; i < 11; i++)
		if (h->array[i] != result[i])
			failwhale(i, h->array[i]);

	h = destroyHashTable(h);

	printf("Everything seems to be unfolding according to plan...\n");
	return 0;
}
