#include<stdio.h>

int fun1(int (*fp3) ())
{
	printf("\nin fun1\n");
	
	return 2 * fp3();
}

int fun2()
{
	printf("\nin fun2\n");
	return 3;
}
int fun3()
{
	printf("\nin fun3\n");
	return 4;
}

int fun4()
{
	printf("\nin fun4\n");
	return 5;
}

int fun5()
{
	printf("\nin fun5\n");
	return 6;
}

int fun6()
{
	printf("\nin fun6\n");
	return 7;
}

int fun7()
{
	printf("\nin fun7\n");
	return 8;
}

int main()
{
	int i;
	scanf ("%d", &i);

	switch (i)
	{
		case 0:
		fun1 (fun3); break;
		case 2:
		fun1 (fun4); fun1 (fun5); break;
		case 1:
		fun1 (fun6); break;
		case 3:
		fun1 (fun2); fun1 (fun4); break;
		default:
		fun1 (fun3); fun1 (fun6); break;

	}
}
