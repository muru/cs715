#include <stdio.h>
/*
  Here points-to information differs for pta and lipta because of flow-sensitivity and liveness
*/

int main() {
	int *p, a, b;

	if (b)
		p = &a;

	printf("%d", *p);			//PTA: p.1_2 -> {a, b}; LIPTA: p.1_2 -> {a}

	p = &b;
	printf("%d %u",*p, &p);			//PTA: p -> {a, b}; LIPTA: p -> {}
						//since *p is accessed directly as 'b' (through a temporary), so no use of p -> b
	return 0;
}

