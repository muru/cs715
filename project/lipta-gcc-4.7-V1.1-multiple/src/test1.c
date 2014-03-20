#include <stdio.h>
extern void funct(int *, int *);
int main ()
{
	int *p, *q, s, t;
	q = &s;
	p = &t;
	//while(*q > 0)
	{
		*q--;
		p = q;
		funct(p,q);
		printf ("%d%d%d", *p, *q, s);
        }
} 
