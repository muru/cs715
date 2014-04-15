#include<stdio.h>
#include<stdlib.h>
int funct(int);
int main()
{
	int q, r;
	q = funct(r);
	//scanf("%d", &q);
	int *p = (int*)malloc(sizeof(p));
	int **z = (int**)malloc(sizeof(z));
	if(q <9)
	   p = &r;
	//*z = p;
	z++;
	printf("%d %d", p, z);
	return 0;
}
