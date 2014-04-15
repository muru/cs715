#include<stdio.h>

void fun1()
{
	printf("In fun1\n");
}

void (*fp)() = fun1;

void fun2()                     
{                                                                                       
        printf("In fun2\n");                                                            
}

void fun3()                     
{                                                                                       
        printf("In fun3\n");                                                            
}

void fun4()                     
{                                                                                       
        printf("In fun4\n");                                                            
}

void fun5()                     
{                                                                                       
        printf("In fun5\n");                                                            
}

int main()
{
	void (*fp1[5])() = {fun1,fun2,fun3,fun4,fun5} ;
       int n;
	fp1[2]();
 	int i,j,*x,*z,y;
	int **a[6], *c[6];
	int b[] = {1,2,3,4,4,5};
	for(i=0;i<n;i++)
	{
		fp1[i]();
		printf("hello");
	}
	n =10;
	for(i = 0, j =2 ; i<n; i++)
		//a[i] = &b[i];
	{
		a[i] = &x;
		a[j] = &z;
		c[i] = &y;
		*(a[i]) = c[i];
	}
	
	printf("%d", *a[i-1]);
	//fp();
}
