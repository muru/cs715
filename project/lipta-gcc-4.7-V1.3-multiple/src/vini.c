#include<stdio.h>
int main()
{
	int **p, *q, r, **z;
	int ***a;
	a=&p;
	p=&q;
	q=&r;
	while(r < 10)
	{
		a=*a;
	}
	return a;
}
