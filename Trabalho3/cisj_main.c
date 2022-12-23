#include "cisj.h"


int
main(int argc, char** argv)
{
	int i, s, j = -1;
	node_set* nodes;

	if (argc < 3) {
		printf(" Usage: %s i s [j]\n", argv[0]);
		return 1;
	}

	//required params
	i = atoi(argv[1]);
	s = atoi(argv[2]);

	//have we the third parameter?
	if (argc > 3) {
		j = atoi(argv[3]);
		if (!VALID_J(j, s)) {
			printf("%d is not a valid node for a cluster size %d.\n", j, s);
			return 1;
		}
	}
	//calculating cis
	nodes = cis(i, s);

	//print all nodes or just one?
	if (j != -1)
		printf("%i\n", nodes->nodes[j-1]);
	else {
		for (i=0; i < nodes->size; i++)
			printf("%i ", nodes->nodes[i]);
		puts("");
	}
	set_free(nodes);
	return 0;
}

//j indexa o j-ésimo elemento do cluster