#include<stdio.h>
int main() {
	int *p, *q, **r, a, b;

	p = &a;
	q = &b;

	if (a)	p = &b;

	if (b)	r = &p;
	else	r = &q;

	printf("%u", *r);		//PTA:  *r → {a, b}
					//LFCPA:  *r → {}, as 'a' and 'b' are not live
	return 0;
}

