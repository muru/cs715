#include<stdio.h>
int main()
{
	int **a,*b;
	int x=10, y=15;
	//a = &x;
	//b =a;
	//scanf("%d:", &y);
	if(y < *b)
	{
		//b++;
		//a = (int*)1000;
		a = &b;
		//*a = 8;
		printf("x=%d  %d, b=%d\n",x,**a,*b);
	}
	printf("%d",**a);	
}
