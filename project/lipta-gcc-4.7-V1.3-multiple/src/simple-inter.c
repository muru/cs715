#include<stdio.h>
int x, y;
int *a, *b,c, d;
void funct()
{
	if(x == y)
		a = &y;
	else
		a = &x;
}
int main()
{
	//int *a, *b, c,d;
	a = b;
	b = &d;
	funct();
	printf("%d", a);
}

	
	
