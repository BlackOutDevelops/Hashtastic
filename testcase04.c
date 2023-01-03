// Sean Szumlanski
// COP 3502, Spring 2020

// ========================
// Hashtastic: testcase04.c
// ========================
// A test of the average number of collisions per operation, over a large number
// of operations. It turns out this hash function is pretty terrible. Can you
// examine the hash function and figure out why it's not so great?


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

int main(void)
{
	int i;
	double avg;

	HashTable *h = makeHashTable(5);
	setHashFunction(h, hash);
	setProbingMechanism(h, QUADRATIC);

	for (i = 0; i < 10000; i++)
		insert(h, i);

	// Print hash table report.
	avg = (double)h->stats.collisions / h->stats.opCount;
	printf("Number of collisions: %d\n", h->stats.collisions);
	printf("Number of operations: %d\n", h->stats.opCount);
	printf("Average collisions per op: %0.2f\n", avg);

	if (avg < 4.0)
		printf("\nThis hash function looks pretty good!\n\n");
	else
		printf("\nThis hash function is kind of terrible.\n\n");

	// Check table stats.
	check(h, 25717, 10000, 1364798);

	h = destroyHashTable(h);

	printf("Everything seems to be unfolding according to plan...\n");
	return 0;
}
