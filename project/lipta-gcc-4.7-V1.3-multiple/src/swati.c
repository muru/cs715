#include<stdio.h>
int f();
int main()
{
	int ***a, **b, *z, **x, u,**p, *y;

	b = &z;

	u = f();
	a = &x;
	/*if(u)
		a=&x;
	else
	{
		a=&p;
		x=&y;
	}*/

	*a = b;
	y=*x;

	printf("%d %d", a,x);
}
