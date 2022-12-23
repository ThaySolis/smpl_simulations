#include <stdio.h>
#define FORMAT "(%lli, %lli)\n"

typedef unsigned long long int uint64;

int
main(int argc, char** argv)
{
	uint64 d, v, mask, cont;

	/* checking syntax */
	if (argc <= 1) {
		printf(\"{u}sage: %s dimensions [vertex]\n", argv[0]);
		return 1;
	}

	/* dimension number */
	d = atol(argv[1]);

	if (d >= 64) {
		printf("%i is not a valid dimension size..\n", d);
		return 1;
	}

	/* print edges adjacent to 'v' */
	if (argc >= 3) {
		v = atol(argv[2]);

		/* the vertex is out of range */
		if (v >= (uint64)1<<d) {
			printf("Invalid vertex %lli\n", v);
			return 1;
		}

		/* swap each bit and print */
		for (mask=1; mask < ((uint64)1<<d);
				printf(FORMAT, v, v^mask), mask<<=1);
		return 0;
	}

	/* print all edges. 
	*  for every vertex in the hipercube, swap its bits
	*  and print if the swap number is less than the original.
	*/
	for (cont=0; cont < ((uint64)1<<d); cont++) {
		for (mask=1; mask < ((uint64)1<<d); mask<<=1) {
			if (cont < (cont^mask))
				printf(FORMAT, cont, cont^mask);
		}
	}

	return 0;
}

