#include<stdio.h>
int main()
{
	int a, b, c;
	int **z, *y;
	scanf("%d %d",&a, &c);
	y = &c;
	z = (int **)&a;
	if (c)
		return 0;
	*z = y;
	z  = (int **) &b;
	printf("%p", z);
}
