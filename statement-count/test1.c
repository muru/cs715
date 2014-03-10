#include "test.h"

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
	printf("%d\n", a);
	return a;
}
	
