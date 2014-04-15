#include<stdio.h>
int main()
{
	int *a, *b, *c, *d;
	int p, q, r;
	a = &p;
	c = &r;
	d = b;
	b = &q;
	d = a;
	//if(*d == 9)
	//	d = c;
	printf("%d %d",*b, *d);
}
