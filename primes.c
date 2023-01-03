// Sean Szumlanski
// COP 3502, Spring 2020

// ====================
// Hashtastic: primes.c
// ====================
// A function for finding prime numbers. You can copy and paste the nextPrime()
// function into your source code for this assignment.


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Finds the next prime number greater than or equal to n. This function assumes
// that n is strictly greater than 2. If n is exactly equal to 2, this function
// actually returns 3.
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

// You can use this at the command line to generate primes.
int main(int argc, char **argv)
{
	int n;

	if (argc < 2)
	{
		printf("Proper syntax: %s <n>\n", argv[0]);
		return 1;
	}

	n = atoi(argv[1]);
	printf("%d\n", nextPrime(n));

	return 0;
}
