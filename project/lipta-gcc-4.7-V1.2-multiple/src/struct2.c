#include<stdio.h>
struct abc
{
	int **p;
	int **q;
};
int main()
{
	int *a, c, *b, ***y;
	struct abc z;
	z.p = &a;
	z.q = &b;
	y = &z.p;
	a = &c;
	z.q = *y;
	printf("%d %d", z.p, z.q);
}
