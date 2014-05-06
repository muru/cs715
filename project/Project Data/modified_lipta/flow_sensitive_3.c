#include "stdio.h"
int a, b, c, *p, *q, **x;

int main() {
	p = &a;
	q = &b;

	while(a) {
		p = q;
		q = *x;
	}
	*p = c;

	printf("%d %d %d %d %d %d", a, b, c, *p, *q, **x);

	return 0;
}

