// Sean Szumlanski
// COP 3502, Spring 2020

// ========================
// Hashtastic: testcase03.c
// ========================
// A test of deletion and search with quadratic probing. Also tests collision
// counting, op counting, hash table expansion, and the creation of a hash table
// with an invalid capacity. This is very similar to testcase01.c.


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

void index_check(int key, int actual_index, int expected_index, int op)
{
	if (actual_index != expected_index)
	{
		printf("Incorrect %s index for %d: %d (actual) vs. %d (expected)\n",
			   (op ? "search" : "deletion"), key, actual_index, expected_index);
		printf("\nfail whale :(\n");
		exit(1);
	}
}

int main(void)
{
	int capacity, ops, collisions, i, key, index, expected_index;

	// This is how the final hash table should be populated in this test case.
	int result[] = {
		UNUSED, 30, 54, DIRTY, UNUSED, UNUSED, UNUSED, UNUSED, 14, UNUSED, UNUSED
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

	index = delete(h, key =  83);
	index_check(key, index, expected_index = 4, 0);
	check(h, capacity = 5, ops = 4, collisions = 2);

	// Current Hash Table (after deleting 83 and marking that position DIRTY):
	// +----+----+----+----+----+
	// | 26 |    |    | 54 |DIRT|
	// +----+----+----+----+----+
	//   0    1    2    3    4

	index = delete(h, key = 83);
	index_check(key, index, expected_index = -1, 0);
	check(h, capacity = 5, ops = 5, collisions = 7);

	// Current Hash Table (after attempting to delete 83 and colliding with
	// indices 4, 0, 3, 3, and 0 before finally giving up because it has
	// gone through 5 iterations (which is equal to the hash table's capacity,
	// which is the maximum number of probing iterations you should go through
	// while performing a search or delete operation)).
	// +----+----+----+----+----+
	// | 26 |    |    | 54 |DIRT|
	// +----+----+----+----+----+
	//   0    1    2    3    4

	insert(h, 14);
	check(h, capacity = 5, ops = 6, collisions = 7);

	// Current Hash Table (after inserting 14 with no collisions):
	// +----+----+----+----+----+
	// | 26 | 14 |    | 54 |DIRT|
	// +----+----+----+----+----+
	//   0    1    2    3    4

	insert(h, 30);
	check(h, capacity = 11, ops = 7, collisions = 7);

	// Inserting 30 causes an expansion during which the keys are inserted in
	// the following order (because this is the order in which they appear in
	// the array, from left to right):
	//
	//    26, 14, 54
	//
	// Notice that the dirty bit does not get inserted into the new table.
	//
	// As those keys are inserted, here's how the table changes at each step:
	//
	//    26 hashes to index 3 (with no collisions).
	//    +----+----+----+----+----+----+----+----+----+----+----+
	//    |    |    |    | 26 |    |    |    |    |    |    |    |
	//    +----+----+----+----+----+----+----+----+----+----+----+
	//      0    1    2    3    4    5    6    7    8    9    10
	//
	//    14 hashes to index 8 (with no collisions).
	//    +----+----+----+----+----+----+----+----+----+----+----+
	//    |    |    |    | 26 |    |    |    |    | 14 |    |    |
	//    +----+----+----+----+----+----+----+----+----+----+----+
	//      0    1    2    3    4    5    6    7    8    9    10
	//
	//    54 hashes to index 2 (with no collisions).
	//    +----+----+----+----+----+----+----+----+----+----+----+
	//    |    |    | 54 | 26 |    |    |    |    | 14 |    |    |
	//    +----+----+----+----+----+----+----+----+----+----+----+
	//      0    1    2    3    4    5    6    7    8    9    10
	//
	// Finally, after the table is properly expanded, 30 is inserted into the
	// table. It hashes to index 1:
	//
	// +----+----+----+----+----+----+----+----+----+----+----+
	// |    | 30 | 54 | 26 |    |    |    |    | 14 |    |    |
	// +----+----+----+----+----+----+----+----+----+----+----+
	//   0    1    2    3    4    5    6    7    8    9    10

	delete(h, 26);
	check(h, capacity = 11, ops = 8, collisions = 7);

	// Current Hash Table (after deleting 26 with no collisions):
	// +----+----+----+----+----+----+----+----+----+----+----+
	// |    | 30 | 54 |DIRT|    |    |    |    | 14 |    |    |
	// +----+----+----+----+----+----+----+----+----+----+----+
	//   0    1    2    3    4    5    6    7    8    9    10

	delete(h, 82);
	check(h, capacity = 11, ops = 9, collisions = 9);

	// Current Hash Table (after attempting to delete 82, which hashes to index
	// 1 and encounters two collisions (with indices 1 and 2) before landing at
	// index 5 and terminating because that cell is empty):
	// +----+----+----+----+----+----+----+----+----+----+----+
	// |    | 30 | 54 |DIRT|    |    |    |    | 14 |    |    |
	// +----+----+----+----+----+----+----+----+----+----+----+
	//   0    1    2    3    4    5    6    7    8    9    10

	// Searching for 30 (which hashes to index 1) should result in 0 collisions.
	index = search(h, key = 30);
	index_check(key, index, expected_index = 1, 1);
	check(h, capacity = 11, ops = 10, collisions = 9);

	// Searching for 47 (which hashes to index 1) should result in 2 collisions.
	index = search(h, key = 47);
	index_check(key, index, expected_index = -1, 1);
	check(h, capacity = 11, ops = 11, collisions = 11);

	// Searching for 77 (which hashes to index 2) should result in 2 collisions.
	index = search(h, key = 77);
	index_check(key, index, expected_index = -1, 1);
	check(h, capacity = 11, ops = 12, collisions = 13);

	for (i = 0; i < 11; i++)
		if (h->array[i] != result[i])
			failwhale(i, h->array[i]);

	h = destroyHashTable(h);

	printf("Everything seems to be unfolding according to plan...\n");
	return 0;
}
