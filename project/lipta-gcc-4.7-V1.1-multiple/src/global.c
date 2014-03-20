#include<stdio.h>
int *p, *y ,b , **z;
int q,r;
int *a = &b;
int funct();
void f1()
{
	int j;
	while(j !=0)
	{
		p++;
		j--;
	}
	//b = &r;
}
int main()
{
	z = &p;
	//a = &r;
	p = &q;
	int c;
	c = funct();
	//scanf("%d", &c);
	if(!c)
		a = *z;
	//f1();
	printf("%d %d\n",*p, a);
	if(c)
	{
		y = &r;
		z = &y;
		p = *z;
	}
	//else
	//	f1();
	printf("%d", p);
}
