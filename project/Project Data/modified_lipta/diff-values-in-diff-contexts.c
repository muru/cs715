#include<stdio.h>

int (*fp3)();
int fun1()
{
	fp3();
	printf("\nin fun1\n");
	return 2;
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

int main()
{

	fp3 = fun2;
	fun1();

	fp3 = fun3;
	fun1();

	fp3 = fun2;
	fun1();

}
