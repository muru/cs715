#include<stdio.h>
int foo(int);
int is_odd(int);

int foo(int a)
{
	static int i = 0;
	i++;
	return (a) + i;
}

int is_odd (int b)
{
	return foo(b) % 2;
}

int main()
{
	int n = 10, j =3, a;
	for (int i = 0; i < n; i++)
	{
		j = foo(j);
		if (is_odd(j))
			a = j;
		else
			a = foo(j % 3);
	}
	return a;
}
	
